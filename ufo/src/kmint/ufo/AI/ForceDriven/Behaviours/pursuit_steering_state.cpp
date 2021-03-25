#include "kmint/ufo/pursuit_steering_state.hpp"
#include "kmint/ufo/wander_steering_state.hpp"

#include "kmint/play.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/state_user.hpp>
#include <kmint/ufo/saucer.hpp>
#include <kmint/ufo/tank.hpp>
#include <kmint/ufo/node_algorithm.hpp>
#include <kmint/ufo/astar.hpp>
#include <kmint/ufo/human.hpp>

namespace kmint {
	namespace ufo {

		pursuit_steering_state* pursuit_steering_state::Instance()
		{
			static pursuit_steering_state instance;
			return &instance;
		}

		void pursuit_steering_state::enter(play::free_roaming_actor* actor) {

		}

		math::vector2d pursuit_steering_state::execute(play::free_roaming_actor* actor)
		{
			math::vector2d v;

			if (auto x = dynamic_cast<saucer*>(actor)) {
				v = x->steering_->pursuit(x->target_);
			}

			if (auto s = dynamic_cast<saucer*>(actor)) {
				for (int i = 0; i < actor->num_colliding_actors(); ++i) {
					auto& a = s->colliding_actor(i);
					if (auto h = dynamic_cast<human*>(&a)) {
						if (h == s->target_) {
							h->remove();
							s->target_ = nullptr;
							s->get_state_machine()->ChangeState(wander_steering_state::Instance());
						}
					}
				}
			}

			return v;
		}

		void pursuit_steering_state::exit(play::free_roaming_actor* actor) {

		}
	}
}



