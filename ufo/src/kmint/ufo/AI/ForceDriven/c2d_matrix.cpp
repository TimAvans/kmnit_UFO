#include "kmint\ufo\c2d_matrix.hpp"
namespace kmint {
	namespace ufo {
		void c2d_matrix::Identity()
		{
			matrix_._11 = 1; matrix_._12 = 0; matrix_._13 = 0;

			matrix_._21 = 0; matrix_._22 = 1; matrix_._23 = 0;

			matrix_._31 = 0; matrix_._32 = 0; matrix_._33 = 1;
		}

		void c2d_matrix::rotate(const math::vector2d& fwd, const math::vector2d& side)
		{
			matrix m;

			m._11 = fwd.x();  m._12 = fwd.y(); m._13 = 0;

			m._21 = side.x(); m._22 = side.y(); m._23 = 0;

			m._31 = 0; m._32 = 0; m._33 = 1;

			matrix_multiply(m);
		}

		void c2d_matrix::rotate(double rotation) {
			matrix mat;

			double Sin = sin(rotation);
			double Cos = cos(rotation);

			mat._11 = Cos;  mat._12 = Sin; mat._13 = 0;

			mat._21 = -Sin; mat._22 = Cos; mat._23 = 0;

			mat._31 = 0; mat._32 = 0; mat._33 = 1;

			matrix_multiply(mat);
		}

		void c2d_matrix::translate(double x, double y)
		{
			matrix m;

			m._11 = 1; m._12 = 0; m._13 = 0;
			m._21 = 0; m._22 = 1; m._23 = 0;
			m._31 = x; m._32 = y; m._33 = 1;

			matrix_multiply(m);
		}

		void c2d_matrix::matrix_multiply(matrix& m) {
			matrix mat_temp;

			//first row
			mat_temp._11 = (matrix_._11 * m._11) + (matrix_._12 * m._21) + (matrix_._13 * m._31);
			mat_temp._12 = (matrix_._11 * m._12) + (matrix_._12 * m._22) + (matrix_._13 * m._32);
			mat_temp._13 = (matrix_._11 * m._13) + (matrix_._12 * m._23) + (matrix_._13 * m._33);

			//second
			mat_temp._21 = (matrix_._21 * m._11) + (matrix_._22 * m._21) + (matrix_._23 * m._31);
			mat_temp._22 = (matrix_._21 * m._12) + (matrix_._22 * m._22) + (matrix_._23 * m._32);
			mat_temp._23 = (matrix_._21 * m._13) + (matrix_._22 * m._23) + (matrix_._23 * m._33);

			//third
			mat_temp._31 = (matrix_._31 * m._11) + (matrix_._32 * m._21) + (matrix_._33 * m._31);
			mat_temp._32 = (matrix_._31 * m._12) + (matrix_._32 * m._22) + (matrix_._33 * m._32);
			mat_temp._33 = (matrix_._31 * m._13) + (matrix_._32 * m._23) + (matrix_._33 * m._33);

			matrix_ = mat_temp;
		}

		void c2d_matrix::transform_vector2ds(math::vector2d& point)
		{
			double tempx = (matrix_._11 * point.x()) + (matrix_._21 * point.y()) + matrix_._31;
			double tempy = (matrix_._12 * point.x()) + (matrix_._22 * point.y()) + matrix_._32;

			point.x(tempx);
			point.y(tempy);
		}
	}
}
