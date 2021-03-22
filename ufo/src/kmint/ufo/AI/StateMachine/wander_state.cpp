#include "kmint/ufo/wander_state.hpp"

#include "kmint/play.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/state_user.hpp>

namespace kmint {
	namespace ufo {

		wander_state* wander_state::Instance()
		{
			static wander_state instance;
			return &instance;
		}

		void wander_state::enter(play::map_bound_actor* actor) {

		}

		void wander_state::execute(play::map_bound_actor* actor)
		{
			auto x = dynamic_cast<state_user*>(actor);

			//heeft ufo gezien? verander state
			//changestate(probablistic_state::instance())

			//Wander
			x->random_move();
		}

		void wander_state::exit(play::map_bound_actor* actor) {}
	}
}



