#pragma once

#include "kmint/play.hpp"

namespace kmint {
	namespace ufo {
		class shield : public play::map_bound_actor {
		public:
			shield(map::map_graph& g, map::map_node& initial_node);
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
