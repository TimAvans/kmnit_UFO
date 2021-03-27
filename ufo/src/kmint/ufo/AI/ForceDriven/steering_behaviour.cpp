#include "kmint/ufo/steering_behaviour.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/c2d_matrix.hpp>
#include <kmint/ufo/saucer.hpp>
#include <kmint/ufo/geometrics.hpp>

namespace kmint {
	namespace ufo {
		steering_behaviour::steering_behaviour(moving_entity* agent) :
			agent_(agent),
			feelers_(3)
		{
			double theta = random_scalar(0.0, 1.0) * (math::pi * 2.0);

			agent_->wander_target_ = math::vector2d(agent_->wander_radius_ * cos(theta),
				agent_->wander_radius_ * sin(theta));


		}

		math::vector2d steering_behaviour::pursuit(play::actor* target) {
			//std::cout << target->location() << std::endl;
				//if the evader is ahead and facing the agent then we can just seek foor the evader's current position
			try {
				if (auto x = dynamic_cast<moving_entity*>(target)) {

					math::vector2d to_target = x->location() - agent_->location();

					double relative_heading = math::dot(agent_->heading_, x->heading_);


					//0.95 = 18degs
					if ((math::dot(to_target, agent_->heading_) > 0) && (relative_heading < -0.95)) {
						return seek(x->location());
					}

					//not considered ahead so we predict where the evader will be.

					//the look-ahead time is proportional to the distance between the evader
					//and the pursuer; and is inversely proportional to the sum of the agents'velocities
					double target_speed = sqrt(x->velocity_.x() * x->velocity_.x() + x->velocity_.y() * x->velocity_.y());

					double look_ahead_time = sqrt(to_target.x() * to_target.x() + to_target.y() * to_target.y()) / (agent_->max_speed_ + target_speed);

					//now seek to the predicted future position of the evader
					return seek(x->location() + x->velocity_ * look_ahead_time) * agent_->pursuit_weight_;
				}
				return agent_->heading_;
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << std::endl;
				return agent_->heading_;
			}
		}

		math::vector2d steering_behaviour::seek(math::vector2d target_location) {
			math::vector2d desired_velocity = math::normalize(target_location - agent_->location()) * agent_->max_speed_;

			return (desired_velocity - agent_->velocity_) * agent_->seek_weight_;
		}

		void steering_behaviour::set_seek_target(play::actor* target)
		{
			seek_target_ = target;
		};

		void steering_behaviour::set_flee_target(play::actor* target)
		{
			flee_target_ = target;
		};

		math::vector2d steering_behaviour::calculate() {
			steering_force_.x(0);
			steering_force_.y(0);

			steering_force_ += wall_avoidance();

			steering_force_ += cohesion(agent_->find_neighbours());
			steering_force_ += alignment(agent_->find_neighbours());
			steering_force_ += separation(agent_->find_neighbours());

			steering_force_ += wander();


			if (flee_target_ != NULL) {
				steering_force_ += flee(flee_target_->location());
			}
			if (seek_target_ != NULL) {
				steering_force_ += seek(seek_target_->location());
			}

			if (sqrt(steering_force_.x() * steering_force_.x() + steering_force_.y() * steering_force_.y()) > agent_->max_force_)
			{
				steering_force_ = math::normalize(steering_force_);

				steering_force_ *= agent_->max_force_;
			}

			return steering_force_;
		}

		math::vector2d steering_behaviour::wander() {

			//first, add a small random vector to the target�s position (RandomClamped
			//returns a value between -1 and 1)
			float rand = random_clamped();
			math::vector2d wander_target = math::vector2d(rand * agent_->wander_jitter_, rand * agent_->wander_jitter_);

			//reproject this new vector back onto a unit circle
			math::normalize(wander_target);
			//increase the length of the vector to the same as the radius
			//of the wander circle
			wander_target *= agent_->wander_radius_;
			//move the target into a position WanderDist in front of the agent
			math::vector2d target_local = wander_target + math::vector2d(agent_->wander_distance_, 0);
			//project the target into world space
			math::vector2d target_world = geo.point_to_world_space(target_local, agent_->heading_, agent_->side_, agent_->location());

			//and steer toward it
			return target_world - agent_->location();
		}

