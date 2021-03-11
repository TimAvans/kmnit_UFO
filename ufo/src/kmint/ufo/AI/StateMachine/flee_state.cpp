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
			if (auto a = dynamic_cast<tank*>(actor)) {

				if (a->type_ == tank_type::red) {
					std::cout << "red : Entered flee state" << std::endl;
				}
				else {
					std::cout << "green : Entered flee state" << std::endl;
				}
			}

			if (auto t = dynamic_cast<tank*>(actor)) {
					for (auto i = t->begin_perceived(); i != t->end_perceived(); ++i) {
						auto& a = *i;
						if (dynamic_cast<saucer*>(&a)) {
							math::vector2d diff = {
								(t->location().x() - a.location().x()) * -1,
								(t->location().y() - a.location().y()) * -1,
							};

							math::vector2d flee_location = {
								t->location().x() - diff.x(),
								t->location().y() - diff.y()
							};

							astar a{ *t->get_graph() };

							t->path_ = a.search(t->node().node_id(), find_closest_node_to(*t->get_graph(), flee_location).node_id());

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

		}
	}
}



