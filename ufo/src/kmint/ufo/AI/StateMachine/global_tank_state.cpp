#include "kmint/ufo/global_tank_state.hpp"
#include "kmint/ufo/state_user.hpp"
#include "kmint/ufo/wander_state.hpp"
#include "kmint/ufo/repair_state.hpp"
#include <kmint/ufo/saucer.hpp>
#include <kmint/ufo/tank.hpp>
#include <kmint/ufo/human.hpp>

namespace kmint {
	namespace ufo {
		global_tank_state* global_tank_state::Instance()
		{
			static global_tank_state instance;
			return &instance;
		}

		void global_tank_state::execute(play::map_bound_actor* actor)
		{
			auto x = dynamic_cast<tank*>(actor);

			for (int i = 0; i < x->num_colliding_actors(); ++i) {
				auto& a = x->colliding_actor(i);
				if (auto h = dynamic_cast<human*>(&a)) {
					h->remove();
				}
			}

			if (x->damage_ >= 100) {
				x->get_state_machine()->ChangeState(repair_state::Instance());
			}
		}

		void global_tank_state::enter(play::map_bound_actor* actor) { }

		void global_tank_state::exit(play::map_bound_actor* actor) { }
	}
}