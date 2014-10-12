#ifndef __MATHVECTOR_H__
#define __MATHVECTOR_H__

#include <array>

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
				std::array<CoordinateType, Dimension> m_vertices;

			public:
				 Vector (const Vector<CoordinateType, Dimension>& i_other);
				 Vector<CoordinateType, Dimension>&		operator = (const Vector<CoordinateType, Dimension>& i_other);

				Vector ();
				virtual ~Vector ();

				const CoordinateType&													operator [] (size_t i_index) const;
				CoordinateType&																operator [] (size_t i_index);

				Vector<CoordinateType, Dimension>&						operator += (const Vector<CoordinateType, Dimension>& i_other);
				Vector<CoordinateType, Dimension>&						operator -= (const Vector<CoordinateType, Dimension>& i_other);
				Vector<CoordinateType, Dimension>&						operator *= (CoordinateType i_multiplyer);
				Vector<CoordinateType, Dimension>&						operator /= (CoordinateType i_multiplyer);
				bool																					operator == (const Vector<CoordinateType, Dimension>& i_other) const;

				CoordinateType																LengthSq() const;
				double																				Length() const;

				Vector<CoordinateType, Dimension>&						Normalize ();
				bool																					IsZero () const;

				CoordinateType																DotProduct(const Vector<CoordinateType, Dimension>& i_other) const;
			  Vector<CoordinateType, Dimension>							CrossProduct(const Vector<CoordinateType, Dimension>& i_other) const;
			};

		} // Math

	} // SDK

#include "VectorImpl.h"

#endif