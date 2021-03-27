#include "kmint/ufo/geometrics.hpp"
#include <kmint/ufo/c2d_matrix.hpp>
namespace kmint {
	namespace ufo {

		void geometrics::rotate_vec_around_origin(math::vector2d& vec, double angle) {
			c2d_matrix mat;

			mat.rotate(angle);

			mat.transform_vector2ds(vec);
		}

		math::vector2d geometrics::point_to_world_space(const math::vector2d& point, const math::vector2d& agent_heading, const math::vector2d& agent_side, const math::vector2d& agent_location) {

			c2d_matrix transformed;
			kmint::math::vector2d new_point = point;
			transformed.rotate(agent_heading, math::perp(agent_heading));
			transformed.translate(agent_location.x(), agent_location.y());
			transformed.transform_vector2ds(new_point);
			return new_point;
		}

		bool geometrics::line_intersection(math::vector2d a, math::vector2d b, math::vector2d c, math::vector2d d, double& dist, math::vector2d& point) {
			double rTop = (a.y() - c.y()) * (d.x() - c.x()) - (a.x() - c.x()) * (d.y() - c.y());
			double rBot = (b.x() - a.x()) * (d.y() - c.y()) - (b.y() - a.y()) * (d.x() - c.x());

			double sTop = (a.y() - c.y()) * (b.x() - a.x()) - (a.x() - c.x()) * (b.y() - a.y());
			double sBot = (b.x() - a.x()) * (d.y() - c.y()) - (b.y() - a.y()) * (d.x() - c.x());

			if ((rBot == 0) || (sBot == 0))
			{
				//lines are parallel
				return false;
			}

			double r = rTop / rBot;
			double s = sTop / sBot;

			if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
			{
				dist = distance(a, b) * r;

				point = a + r * (b - a);

				return true;
			}

			else
			{
				dist = 0;

				return false;
			}
		}

		double geometrics::distance(math::vector2d& a, math::vector2d& b) {
			double distance_y = b.y() - a.y();
			double distance_x = b.x() - a.x();

			return sqrt(distance_y * distance_y + distance_x * distance_x);
		}
	}
}