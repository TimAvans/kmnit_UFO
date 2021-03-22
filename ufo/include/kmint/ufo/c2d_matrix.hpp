#pragma once
#include "kmint/math/vector2d.hpp"

namespace kmint {
	namespace ufo {

		class c2d_matrix {
		private:
			struct matrix {
				double _11, _12, _13;
				double _21, _22, _23;
				double _31, _32, _33;

				matrix() {
					_11 = 0.0; _12 = 0.0; _13 = 0.0;
					_21 = 0.0; _22 = 0.0; _23 = 0.0;
					_31 = 0.0; _32 = 0.0; _33 = 0.0;
				}
			};

			matrix matrix_;
		public:
			c2d_matrix() {
				Identity();
			}

			void Identity();

			void rotate(const math::vector2d& fwd, const math::vector2d& side);

			void rotate(double rotation);
			
			void translate(double x, double y);

			void transform_vector2ds(math::vector2d& point);

			void matrix_multiply(matrix& m);
		};
	}
}