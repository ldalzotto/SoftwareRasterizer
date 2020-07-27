#pragma once

namespace _GameEngine::_Math
{
	struct Vector2f;
	struct Vector2d;
	struct Vector3f;
	struct Vector4f;
}

namespace _GameEngine::_Math
{
	void Vector2f_normalize(Vector2f* p_vector);
	float Vector2f_dot(Vector2f* p_left, Vector2f* p_right);
	float Vector2f_length(Vector2f* p_vec);

	void Vector2d_normalize(Vector2d* p_vector);
	void Vector2d_min(Vector2d* p_left, Vector2d* p_right, Vector2d* p_out);
	bool Vector2d_equals(Vector2d* p_left, Vector2d* p_right);

	void Vector3f_build(Vector4f* p_xyz, Vector3f* out);

	bool Vector3f_equals(Vector3f* left, Vector3f* right);
	void Vector3f_normalize(Vector3f* p_vec);
	float Vector3f_length(Vector3f* p_vec);
	float Vector3f_distance(Vector3f* p_vec, Vector3f* p_target);
	
	void Vector3f_mul(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_mul(Vector3f* p_vector, float p_value, Vector3f* p_out);
	void Vector3f_add(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_min(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	
	float Vector3f_dot(Vector3f* p_vector, Vector3f* p_other);
	void Vector3f_cross(Vector3f* p_vector, Vector3f* p_other, Vector3f* p_out);
	void Vector3f_project(Vector3f* p_vector, Vector3f* p_projectedOn, Vector3f* p_out);
	float Vector3f_angle_signed(Vector3f* p_p1, Vector3f* p_p2);
	
	float Vector3f_angle(Vector3f* p_vector, Vector3f* p_other);

	bool Vector4f_equals(Vector4f* left, Vector4f* right);
	void Vector4f_build(Vector3f* p_xyz, float p_w, Vector4f* out);
	void Vector4f_build(Vector2f* p_xy, float p_z, float p_w, Vector4f* out);

	void Vector4f_mul(Vector4f* p_vect, float p_value, Vector4f* p_out);
	void Vector4f_homogenize_w(Vector4f* p_vect, Vector4f* p_out);

	float Vector4f_length(Vector4f* p_vec);
	void Vector4f_add(Vector4f* p_vector, Vector4f* p_other, Vector4f* p_out);
	void Vector4f_add(Vector4f* p_vector, Vector3f* p_other, Vector4f* p_out);

}