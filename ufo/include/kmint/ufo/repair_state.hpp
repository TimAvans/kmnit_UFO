#pragma once
#include "state.hpp"
#include <kmint\play\actor.hpp>

namespace kmint {
	namespace ufo {
		class repair_state : public state<play::map_bound_actor> {
		public:
			static repair_state* Instance();
			void execute(play::map_bound_actor* actor) override;
			void enter(play::map_bound_actor* actor) override;
			void exit(play::map_bound_actor* actor) override;

		private:
			repair_state() {}
		};
	}
}