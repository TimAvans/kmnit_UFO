#pragma once
#include "steering_state.hpp"
#include <kmint\play\actor.hpp>

namespace kmint {
	namespace ufo {
		class seek_steering_state : public steering_state<play::free_roaming_actor> {
		public:
			static seek_steering_state* Instance();
			math::vector2d execute(play::free_roaming_actor* actor) override;
			void enter(play::free_roaming_actor* actor) override;
			void exit(play::free_roaming_actor* actor) override;
		private:
			seek_steering_state() {}
		};
	}
}