#include "kmint/ufo/state_user.hpp"
#include <kmint/random.hpp>
#include "kmint/ufo/human.hpp"
#include "kmint/ufo/state_machine.hpp"
#include "kmint/ufo/wander_state.hpp"
#include "kmint/ufo/flee_state.hpp"
#include <kmint/ufo/astar.hpp>

namespace kmint {
	namespace ufo {
		state_user::state_user(map::map_graph& g, map::map_node& initial_node, graphics::image image) : play::map_bound_actor{ initial_node },
			drawable_{ *this, image }, graph_{ &g }{

			state_machine_ = new probablistic_state_machine<play::map_bound_actor>(this);
			state_machine_->SetCurrentState(wander_state::Instance()); }

		void state_user::move_over_path()
		{
			if (path_.size() > 0)
			{
				for (std::size_t i = 0; i < node().num_edges(); ++i)
				{
					if (node()[i].to().node_id() == path_.back())
					{
						node(node()[i].to());
						path_.pop_back();
						break;
					}
				}
			}
			else {
				astar a(*get_graph());
				//a.clear_path_color(initial_path_, visited_);
			}
		}

		void state_user::random_move()
		{
			int next_index = random_int(0, node().num_edges());

			while(node()[next_index].weight() > 4){
				next_index = random_int(0, node().num_edges());
			}
			node(node()[next_index].to());
		}

		probablistic_state_machine<play::map_bound_actor>* state_user::get_state_machine() {
			return state_machine_;
		}

		map::map_graph* state_user::get_graph() {
			return graph_;
		}

		void state_user::set_path(std::vector<int> path) {
			path_ = path;
			initial_path_ = path;
		}

		void state_user::collide_with_human() {
			for (int i = 0; i < num_colliding_actors(); ++i) {
				auto& a = colliding_actor(i);
				if (auto h = dynamic_cast<human*>(&a)) {
					h->remove_human();
				}
			}
		}

		void state_user::change_color(graphics::color_component r, graphics::color_component g, graphics::color_component b) {
			graphics::color color{ r,g,b };
			drawable_.set_tint(color);
		}

		void state_user::clear_color() {
			drawable_.remove_tint();
		}
	}
}