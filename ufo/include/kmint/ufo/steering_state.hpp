#pragma once
#include "kmint/play.hpp"

namespace kmint {
	namespace ufo {
		template <class entity_type>
		class steering_state
		{
		public:
			virtual ~steering_state() {}
			virtual void enter(entity_type* actor) = 0;
			virtual math::vector2d execute(entity_type* actor) = 0;
			virtual void exit(entity_type* actor) = 0;

		};
	}
}