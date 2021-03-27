#pragma once 

#include <kmint/math/vector2d.hpp>

namespace kmint {
	namespace ufo {

		class geometrics {
		public: 
			float length(math::vector2d vector);

			void rotate_vec_around_origin(math::vector2d& vec, double angle);
			double distance(math::vector2d& a, math::vector2d& b);
			bool line_intersection(math::vector2d a, math::vector2d b, math::vector2d c, math::vector2d d, double& dist, math::vector2d& point);
			math::vector2d point_to_world_space(const math::vector2d& point, const math::vector2d& agent_heading, const math::vector2d& agent_side, const math::vector2d& agent_location);
		};

	}
}