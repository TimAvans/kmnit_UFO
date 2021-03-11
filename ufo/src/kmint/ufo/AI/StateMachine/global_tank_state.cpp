#include "kmint/ufo/global_tank_state.hpp"
#include "kmint/ufo/state_user.hpp"
#include "kmint/ufo/wander_state.hpp"

namespace kmint {
	namespace ufo {
		global_tank_state* global_tank_state::Instance()
		{
			static global_tank_state instance;
			return &instance;
		}

		void global_tank_state::execute(play::map_bound_actor* actor)
		{
			auto x = dynamic_cast<state_user*>(actor);

			//x->get_state_machine()->ChangeState(wander_state::Instance());
		}

		void global_tank_state::enter(play::map_bound_actor* actor) { }

		void global_tank_state::exit(play::map_bound_actor* actor) { }
	}
}