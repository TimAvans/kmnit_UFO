#pragma once

#include "kmint/play.hpp"

namespace kmint {
	namespace ufo {
		class door : public play::static_actor {
		public:
			door(map::map_graph& g, math::vector2d location);
			ui::drawable const& drawable() const override { return drawable_; }
			bool incorporeal() const override { return false; }
			scalar collision_range() const override { return 16.0; }
			bool perceptive() const override { return true; }
			scalar perception_range() const override { return 200.f; }

		private:
			play::image_drawable drawable_;
			delta_time t_since_move_{};
		};

	} 
} 
