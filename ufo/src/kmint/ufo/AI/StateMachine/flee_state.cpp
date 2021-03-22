#include "kmint/ufo/flee_state.hpp"

#include "kmint/play.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/state_user.hpp>
#include <kmint/ufo/saucer.hpp>
#include <kmint/ufo/tank.hpp>
#include <kmint/ufo/node_algorithm.hpp>
#include <kmint/ufo/astar.hpp>

namespace kmint {
	namespace ufo {

		flee_state* flee_state::Instance()
		{
			static flee_state instance;
			return &instance;
		}

		void flee_state::enter(play::map_bound_actor* actor) {
			if (auto t = dynamic_cast<tank*>(actor)) {
				begin_damage_ = t->damage_;

				t->change_color(0, 0, 255);

				for (auto i = t->begin_perceived(); i != t->end_perceived(); ++i) {
					auto& u = *i;
					if (dynamic_cast<saucer*>(&u)) {
						math::vector2d diff = {
							(t->location().x() - u.location().x()) * -1,
							(t->location().y() - u.location().y()) * -1,
						};

						math::vector2d flee_location = {
							t->location().x() - diff.x(),
							t->location().y() - diff.y()
						};

						astar a{ *t->get_graph() };

						auto result = a.search(t->node().node_id(), find_closest_node_to(*t->get_graph(), flee_location).node_id());
						a.clear_path_color(t->path_, t->visited_);
						
						t->set_path(result[0]);
						t->visited_ = result[1];

						a.show_shortest_path(t->path_, t->visited_);

						break;
					}
				}
			}
		}

		void flee_state::execute(play::map_bound_actor* actor)
		{
			if (auto a = dynamic_cast<tank*>(actor)) {
				a->move_over_path();

				if (a->path_.size() == 0) {
					a->get_state_machine()->ChangeState(wander_state::Instance());
				}
			}
		}

		void flee_state::exit(play::map_bound_actor* actor) {
			if (auto a = dynamic_cast<tank*>(actor)) {
				if (a->damage_ > begin_damage_) {
					a->get_state_machine()->change_chances(actions::FLEE, 0);
				}
				a->clear_color();
			}
		}
	}
}



