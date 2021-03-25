#pragma once
#include "kmint/play.hpp"

namespace kmint {
	namespace ufo {
		template <class entity_type>
		class steering_state
		{
		public:
			virtual ~steering_state() {}

			//this will execute when the state is entered
			virtual void enter(entity_type* actor) = 0;

			//this is called by the actor's act function each update step
			virtual math::vector2d execute(entity_type* actor) = 0;

			//this will execute when the state is exited
			virtual void exit(entity_type* actor) = 0;

		};
	}
}