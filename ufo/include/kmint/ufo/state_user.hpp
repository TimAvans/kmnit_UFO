#pragma once
#include "kmint/play.hpp"
#include <tuple>
#include <vector>
#include "kmint/ufo/probablistic_state_machine.hpp"

namespace kmint {
	namespace ufo {

		class state_user : public play::map_bound_actor {
		public:
			state_user(map::map_graph& g, map::map_node& initial_node, graphics::image image);
			ui::drawable const& drawable() const override { return drawable_; }

			std::vector<int> path_;

			void move_over_path();
			void random_move();

			map::map_graph* get_graph();
			probablistic_state_machine<play::map_bound_actor>* get_state_machine();
			bool incorporeal() const override { return false; }

			void collide_with_human();
			void change_color(graphics::color_component r, graphics::color_component g, graphics::color_component b);
			void clear_color();
			play::image_drawable drawable_;

		private:
			map::map_graph* graph_;
		protected:
			probablistic_state_machine<play::map_bound_actor>* state_machine_;
		};
	}
}
