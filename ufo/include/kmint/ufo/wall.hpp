#pragma once

#include <kmint/math/vector2d.hpp>

namespace kmint {
	namespace ufo {
		class wall {
		public:
			math::vector2d from;
			math::vector2d to;
			math::vector2d normal;

			wall(math::vector2d from, math::vector2d to) : from(from), to(to) {
				math::vector2d temp = math::normalize(to - from);

				normal.x(-temp.y());
				normal.y(temp.x());
			}
		};
	}
}
