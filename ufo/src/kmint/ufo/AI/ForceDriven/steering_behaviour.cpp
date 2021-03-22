#include "kmint/ufo/steering_behaviour.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/c2d_matrix.hpp>

namespace kmint {
	namespace ufo {
		steering_behaviour::steering_behaviour(moving_entity* agent, double jitter, double radius, double distance) : 
			agent_(agent), 
			feelers_(3),
			wander_jitter_(jitter),
			wander_radius_(radius),
			wander_distance_(distance)
		{
			double theta = random_scalar(0.0, 1.0) * (math::pi * 2.0);

			wander_target_ = math::vector2d(wander_radius_ * cos(theta),
				wander_radius_ * sin(theta));

			walls_ = {
				wall({0.0f, 0.0f}, {0.0f, 768.0f}),
				wall({0.0f, 768.0f}, {1024.0f, 768.0f}),
				wall({1024.0f, 768.0f}, {1024.0f, 0.0f}),
				wall({1024.0f, 0.0f}, {0.0f, 0.0f})
			};
		}

		math::vector2d steering_behaviour::calculate() {
			steering_force_.x(0);
			steering_force_.y(0);

			steering_force_ += wall_avoidance(walls_) *
					wall_avoidance_weight_;

			steering_force_ += wander() * wander_weight_;

			if (sqrt(steering_force_.x() * steering_force_.x() + steering_force_.y() * steering_force_.y()) > agent_->max_force_)
			{
				math::normalize(steering_force_);

				steering_force_ *= agent_->max_force_;
			}

			return steering_force_;
		}

		math::vector2d steering_behaviour::wander() {
			double JitterThisTimeSlice = wander_jitter_ * agent_->time_elapsed_;

			wander_target_ += math::vector2d(random_clamped() * wander_jitter_,
				random_clamped() * wander_jitter_);

			wander_target_ = math::normalize(wander_target_);

			wander_target_ *= wander_radius_;

			math::vector2d target = wander_target_ + math::vector2d(wander_distance_, 0);
			math::vector2d world_target = point_to_world_space(target, agent_->heading_, agent_->side_, agent_->location());

			return world_target - agent_->location();
		}

		math::vector2d steering_behaviour::wall_avoidance(const std::vector<wall>& walls) {

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
				for (int w = 0; w < walls.size(); ++w) {
					if (line_intersection(agent_->location(), feelers_[flr], walls[w].from, walls[w].to, distToThisIP, point)) {
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
					steering_force = walls[closest_wall].normal * (sqrt(overshoot.x() * overshoot.x() + overshoot.y() * overshoot.y()));
				}
			}

			return steering_force * -1;
		}

		void steering_behaviour::create_feelers() {
			//feeler in front
			feelers_[0] = agent_->location() + feeler_length_ * agent_->heading_;

			//feeler left
			math::vector2d temp = agent_->heading_;
			rotate_vec_around_origin(temp, (math::pi / 2) * 3.5f);
			feelers_[1] = agent_->location() + feeler_length_ / 2.0f * temp;

			//feeler right
			temp = agent_->heading_;
			rotate_vec_around_origin(temp, math::pi * 0.5f);
			feelers_[2] = agent_->location() + feeler_length_ / 2.0f * temp;
		}

		float steering_behaviour::random_clamped() {
			return random_scalar(-1.0, 1.0);
		}

		void steering_behaviour::rotate_vec_around_origin(math::vector2d& vec, double angle) {
			c2d_matrix mat;

			mat.rotate(angle);

			mat.transform_vector2ds(vec);
		}

		math::vector2d steering_behaviour::point_to_world_space(const math::vector2d& point, const math::vector2d& agent_heading, const math::vector2d& agent_side, const math::vector2d& agent_location) {
			math::vector2d trans_point = point;

			c2d_matrix mat_transform;

			mat_transform.rotate(agent_heading, agent_side);
			mat_transform.translate(agent_location.x(), agent_location.y());

			mat_transform.transform_vector2ds(trans_point);

			return trans_point;
		}

		bool steering_behaviour::line_intersection(math::vector2d a, math::vector2d b, math::vector2d c, math::vector2d d, double& dist, math::vector2d& point) {
			double rTop = (a.y() - c.y()) * (d.x() - c.x()) - (a.x() - c.x()) * (d.y() - c.y());
			double rBot = (b.x() - a.x()) * (d.y() - c.y()) - (b.y() - a.y()) * (d.x() - c.x());

			double sTop = (a.y() - c.y()) * (b.x() - a.x()) - (a.x() - c.x()) * (b.y() - a.y());
			double sBot = (b.x() - a.x()) * (d.y() - c.y()) - (b.y() - a.y()) * (d.x() - c.x());

			if ((rBot == 0) || (sBot == 0))
			{
				//lines are parallel
				return false;
			}

			double r = rTop / rBot;
			double s = sTop / sBot;

			if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
			{
				dist = distance(a, b) * r;

				point = a + r * (b - a);

				return true;
			}

			else
			{
				dist = 0;

				return false;
			}
		}

		double steering_behaviour::distance(math::vector2d& a, math::vector2d& b) {
			double distance_y = b.y() - a.y();
			double distance_x = b.x() - a.x();

			return sqrt(distance_y * distance_y + distance_x * distance_x);
		}

	}
}