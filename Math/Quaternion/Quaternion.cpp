#include "Quaternion.h"

#include <math.h>
#include "Vector/Vector.h"

namespace _Math
{
	void RQuaternion_build(float p_vec[3], float p_scal, float p_out[4])
	{
		p_out[0] = p_vec[0];
		p_out[1] = p_vec[1];
		p_out[2] = p_vec[2];
		p_out[3] = p_scal;
	};

	void RQuaternion_conjugate(float p_quat[4], float p_out[4])
	{
		RVector_3_mul(p_quat, -1.0f, p_out);
		p_out[3] = p_quat[3];
	};

	void RQuaternion_normalize(float p_quat[4], float p_out[4])
	{
		RVector_4_mul(p_quat, 1 / RVector_4_length(p_quat), p_out);
	};

	void RQuaternion_mul(float p_left[4], float p_right[4], float p_out[4])
	{
		RVector_3_mul(p_left, p_right[3], p_out);
		{
			float l_tmp[3];
			RVector_3_mul(p_right, p_left[3], l_tmp);
			RVector_3_add(p_out, l_tmp, p_out);

			RVector_3_cross(p_left, p_right, l_tmp);
			RVector_3_add(p_out, l_tmp, p_out);
		}

		p_out[3] = (p_left[3] * p_right[3]) - RVector_3_dot(p_left, p_right);
	};

	void RQuaternion_rotateAround(float p_axis[3], float p_angle, float out_quat[4])
	{
		RVector_3_mul(p_axis, sinf(p_angle * 0.5f), out_quat);
		out_quat[3] = cosf(p_angle * 0.5f);
	};
}