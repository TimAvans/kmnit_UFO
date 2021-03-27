#include "kmint/ufo/seek_steering_state.hpp"
#include "kmint/ufo/wander_steering_state.hpp"

#include "kmint/play.hpp"
#include <kmint/random.hpp>
#include <kmint/ufo/state_user.hpp>
#include <kmint/ufo/tank.hpp>
#include <kmint/ufo/saucer.hpp>

namespace kmint {
	namespace ufo {

		seek_steering_state* seek_steering_state::Instance()
		{
			static seek_steering_state instance;
			return &instance;
		}

		void seek_steering_state::enter(play::free_roaming_actor* actor) {
			if (auto x = dynamic_cast<saucer*>(actor)) {
				x->change_color(0, 255, 0);
			}
		}

		math::vector2d seek_steering_state::execute(play::free_roaming_actor* actor)
		{
			math::vector2d v;


			if (auto x = dynamic_cast<saucer*>(actor)) {
				if (x->target_ != nullptr) {
					v = x->steering_->seek(x->target_->location());
				}
				else {
					v = x->heading_;
				}
			}

			if (auto s = dynamic_cast<saucer*>(actor)) {
				for (int i = 0; i < actor->num_colliding_actors(); ++i) {
					auto& a = s->colliding_actor(i);
					if (auto t = dynamic_cast<tank*>(&a)) {
						if (t == s->target_) {
							t->take_damage();
							if (t->damage_ >= 100) {
								s->target_ = nullptr;
								s->get_state_machine()->ChangeState(wander_steering_state::Instance());
							}
						}
					}
				}
			}
			
			return v;
		}

		void seek_steering_state::exit(play::free_roaming_actor* actor) {
			if (auto x = dynamic_cast<saucer*>(actor)) {
				x->clear_color();
			}
		}
	}
}



