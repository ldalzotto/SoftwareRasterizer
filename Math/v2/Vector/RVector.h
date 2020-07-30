#pragma once

namespace _MathV2
{
	extern const float RIGHT_arr[3];
	extern const float UP_arr[3];
	extern const float FORWARD_arr[3];

	bool RVector_3_equals(const float p_left[3], const float p_right[3]);

	void RVector_3_add(const float p_left[3], const float p_right[3], float p_out[3]);
	void RVector_3_min(const float p_left[3], const  float p_right[3], float p_out[3]);

	void RVector_3_mul(const float p_left[3], const float p_right, float p_out[3]);
	void RVector_4_mul(const float p_left[4], const float p_right, float p_out[4]);

	float RVector_3_length(const float p_vec[3]);
	float RVector_4_length(const float p_vec[4]);

	void RVector_3_normalize(const float p_vec[3], float p_out[3]);

	float RVector_3_dot(const float p_left[3], const float p_right[3]);
	
	void RVector_3_cross(const float p_left[3], const float p_right[3], float p_out[3]);

	float RVector_3_angle(const float p_begin[3], const  float p_end[3]);
	float RVector_3_angle_normalized(const float p_begin[3], const float p_end[3]);

	short int RVector_3_angleSign(const float p_begin[3], const  float p_end[3], const float p_referenceAxis[3]);

	void RVector_3_rotate(const float p_vector[3], const float p_rotation[4], float p_out[3]);

}