		math::vector2d steering_behaviour::wall_avoidance() {

			create_feelers();

			double distToThisIP = 0.0;
			double distToClosestIP = DBL_MAX;

			int closest_wall = -1;

			math::vector2d steering_force;
			math::vector2d point;
			math::vector2d closest_point;

			//feelers
			for (int flr = 0; flr < feelers_.size(); ++flr) {
				//walls
				for (int w = 0; w < agent_->walls_.size(); ++w) {
					if (geo.line_intersection(agent_->location(), feelers_[flr], agent_->walls_[w].from, agent_->walls_[w].to, distToThisIP, point)) {
						if (distToThisIP < distToClosestIP) {
							distToClosestIP = distToThisIP;
							closest_wall = w;
							closest_point = point;
						}
					}
				}

				//if intersection point is found
				if (closest_wall >= 0) {
					math::vector2d overshoot = feelers_[flr] - closest_point;
					steering_force = agent_->walls_[closest_wall].normal * (sqrt(overshoot.x() * overshoot.x() + overshoot.y() * overshoot.y()));
				}
			}

			return steering_force * agent_->wall_avoidance_weight_;
		}

		math::vector2d steering_behaviour::separation(const std::vector<moving_entity*>& neighbours) {
			math::vector2d steering_force;

			for (int a = 0; a < neighbours.size(); ++a) {
				if (neighbours[a] != agent_) {
					math::vector2d distance_to_agent = agent_->location() - neighbours[a]->location();

					double length = sqrt(distance_to_agent.x() * distance_to_agent.x() + distance_to_agent.y() * distance_to_agent.y());
					steering_force += math::normalize(distance_to_agent) / length;
				}
			}

			return steering_force * agent_->separation_weight_;
		}

		math::vector2d steering_behaviour::alignment(const std::vector<moving_entity*>& neighbours) {
			math::vector2d average_heading;

			int neighbour_count = 0;

			for (int a = 0; a < neighbours.size(); ++a) {
				if (neighbours[a] != agent_) {
					average_heading += neighbours[a]->heading_;

					++neighbour_count;
				}
			}

			if (neighbour_count > 0) {
				average_heading /= (double)neighbour_count;
				average_heading = agent_->heading_;
			}

			return average_heading * agent_->alignment_weight_;
		}

		math::vector2d steering_behaviour::cohesion(const std::vector<moving_entity*>& neighbours) {
			math::vector2d center;
			math::vector2d steering_force;

			int neighbour_count = 0;

			for (int a = 0; a < neighbours.size(); ++a) {
				if (neighbours[a] != agent_) {
					center += neighbours[a]->location();

					++neighbour_count;
				}
			}

			if (neighbour_count > 0) {
				center /= (double)neighbour_count;
				steering_force = seek(center);
			}

			return steering_force * agent_->cohesion_weight_;
		}

		void steering_behaviour::create_feelers() {
			//feeler in front
			feelers_[0] = agent_->location() + agent_->feeler_length_ * agent_->heading_;

			//feeler left
			math::vector2d temp = agent_->heading_;
			geo.rotate_vec_around_origin(temp, (math::pi / 2) * 3.5f);
			feelers_[1] = agent_->location() + agent_->feeler_length_ / 2.0f * temp;

			//feeler right
			temp = agent_->heading_;
			geo.rotate_vec_around_origin(temp, math::pi * 0.5f);
			feelers_[2] = agent_->location() + agent_->feeler_length_ / 2.0f * temp;
		}

		float steering_behaviour::random_clamped() {
			return random_scalar(-1.0, 1.0);
		}

		

		math::vector2d steering_behaviour::flee(math::vector2d target_location) {

			math::vector2d target_pos;


			target_pos = target_location;

			math::vector2d desired_velocity = math::normalize(agent_->location() - target_pos) * agent_->max_speed_;

			return (desired_velocity - agent_->velocity_) * agent_->flee_weight_;
	}
}
}