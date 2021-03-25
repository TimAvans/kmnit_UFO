#include "kmint/ufo/wander_steering_state.hpp"

#include "kmint/play.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/state_user.hpp>
#include <kmint/ufo/saucer.hpp>
#include <kmint/ufo/tank.hpp>
#include <kmint/ufo/node_algorithm.hpp>
#include <kmint/ufo/astar.hpp>

namespace kmint {
	namespace ufo {

		wander_steering_state* wander_steering_state::Instance()
		{
			static wander_steering_state instance;
			return &instance;
		}

		void wander_steering_state::enter(play::free_roaming_actor* actor) {

		}

		math::vector2d wander_steering_state::execute(play::free_roaming_actor* actor)
		{
			math::vector2d v;

			if (auto x = dynamic_cast<saucer*>(actor)) {
				v = x->steering_->wander();
			}

			return v;
		}

		void wander_steering_state::exit(play::free_roaming_actor* actor) {

		}
	}
}



