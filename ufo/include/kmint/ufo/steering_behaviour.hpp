#pragma once
#include <kmint/ufo/moving_entity.hpp>
#include <kmint/ufo/wall.hpp>

namespace kmint {
	namespace ufo {
		class steering_behaviour {
		private:
			moving_entity *agent_;

			math::vector2d steering_force_;

			std::vector<math::vector2d> feelers_;
			double feeler_length_ = 30;

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
			double        wall_avoidance_weight_ = 5000;
			double        seek_weight_ = 1;
			double        flee_weight_;
			double        pursuit_weight_ = 1;
			double        offset_pursuit_weight_;

			std::vector<wall> walls_;

			void create_feelers();

			math::vector2d flee(math::vector2d target_pos);

			//this returns a steering force which will keep the agent away from any
			//walls it may encounter

			float random_clamped();
			float length(math::vector2d vector);

			void rotate_vec_around_origin(math::vector2d& vec, double angle);
			double distance(math::vector2d& a, math::vector2d& b);
			bool line_intersection(math::vector2d a, math::vector2d b, math::vector2d c, math::vector2d d, double& dist, math::vector2d& point);
			math::vector2d point_to_world_space(const math::vector2d& point, const math::vector2d& agent_heading, const math::vector2d& agent_side, const math::vector2d& agent_location);
		public:
			steering_behaviour(moving_entity* agent, double jitter, double radius, double distance);

			math::vector2d wall_avoidance();
			math::vector2d wander();
			math::vector2d pursuit(play::actor* evader);
			math::vector2d seek(math::vector2d target_pos);

			math::vector2d calculate();
		};
	}
}