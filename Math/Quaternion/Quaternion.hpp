#pragma once

#include "Vector/Vector.hpp"

namespace _Math
{
	template <typename T>
	struct Quaternion
	{
		T x;
		T y;
		T z;
		T w = 1;
	};

	template <typename T>
	Vector3<T> Quaternion_getVector(Quaternion<T>& p_quat)
	{
		return Vector3<T>::build(p_quat.x, p_quat.y, p_quat.z);
	};

	template <typename T>
	Quaternion<T> Quaternion_build(Vector4<T>&& p_vec4)
	{
		return *(Quaternion<T>*) & p_vec4;
	};

	template <typename T>
	Quaternion<T> Quaternion_build(Vector3<T>& p_vector, T p_scalar)
	{
		return Quaternion<T>
		{
			p_vector.x,
				p_vector.y,
				p_vector.z,
				p_scalar
		};
	};

	template <typename T>
	Quaternion<T> Quaternion_conjugate(Quaternion<T>& p_quat)
	{
		return Quaternion_build(
			Quaternion_getVector(p_quat).mul(-1.0f),
			p_quat.w
		);
	};

	template <typename T>
	Quaternion<T> Quaternion_normalize(Quaternion<T>& p_quat)
	{
		Vector4<T>* p_quat_as_vector = (Vector4<T>*)(&p_quat);
		return Quaternion_build(Vector4_mul(*p_quat_as_vector, 1 / Vector4_length(*p_quat_as_vector)));
	};

	template <typename T>
	Quaternion<T> Quaternion_normalize(Quaternion<T>&& p_quat)
	{
		return Quaternion_normalize(p_quat);
	};

	template <typename T>
	Quaternion<T> Quaternion_rotateAround(Vector3<T>& p_axis, T p_angle)
	{
		return Quaternion_build(
			p_axis.mul(sinf(p_angle * 0.5f)),
			cosf(p_angle * 0.5f)
		);
	}

	template <typename T>
	Quaternion<T> Quaternion_mul(Quaternion<T>& p_left, Quaternion<T>& p_right)
	{
		Vector3<T> p_left_vector = Quaternion_getVector(p_left);
		Vector3<T> p_right_vector = Quaternion_getVector(p_right);

		Vector3<T> l_vec =
				p_left_vector.mul(p_right.w)
				.add(p_right_vector.mul(p_left.w))
				.add(p_left_vector.cross(p_right_vector));

		T l_scal = (p_left.w * p_right.w) - p_left_vector.dot(p_right_vector);
		return Quaternion_build(l_vec, l_scal);
	};

	template <typename T>
	Vector3<T> Vector_rotate(Vector3<T>& p_vector, Quaternion<T>& p_rotation)
	{
		Quaternion<T> l_vectorAsQuat = Quaternion_build(p_vector, 0.0f);
		Quaternion<T> l_rotatedVector = Quaternion_mul(p_rotation, l_vectorAsQuat);
		l_rotatedVector = Quaternion_mul(l_rotatedVector, Quaternion_conjugate(p_rotation));
		return Quaternion_getVector(l_rotatedVector).normalize();
	};

	template <typename T>
	Vector3<T> Vector_rotate(Vector3<T>&& p_vector, Quaternion<T>&& p_rotation)
	{
		return Vector_rotate(p_vector, p_rotation);
	};
}