#pragma once
#include <kmint/ufo/moving_entity.hpp>
#include <kmint/ufo/wall.hpp>

namespace kmint {
	namespace ufo {
		class steering_behaviour {
		private:
			moving_entity* agent_;

			math::vector2d steering_force_;

			std::vector<math::vector2d> feelers_;
			double feeler_length_;

			//wander
			math::vector2d wander_target_;
			double wander_jitter_;
			double wander_radius_;
			double wander_distance_;

			//multipliers. These can be adjusted to effect strength of the  
			//appropriate behavior. Useful to get flocking the way you require
			//for example.
			double        separation_weight_;
			double        cohesion_weight_;
			double        alignment_weight_;
			double        wander_weight_ = 1;
			double        obstacle_avoidance_weight_;
			double        wall_avoidance_weight_;
			double        seek_weight_;
			double        flee_weight_;
			double        pursuit_weight_;
			double        offset_pursuit_weight_;



			void create_feelers();

			math::vector2d seek(math::vector2d target_pos);
			math::vector2d flee(math::vector2d target_pos);
			math::vector2d wander();

			//this returns a steering force which will keep the agent away from any
			//walls it may encounter
			math::vector2d WallAvoidance(const std::vector<wall>& walls);

			float random_clamped();
			float length(math::vector2d vector);

			math::vector2d point_to_world_space(const math::vector2d& point, const math::vector2d& agent_heading, const math::vector2d& agent_side, const math::vector2d& agent_location);
		public:
			steering_behaviour(moving_entity* agent, double jitter, double radius, double distance);



			math::vector2d calculate();
		};
	}
}