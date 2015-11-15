#ifndef __MATH_QUATERNIONIMPL_H__
#define __MATH_QUATERNIONIMPL_H__

namespace SDK
{

	namespace Math
	{
		/////////////////////////////////////////////////////////////////////
		// Constants

		template <typename CT>
		const CT Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::msEpsilon = CT(1e-03);
		template <typename CT>
		const Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::ZERO(0, 0, 0, 0);
		template <typename CT>
		const Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::IDENTITY(1, 0, 0, 0);

		/////////////////////////////////////////////////////////////////////
		// Constructors

		template <typename CT>
		Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Quaternion()
			: x(0)
			, y(0)
			, z(0)
			, w(0)
		{	}

		template <typename CT>
		Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Quaternion(CT i_w, CT i_x, CT i_y, CT i_z)
			: x(i_x)
			, y(i_y)
			, z(i_z)
			, w(i_w)
		{	}

		template <typename CT>
		Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Quaternion(const Quaternion& i_other)
			: x(i_other.x)
			, y(i_other.y)
			, z(i_other.z)
			, w(i_other.w)
		{	}

		template <typename CT>
		Quaternion<CT>& Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator = (const Quaternion<CT>& i_other)
		{
			x = i_other.x;
			y = i_other.y;
			z = i_other.z;
			w = i_other.w;
			return *this;
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Set(CT i_w, CT i_x, CT i_y, CT i_z)
		{
			w = i_w;
			x = i_x;
			y = i_y;
			z = i_z;
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::FromRotationMatrix(const Matrix3& i_rot)
		{
			// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
			// article "Quaternion Calculus and Fast Animation".

			CT fTrace = i_rot[0][0] + i_rot[1][1] + i_rot[2][2];
			CT fRoot;

			if (fTrace > 0.0)
			{
				// |w| > 1/2, may as well choose w > 1/2
				fRoot = sqrt(fTrace + 1.0f);  // 2w
				w = 0.5f*fRoot;
				fRoot = 0.5f / fRoot;  // 1/(4w)
				x = (i_rot[2][1] - i_rot[1][2])*fRoot;
				y = (i_rot[0][2] - i_rot[2][0])*fRoot;
				z = (i_rot[1][0] - i_rot[0][1])*fRoot;
			}
			else
			{
				// |w| <= 1/2
				static size_t s_iNext[3] = { 1, 2, 0 };
				size_t i = 0;
				if (i_rot[1][1] > i_rot[0][0])
					i = 1;
				if (i_rot[2][2] > i_rot[i][i])
					i = 2;
				size_t j = s_iNext[i];
				size_t k = s_iNext[j];

				// TODO: Math::Sqrt
				fRoot = sqrt(i_rot[i][i] - i_rot[j][j] - i_rot[k][k] + 1.0f);
				CT* apkQuat[3] = { &x, &y, &z };
				*apkQuat[i] = 0.5f*fRoot;
				fRoot = 0.5f / fRoot;
				w = (i_rot[k][j] - i_rot[j][k])*fRoot;
				*apkQuat[j] = (i_rot[j][i] + i_rot[i][j])*fRoot;
				*apkQuat[k] = (i_rot[k][i] + i_rot[i][k])*fRoot;
			}
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::ToRotationMatrix(Matrix3& o_rotation_matrix) const
		{
			CT fTx = x + x;
			CT fTy = y + y;
			CT fTz = z + z;
			CT fTwx = fTx*w;
			CT fTwy = fTy*w;
			CT fTwz = fTz*w;
			CT fTxx = fTx*x;
			CT fTxy = fTy*x;
			CT fTxz = fTz*x;
			CT fTyy = fTy*y;
			CT fTyz = fTz*y;
			CT fTzz = fTz*z;

			o_rotation_matrix[0][0] = 1.0f - (fTyy + fTzz);
			o_rotation_matrix[0][1] = fTxy - fTwz;
			o_rotation_matrix[0][2] = fTxz + fTwy;
			o_rotation_matrix[1][0] = fTxy + fTwz;
			o_rotation_matrix[1][1] = 1.0f - (fTxx + fTzz);
			o_rotation_matrix[1][2] = fTyz - fTwx;
			o_rotation_matrix[2][0] = fTxz - fTwy;
			o_rotation_matrix[2][1] = fTyz + fTwx;
			o_rotation_matrix[2][2] = 1.0f - (fTxx + fTyy);
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::FromAngleAxis(const CT& i_radian_angle, const Vector3& i_axis)
		{
			// The quaternion representing the rotation is
			//   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
			CT half_angle(CT(0.5)*i_radian_angle);
			CT s = sin(half_angle);
			w = cos(half_angle);
			x = s*i_axis[0];
			y = s*i_axis[1];
			z = s*i_axis[2];
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::ToAngleAxis(CT& i_radian_angle, Vector3& i_axis) const
		{
			CT fSqrLength = x*x + y*y + z*z;
			if (fSqrLength > 0.0)
			{
				i_radian_angle = 2.0*acos(w);
				CT fInvLength = 1 / sqrt(fSqrLength);
				i_axis[0] = x*fInvLength;
				i_axis[1] = y*fInvLength;
				i_axis[2] = z*fInvLength;
			}
			else
			{
				// angle is 0 (mod 2*pi), so any axis will do
				i_radian_angle = 0;
				i_axis[0] = 1;
				i_axis[1] = 0;
				i_axis[2] = 0;
			}
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
		{
			Matrix3 kRot;

			kRot[0][0] = xAxis[0];
			kRot[1][0] = xAxis[1];
			kRot[2][0] = xAxis[2];

			kRot[0][1] = yAxis[0];
			kRot[1][1] = yAxis[1];
			kRot[2][1] = yAxis[2];

			kRot[0][2] = zAxis[0];
			kRot[1][2] = zAxis[1];
			kRot[2][2] = zAxis[2];

			FromRotationMatrix(kRot);
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const
		{
			Matrix3 kRot;

			ToRotationMatrix(kRot);

			xAxis[0] = kRot[0][0];
			xAxis[1] = kRot[1][0];
			xAxis[2] = kRot[2][0];

			yAxis[0] = kRot[0][1];
			yAxis[1] = kRot[1][1];
			yAxis[2] = kRot[2][1];

			zAxis[0] = kRot[0][2];
			zAxis[1] = kRot[1][2];
			zAxis[2] = kRot[2][2];
		}

		/////////////////////////////////////////////////////////////////////
		// Helpers
		template <typename CT>
		CT Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Length() const
		{
			return sqrt(x*x + y*y + z*z + w*w);
		}

		template <typename CT>
		CT Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::LengthSq() const
		{
			return x*x + y*y + z*z + w*w;
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Normalize()
		{
			CT length = Length();
			w /= length;
			x /= length;
			y /= length;
			z /= length;
		}

		template <typename CT>
		void Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Conjugate()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		/////////////////////////////////////////////////////////////////////
		// Operators

		template <typename CT>
		Quaternion<CT>& Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator += (const Quaternion<CT>& i_other)
		{
			x += i_other.x;
			y += i_other.y;
			z += i_other.z;
			w += i_other.w;
			return *this;
		}

		template <typename CT>
		Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator + (const Quaternion<CT>& i_other) const
		{
			Quaternion res(*this);
			res += i_other;
			return res;
		}

		template <typename CT>
		Quaternion<CT>& Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator -= (const Quaternion<CT>& i_other)
		{
			x -= i_other.x;
			y -= i_other.y;
			z -= i_other.z;
			w -= i_other.w;
			return *this;
		}

		template <typename CT>
		Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator - (const Quaternion<CT>& i_other) const
		{
			Quaternion res(*this);
			res -= i_other;
			return res;
		}

		template <typename CT>
		Quaternion<CT>& Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator *= (const Quaternion<CT>& i_other)
		{
			const auto& q2 = i_other;

			CT _w = w*q2.w - x*q2.x - y*q2.y - z*q2.z;
			CT _x = w*q2.x + x*q2.w + y*q2.z - z*q2.y;
			CT _y = w*q2.y - x*q2.z + y*q2.w + z*q2.x;
			CT _z = w*q2.z + x*q2.y - y*q2.x + z*q2.w;

			Set(_w, _x, _y, _z);
			return *this;
		}

		template <typename CT>
		Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator * (const Quaternion<CT>& i_other) const
		{
			Quaternion res(*this);
			res *= i_other;
			return res;
		}

		template <typename CT>
		Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator * (CT i_scalar) const
		{
			return Quaternion(i_scalar*w, i_scalar*x, i_scalar*y, i_scalar*z);
		}

		template <typename CT>
		Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::operator - () const
		{
			return Quaternion(-w, -x, -y, -z);
		}


		/////////////////////////////////////////////////////////////////////
		// non-member operators

		template <typename CT>
		Quaternion<CT> operator * (CT i_scalar, const Quaternion<CT>& i_right)
		{
			return Quaternion<CT>(i_scalar*i_right.w, i_scalar*i_right.x, i_scalar*i_right.y, i_scalar*i_right.z);
		}

		template <typename CT>
		bool operator == (const Quaternion<CT> i_left, const Quaternion<CT>& i_right)
		{
			return i_left == i_right;
		}

		template <typename CT>
		bool operator != (const Quaternion<CT> i_left, const Quaternion<CT>& i_right)
		{
			return i_left != i_right;
		}

		/////////////////////////////////////////////////////////////////////
		//

		template <typename CT>
		CT Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::DotProduct(const Quaternion<CT>& i_r) const
		{
			return w*i_r.w + x*i_r.x + y*i_r.y + z*i_r.z;
		}

		template <typename CT>
		Vector<CT, 3> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::ApplyTo(const Vector<CT, 3>& i_vector) const
		{
			Vector<CT, 3> quaternion_vec = VectorConstructor<CT>::Construct(x, y, z);
			Vector<CT, 3> t = quaternion_vec.CrossProduct(i_vector) * CT(2);
			return i_vector + t * w + quaternion_vec.CrossProduct(t);
		}

		template <typename CT>
		Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Slerp(CT t, const Quaternion<CT>& p, const Quaternion<CT>& q, bool shortest_path /* = false*/)
		{
			Quaternion result;

			CT cosine = p.DotProduct(q);
			Quaternion<CT> t_quat;

			// if shortest path we need to flip direction {p->q} => {q->p}
			if (cosine < 0.0f && shortest_path)
			{
				cosine = -cosine;
				t_quat = -q;
			}
			else
			{
				t_quat = q;
			}

			if (abs(cosine) < 1 - msEpsilon)
			{
				// standardd case
				CT sinus = sqrt(1 - cosine);
				CT angle = atan2(sinus, cosine);
				CT inv_sin = 1 / sinus;

				CT coeff_0 = sin((1 - t) * angle) * inv_sin;
				CT coeff_1 = sin(t * angle) * inv_sin;

				return coeff_0*p + coeff_1*t_quat;
			}
			else
			{
				// There are two situations:
				// 1. "p" and "q" are very close (fCos ~= +1), so we can do a linear
				//    interpolation safely.
				// 2. "p" and "q" are almost inverse of each other (fCos ~= -1), there
				//    are an infinite number of possibilities interpolation. but we haven't
				//    have method to fix this case, so just use linear interpolation here.
				Quaternion result_q = (CT(1.0) - t)*p + t*t_quat;
				// taking the complement requires renormalisation
				result_q.Normalize();
				return result_q;
			}

			return result;
		}

		template <typename CT>
		Quaternion<CT> Quaternion<CT, typename std::enable_if<std::is_floating_point<CT>::value>::type>::Nlerp(CT t, const Quaternion<CT>& p, const Quaternion<CT>& q, bool shortest_path /* = false*/)
		{
			Quaternion result;
			CT cosine = p.DotProduct(q);
			if (cosine < 0.0f && shortest_path)
			{
				result = p + t * ((-q) - p);
			}
			else
			{
				result = p + t * (q - p);
			}
			result.Normalize();
			return result;
		}

	} // Math

} // SDK

#endif