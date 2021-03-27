#include "kmint/ufo/andre.hpp"
#include "kmint/graphics.hpp"
#include "kmint/ufo/node_algorithm.hpp"
#include "kmint/random.hpp"
#include <iostream>
#include <kmint/ufo/astar.hpp>

namespace kmint::ufo {

namespace {

graphics::image andre_image() {
  constexpr scalar scale = 1.0;
  return graphics::image{"resources/andre.png"};
}

}
andre::andre(map::map_graph& g, map::map_node& initial_node)
	: play::map_bound_actor{ initial_node }, drawable_{ *this,
													 graphics::image{
														 andre_image()}}, graph_{ &g } {}



void andre::set_path(std::vector<int> path) {
	path_ = path;
	initial_path_ = path;
}

void andre::act(delta_time dt) {
	t_since_move_ += dt;
	if (to_seconds(t_since_move_) >= waiting_time(node())) {
		astar a{ *graph_ };

		if (node().node_info().kind == current_target_) {
			if (current_target_ == '4') {
				current_target_ = '0';
			}
			current_target_++;
		}

		if (path_.size() <= 0) {

			auto result = a.search(this->node().node_id(), find_node_of_kind(*graph_, current_target_).node_id());

			a.clear_path_color(initial_path_, visited_);

			set_path(result[0]);
			visited_ = result[1];

			a.show_shortest_path(path_, visited_);
		}
		else {
			for (std::size_t i = 0; i < node().num_edges(); ++i) {
				if (node()[i].to().node_id() == path_.back()) {
					node(node()[i].to());
					path_.pop_back();
					break;
				}
			}
			t_since_move_ = from_seconds(0);
		}

	}
}

}
