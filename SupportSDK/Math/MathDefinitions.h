#ifndef	__MATH_MATHDEFINITIONS_H__
#define __MATH_MATHDEFINITIONS_H__

#include <Math/Constants.h>
#include <Math/Vector.h>
#include <Math/Rectangle.h>
#include <Math/Matrix.h>
#include <Math/Quaternion.h>
#include <Math/Matrix4.h>
#include <Math/Line.h>
#include <Math/Plane.h>

#ifndef DOUBLE_PRECISION
# define FLOAT_PRECISION
#endif

namespace SDK
{

#ifdef FLOAT_PRECISION
	typedef float				real;
#elif DOUBLE_PRECISION
	typedef double				real;
#endif

	typedef Math::Matrix<real, 3, 3> Matrix3;
	typedef Math::Matrix4<real> Matrix4f;

	typedef Math::Vector<real, 2> Vector2;
	typedef Math::Vector<real, 3> Vector3;
	typedef Math::Vector<real, 4> Vector4;

	typedef Math::Quaternion<real> QuaternionF;

	typedef Math::Rectangle<real> FRect;
	typedef Math::Rectangle<int> IRect;

	typedef Math::Line<real, 2> Point2F;
	typedef Math::Line<real, 3> Point3F;

	typedef Math::Plane<real> PlaneF;

	typedef float Radian;

} // SDK

#endif
