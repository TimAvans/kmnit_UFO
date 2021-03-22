#pragma once
#include "state.hpp"
#include <kmint\play\actor.hpp>

namespace kmint {
	namespace ufo {
		class target_state : public state<play::map_bound_actor> {
		public:
			static target_state* Instance();
			void execute(play::map_bound_actor* actor) override;
			void enter(play::map_bound_actor* actor) override;
			void exit(play::map_bound_actor* actor) override;
			
			std::string current_target_type_;
		private:
			target_state() {}
		};
	}
}