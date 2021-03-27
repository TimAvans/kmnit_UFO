#include "kmint/ufo/global_saucer_state.hpp"
#include "kmint/ufo/pursuit_steering_state.hpp"
#include "kmint/ufo/seek_steering_state.hpp"

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

		global_saucer_state* global_saucer_state::Instance()
		{
			static global_saucer_state instance;
			return &instance;
		}

		void global_saucer_state::enter(play::free_roaming_actor* actor) {
			
		}

		math::vector2d global_saucer_state::execute(play::free_roaming_actor* actor)
		{

			math::vector2d v;

			if (auto x = dynamic_cast<saucer*>(actor)) {
			for (auto i = x->begin_perceived(); i != x->end_perceived(); ++i) {
				auto& a = *i;
				if (x->target_ == nullptr) {
					if (auto h = dynamic_cast<human*>(&a)) {
						if (!h->targeted) {
							x->target_ = h;
							h->targeted = true;
							h->tag_as_target();
							x->get_state_machine()->ChangeState(pursuit_steering_state::Instance());
						}
					}
				}
				if (auto t = dynamic_cast<tank*>(&a)) {
					if (t->damage_ < 100) {
						if (!dynamic_cast<tank*>(x->target_)) {

							if (auto h = dynamic_cast<human*>(x->target_)) {
								h->targeted = false;
								h->clear_tag();
							}

							x->target_ = t;
							x->get_state_machine()->ChangeState(seek_steering_state::Instance());
						}
					}
				}
			}
				v = x->steering_->wall_avoidance();
			}
			return v;
		}

		void global_saucer_state::exit(play::free_roaming_actor* actor) {
			
		}
	}
}



