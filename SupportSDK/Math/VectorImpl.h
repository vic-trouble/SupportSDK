#ifndef __MATHVECTORIMPL_H__
#define __MATHVECTORIMPL_H__

#include "CrossProductCalculator.h"

#include <cmath>

namespace SDK
	{

	namespace Math
		{

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>::Vector(const Vector<CoordinateType, Dimension>& i_other)
			{
			for (size_t i = 0; i < VertexNumber; ++i)
				m_vertices[i] = i_other.m_vertices[i];
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>& Vector<CoordinateType, Dimension>::operator = (const Vector<CoordinateType, Dimension>& i_other)
			{
			for (size_t i = 0; i < VertexNumber; ++i)
				m_vertices[i] = i_other.m_vertices[i];
			return *this;
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>::Vector ()
			{
			for (size_t i = 0; i < VertexNumber; ++i)
				m_vertices[i] = 0;
			}
		
		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>::~Vector()
			{			}

		template <typename CoordinateType, size_t Dimension>
		const CoordinateType&	Vector<CoordinateType, Dimension>::operator [] (size_t i_index) const
			{
			#ifdef _DEBUG
			if (i_index >= VertexNumber)
				throw std::runtime_error("array index out of bounds");
			#endif
			return m_vertices[i_index];
			}

		template <typename CoordinateType, size_t Dimension>
		CoordinateType&	Vector<CoordinateType, Dimension>::operator [] (size_t i_index)
			{
			#ifdef _DEBUG
			if (i_index >= Dimension)
				throw std::runtime_error("array index out of bounds");
			#endif
			return m_vertices[i_index];
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>& Vector<CoordinateType, Dimension>::operator += (const Vector<CoordinateType, Dimension>& i_other)
			{
			for (size_t i = 0; i < Dimension; ++i)
				m_vertices[i] += i_other.m_vertices[i];
			return *this;
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>& Vector<CoordinateType, Dimension>::operator -= (const Vector<CoordinateType, Dimension>& i_other)
			{
			for (size_t i = 0; i < Dimension; ++i)
				m_vertices[i] -= i_other.m_vertices[i];
			return *this;
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>& Vector<CoordinateType, Dimension>::operator *= (CoordinateType i_multiplyer)
			{
			for (size_t i = 0; i < Dimension; ++i)
				m_vertices[i] *= i_multiplyer;
			return *this;
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>& Vector<CoordinateType, Dimension>::operator /= (CoordinateType i_multiplyer)
			{
			for (size_t i = 0; i < Dimension; ++i)
				m_vertices[i] /= i_multiplyer;
			return *this;
			}

		template <typename CoordinateType, size_t Dimension>
		bool Vector<CoordinateType, Dimension>::operator == (const Vector<CoordinateType, Dimension>& i_other) const
			{
			for (size_t i = 0; i < Dimension; ++i)
				if (m_vertices[i] != i_other.m_vertices[i])
					return false;
			return true;
			}

		template <typename CoordinateType, size_t Dimension>
		CoordinateType Vector<CoordinateType, Dimension>::LengthSq() const
			{
			CoordinateType length_sq = 0;
			for (size_t i = 0; i < Dimension; ++i)
				length_sq += m_vertices[i] * m_vertices[i];
			return length_sq;
			}

		template <typename CoordinateType, size_t Dimension>
		double Vector<CoordinateType, Dimension>::Length() const
			{
			return sqrt(LengthSq());
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension>& Vector<CoordinateType, Dimension>::Normalize ()
			{
			*this /= Length();
			return *this;
			}

		template <typename CoordinateType, size_t Dimension>
		bool Vector<CoordinateType, Dimension>::IsZero () const
			{
			for (size_t i = 0; i < Dimension; ++i)
				if (abs(m_vertices[i]) > std::numeric_limits<CoordinateType>::min())
					return false;
			return true;
			}

		template <typename CoordinateType, size_t Dimension>
		CoordinateType Vector<CoordinateType, Dimension>::DotProduct(const Vector<CoordinateType, Dimension>& i_other) const
			{
			CoordinateType dot_product = 0;
			for (size_t i = 0; i < Dimension; ++i)
				dot_product += m_vertices[i]*i_other.m_vertices[i];
			return dot_product;
			}

		template <typename CoordinateType, size_t Dimension>
		Vector<CoordinateType, Dimension> Vector<CoordinateType, Dimension>::CrossProduct (const Vector<CoordinateType, Dimension>& i_other) const
			{
			return CrossProductCalculator<CoordinateType, Dimension>::Calculate(*this, i_other);
			}

		} // Math

	} // SDK

#endif