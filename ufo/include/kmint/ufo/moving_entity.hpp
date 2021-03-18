#pragma once

#include "kmint/play.hpp"
#include "kmint/math/vector2d.hpp"

namespace kmint {
	namespace ufo {
		class moving_entity : public kmint::play::free_roaming_actor {
		protected:


		
		public:
			double time_elapsed_;

			double max_force_;

			double mass_;
			double max_speed_;
			double max_turnrate_;

			math::vector2d velocity_;
			math::vector2d heading_;
			math::vector2d side_;

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