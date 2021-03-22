#include "kmint/ufo/repair_state.hpp"

#include "kmint/play.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/state_user.hpp>
#include <kmint/ufo/tank.hpp>
#include <kmint/ufo/astar.hpp>
#include <kmint/ufo/node_algorithm.hpp>

namespace kmint {
	namespace ufo {

		repair_state* repair_state::Instance()
		{
			static repair_state instance;
			return &instance;
		}

		void repair_state::enter(play::map_bound_actor* actor) {
			if (auto x = dynamic_cast<tank*>(actor)) {
				astar a(*x->get_graph());
				x->current_target_ = x->andre_->current_target_;

				auto result = a.search(x->node().node_id(), find_node_of_kind(*x->get_graph(), x->andre_->current_target_).node_id());
				a.clear_path_color(x->path_, x->visited_);


				x->set_path(result[0]);
				x->visited_ = result[1];

				a.show_shortest_path(x->path_, x->visited_);

				x->change_color(0, 255, 0);
			}
		}

		void repair_state::execute(play::map_bound_actor* actor)
		{
			if (auto x = dynamic_cast<tank*>(actor)) {
				x->move_over_path();


				for (int i = 0; i < x->num_colliding_actors(); ++i) {
					auto& a = x->colliding_actor(i);
					if (auto p = dynamic_cast<andre*>(&a)) {
						x->damage_ = 0;
						x->get_state_machine()->ChangeState(wander_state::Instance());
					}
				}

				if (x->path_.size() == 0) {
					x->current_target_--;
					if (x->current_target_ == '0') {
						x->current_target_ = '4';
					}
					astar a(*x->get_graph());

					auto result = a.search(x->node().node_id(), find_node_of_kind(*x->get_graph(), x->current_target_).node_id());

					a.clear_path_color(x->path_, x->visited_);


					x->set_path(result[0]);
					x->visited_ = result[1];

					a.show_shortest_path(x->path_, x->visited_);
				}
			}
		}

		void repair_state::exit(play::map_bound_actor* actor) {
			auto x = dynamic_cast<state_user*>(actor);

			x->clear_color();
		}
	}
}



