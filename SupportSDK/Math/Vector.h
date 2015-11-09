#ifndef __MATHVECTOR_H__
#define __MATHVECTOR_H__

#if _MSC_VER >= 1800
#include <initializer_list>
#endif

namespace SDK
	{

	namespace Math
		{

		template <typename CoordinateType, size_t Dimension>
		class Vector
			{
			public:
				enum { VertexNumber = Dimension };	// helper for expression evaluator
				typedef CoordinateType CoordinateType;
				
			private:
				CoordinateType m_vertices[Dimension];

			public:

#if _MSC_VER >= 1800
				Vector(std::initializer_list<CoordinateType> coordinates)
					{
					if (coordinates.size() > VertexNumber)
						throw std::logic_error("Initializer list contains more entries than dimension");
					std::copy(coordinates.begin(), coordinates.end(), m_vertices);
					}
#endif

				Vector (const Vector<CoordinateType, Dimension>& i_other);
				Vector<CoordinateType, Dimension>&	operator = (const Vector<CoordinateType, Dimension>& i_other);

				Vector ();

				CoordinateType*						GetPointer() { return m_vertices; }

				const CoordinateType&				operator [] (size_t i_index) const;
				CoordinateType&						operator [] (size_t i_index);

				Vector<CoordinateType, Dimension>&	operator += (const Vector<CoordinateType, Dimension>& i_other);
				Vector<CoordinateType, Dimension>	operator + (const Vector<CoordinateType, Dimension>& i_other) const;
				Vector<CoordinateType, Dimension>&	operator -= (const Vector<CoordinateType, Dimension>& i_other);
				Vector<CoordinateType, Dimension>	operator - (const Vector<CoordinateType, Dimension>& i_other) const;
				Vector<CoordinateType, Dimension>	operator - () const;

				Vector<CoordinateType, Dimension>&	operator *= (CoordinateType i_multiplyer);
				Vector<CoordinateType, Dimension>	operator * (CoordinateType i_multiplyer) const;
				Vector<CoordinateType, Dimension>&	operator /= (CoordinateType i_multiplyer);
				Vector<CoordinateType, Dimension>	operator / (CoordinateType i_multiplyer) const;
				bool								operator == (const Vector<CoordinateType, Dimension>& i_other) const;

				CoordinateType						LengthSq() const;
				CoordinateType						Length() const;

				Vector<CoordinateType, Dimension>&	Normalize ();
				bool								IsZero () const;

				CoordinateType						DotProduct(const Vector<CoordinateType, Dimension>& i_other) const;
				Vector<CoordinateType, Dimension>	CrossProduct(const Vector<CoordinateType, Dimension>& i_other) const;

				// Multiplies every component of this vector by the same component of scale
				void								Scale(const Vector<CoordinateType, Dimension>& i_other);
			};

		} // Math

	} // SDK

#include "VectorImpl.h"

#endif