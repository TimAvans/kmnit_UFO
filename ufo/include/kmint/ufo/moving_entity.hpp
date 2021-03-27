#pragma once

#include "kmint/play.hpp"
#include "kmint/ufo/wall.hpp"
#include "kmint/math/vector2d.hpp"

namespace kmint {
	namespace ufo {
		class moving_entity : public kmint::play::free_roaming_actor {
		protected:

		public:
			std::vector<wall> walls_;

			double time_elapsed_;

			double max_force_;

			double mass_;
			double max_speed_;
			double max_turnrate_;

			math::vector2d velocity_;
			math::vector2d heading_;
			math::vector2d side_;

			double feeler_length_;

			play::actor* target_;

			math::vector2d wander_target_;
			double wander_jitter_;
			double wander_radius_;
			double wander_distance_;

			double        separation_weight_;
			double        cohesion_weight_;
			double        alignment_weight_;
			double        wander_weight_;
			double        wall_avoidance_weight_;
			double        seek_weight_;
			double        flee_weight_;
			double        pursuit_weight_;

			std::vector<moving_entity*> find_neighbours();

			moving_entity(
				math::vector2d velocity,
				double max_speed,
				math::vector2d heading,
				double mass,
				double turn_rate,
				double max_force,
				math::vector2d location
			) : kmint::play::free_roaming_actor(location) {
				velocity_ = velocity;
				max_speed_ = max_speed;
				heading_ = heading;
				side_ = math::perp(heading_);
				mass_ = mass;
				max_turnrate_ = turn_rate;
				max_force_ = max_force;
			}
		};
	}
}