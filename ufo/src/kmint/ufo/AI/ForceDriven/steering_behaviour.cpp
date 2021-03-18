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
		}

		math::vector2d steering_behaviour::calculate() {
			steering_force_.x(0);
			steering_force_.y(0);

			//if (On(wall_avoidance))
			//{
			//	m_vSteeringForce += WallAvoidance(m_pVehicle->World()->Walls()) *
			//		m_dWeightWallAvoidance;
			//}

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

		float steering_behaviour::random_clamped() {
			return random_scalar(-1.0, 1.0);
		}

		math::vector2d steering_behaviour::point_to_world_space(const math::vector2d& point, const math::vector2d& agent_heading, const math::vector2d& agent_side, const math::vector2d& agent_location) {
			math::vector2d trans_point = point;

			c2d_matrix mat_transform;

			mat_transform.rotate(agent_heading, agent_side);
			mat_transform.translate(agent_location.x(), agent_location.y());

			mat_transform.transform_vector2ds(trans_point);

			return trans_point;
		}

	}
}