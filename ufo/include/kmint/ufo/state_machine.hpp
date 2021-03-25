#pragma once
#include "steering_state.hpp"

namespace kmint {
	namespace ufo {
		template <class entity_type>
		class state_machine
		{
		private:

			entity_type* owner_;
			steering_state<entity_type>* current_state_;
			steering_state<entity_type>* previous_state_;
			steering_state<entity_type>* global_state_;

		public:
			state_machine(entity_type* owner) :owner_(owner), current_state_(NULL), global_state_(NULL) {}

			void SetCurrentState(steering_state<entity_type>* s)
			{
				current_state_ = s;
			}

			void SetGlobalState(steering_state<entity_type>* g)
			{
				global_state_ = g;
			}

			math::vector2d Update() const
			{
				math::vector2d v;


				if (global_state_)
				{
					v += global_state_->execute(owner_);
				}

				if (current_state_)
				{
					v += current_state_->execute(owner_);
				}

				if (auto x = dynamic_cast<saucer*>(owner_)) {
					if (sqrt(v.x() * v.x() + v.y() * v.y()) > x->max_force_)
					{
						math::normalize(v);

						v *= x->max_force_;
					}
				}

				return v;
			}

			void ChangeState(steering_state<entity_type>* new_state)
			{
				if (new_state != current_state_) {
					previous_state_ = current_state_;

					current_state_->exit(owner_);
					current_state_ = new_state;
					current_state_->enter(owner_);
				}
			}

			steering_state<entity_type>* CurrentState() const
			{
				return current_state_;
			}

			steering_state<entity_type>* GlobalState() const
			{
				return global_state_;
			}

			void RevertToPreviousState() {
				ChangeState(previous_state_);
			}

		};
	}
}