#include "RVector.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "Functional/Equals/Equals.hpp"
#include "v2/Quaternion/RQuaternion.h"

namespace _MathV2
{
	const float RIGHT_arr[3] = { 1.0f, 0.0f, 0.0f };
	const float UP_arr[3] = { 0.0f, 1.0f, 0.0f };
	const float FORWARD_arr[3] = { 0.0f, 0.0f, 1.0f };

	inline void RVector_mul(const float* p_left, const int p_left_nb, const float p_value, float* p_out)
	{
		for (int i = 0; i < p_left_nb; i++)
		{
			p_out[i] = p_left[i] * p_value;
		}
	};

	bool RVector_3_equals(const float p_left[3], const float p_right[3])
	{
		return _Core::Equals_float(&p_left[0], &p_right[0])
			&& _Core::Equals_float(&p_left[1], &p_right[1])
			&& _Core::Equals_float(&p_left[2], &p_right[2]);
	};

	void RVector_3_add(const float p_left[3], const float p_right[3], float p_out[3])
	{
		p_out[0] = p_left[0] + p_right[0];
		p_out[1] = p_left[1] + p_right[1];
		p_out[2] = p_left[2] + p_right[2];
	};

	void RVector_3_min(const float p_left[3], const float p_right[3], float p_out[3])
	{
		p_out[0] = p_left[0] - p_right[0];
		p_out[1] = p_left[1] - p_right[1];
		p_out[2] = p_left[2] - p_right[2];
	};

	void RVector_3_mul(const float p_left[3], const float p_right, float p_out[3])
	{
		RVector_mul(p_left, 3, p_right, p_out);
	};

	void RVector_4_mul(const float p_left[4], const float p_right, float p_out[4])
	{
		RVector_mul(p_left, 4, p_right, p_out);
	};

	float RVector_3_length(const float p_vec[3])
	{
		return
			sqrtf(RVector_3_dot(p_vec, p_vec));
	};

	float RVector_4_length(const float p_vec[4])
	{
		return
			sqrtf((p_vec[0] * p_vec[0]) +
				(p_vec[1] * p_vec[1]) +
				(p_vec[2] * p_vec[2]) +
				(p_vec[3] * p_vec[3]));
	};

	void RVector_3_normalize(const float p_vec[3], float p_out[3])
	{
		float l_length = RVector_3_length(p_vec);
		p_out[0] = p_vec[0] / l_length;
		p_out[1] = p_vec[1] / l_length;
		p_out[2] = p_vec[2] / l_length;
	};

	float RVector_3_dot(const float p_left[3], const float p_right[3])
	{
		return
			(p_left[0] * p_right[0]) +
			(p_left[1] * p_right[1]) +
			(p_left[2] * p_right[2]);
	};

	void RVector_3_cross(const float p_left[3], const float p_right[3], float p_out[3])
	{
		p_out[0] = (p_left[1] * p_right[2]) - (p_left[2] * p_right[1]);
		p_out[1] = (p_left[2] * p_right[0]) - (p_left[0] * p_right[2]);
		p_out[2] = (p_left[0] * p_right[1]) - (p_left[1] * p_right[0]);
	};

	float RVector_3_angle(const float p_begin[3], const float p_end[3])
	{
		return acosf(
			RVector_3_dot(p_begin, p_end) / (RVector_3_length(p_begin) * RVector_3_length(p_end))
		);
	};

	float RVector_3_angle_normalized(const float p_begin[3], const float p_end[3])
	{
		return acosf(
			RVector_3_dot(p_begin, p_end)
		);
	};

	short int RVector_3_angleSign(const float p_begin[3], const float p_end[3], const float p_referenceAxis[3])
	{
		float l_cross[3];
		RVector_3_cross(p_begin, p_end, l_cross);
		float l_dot = RVector_3_dot(l_cross, p_referenceAxis);
		return l_dot >= 0.0000f ? 1 : -1;
	};

	void RVector_3_rotate(const float p_vector[3], const float p_rotation[4], float p_out[3])
	{
		float l_vectorAsQuat[4];
		RQuaternion_build(p_vector, 0.0f, l_vectorAsQuat);
		
		float l_rotatedVector[4];
		RQuaternion_mul(p_rotation, l_vectorAsQuat, l_rotatedVector);
		{
			float l_tmp[4];
			RQuaternion_conjugate(p_rotation, l_tmp);

			float l_rotatedVector_cpy[4]; 
			memcpy(l_rotatedVector_cpy, l_rotatedVector, sizeof(float) * 4);
			RQuaternion_mul(l_rotatedVector_cpy, l_tmp, l_rotatedVector);
		}

		RVector_3_normalize(l_rotatedVector, p_out);
	};
}