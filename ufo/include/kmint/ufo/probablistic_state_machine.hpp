#pragma once
#include "state.hpp"
#include <map>
#include <random>

#include "target_state.hpp"
#include "flee_state.hpp"
#include "wander_state.hpp"

namespace kmint {
	namespace ufo {
		enum class actions {
			EMP,
			SHIELD,
			FLEE
		};

		template <class entity_type>
		class probablistic_state_machine
		{
		private:
			entity_type* owner_;
			state<entity_type>* current_state_;
			state<entity_type>* previous_state_;
			state<entity_type>* global_state_;

			//float[3] = chance, lowerbound, upperbound
			std::map<actions, std::vector<float>> chances;

		public:
			probablistic_state_machine(entity_type* owner) :owner_(owner), current_state_(NULL), global_state_(NULL) {
				std::vector<float> test = { 1.0f / 3.0f, 0.0f, 0.0f };

				chances = {
					{actions::EMP, {1.0f / 3.0f, 0.0f, 0.0f}},
					{actions::SHIELD, {1.0f / 3.0f, 0.0f, 0.0f}},
					{actions::FLEE, {1.0f / 3.0f, 0.0f, 0.0f}}
				};
			}

			void SetCurrentState(state<entity_type>* s)
			{
				current_state_ = s;
			}

			void SetGlobalState(state<entity_type>* g)
			{
				global_state_ = g;
			}

			void Update() const
			{
				if (global_state_)
				{
					global_state_->execute(owner_);
				}

				if (current_state_)
				{
					current_state_->execute(owner_);
				}
			}

			void ChangeState(state<entity_type>* new_state)
			{
				if (new_state != current_state_) {
					previous_state_ = current_state_;

					current_state_->exit(owner_);
					current_state_ = new_state;
					current_state_->enter(owner_);
				}
			}

			void ChangeState() {
				if (dynamic_cast<wander_state*>(current_state_)) {
					float random_chance = kmint::random_int(0.0, 100.0) / 100.0f;

					float accumulated = 0;
					for (std::map<actions, std::vector<float>>::iterator it = chances.begin(); it != chances.end(); ++it) {
						it->second[1] = accumulated;
						it->second[2] = it->second[1] + it->second[0];
						accumulated = it->second[2];
						
						if (random_chance > it->second[1] && random_chance < it->second[2]) {
							if (it->first == actions::EMP) {
								auto new_state = target_state::Instance();
								new_state->current_target_type_ = "EMP";
								ChangeState(target_state::Instance());
							}
							else if (it->first == actions::SHIELD) {
								auto new_state = target_state::Instance();
								new_state->current_target_type_ = "SHIELD";
								ChangeState(target_state::Instance());
							}
							else {
								ChangeState(flee_state::Instance());
							}
							break;
						}
					}
				}
			}

			state<entity_type>* CurrentState() const
			{
				return current_state_;
			}

			state<entity_type>* GlobalState() const
			{
				return global_state_;
			}

			void RevertToPreviousState() {
				ChangeState(previous_state_);
			}

		};
	}
}