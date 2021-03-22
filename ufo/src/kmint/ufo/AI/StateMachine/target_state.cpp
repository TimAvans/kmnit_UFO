#include "kmint/ufo/target_state.hpp"

#include "kmint/play.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/state_user.hpp>
#include <kmint/ufo/tank.hpp>

namespace kmint {
	namespace ufo {

		target_state* target_state::Instance()
		{
			static target_state instance;
			return &instance;
		}

		void target_state::enter(play::map_bound_actor* actor) {
			if (auto a = dynamic_cast<tank*>(actor)) {
				if (a->type_ == tank_type::red) {
					std::cout << "red : Entered Target state" << std::endl;
				}
				else {
					std::cout << "green : Entered Target state" << std::endl;

				}

				a->change_color(255, 0, 0);
				a->current_target_object_ = a->find_closest_target(current_target_type_);
			}
		}

		void target_state::execute(play::map_bound_actor* actor)
		{
			if (auto a = dynamic_cast<tank*>(actor)) {
				if (a->path_.size() == 0) {
					a->pick_up_item(*a->current_target_object_, current_target_type_);
					a->get_state_machine()->ChangeState(wander_state::Instance());
				}
				a->move_over_path();
			}
		}

		void target_state::exit(play::map_bound_actor* actor) {
			auto x = dynamic_cast<state_user*>(actor);
			x->clear_color();
		}
	}
}



