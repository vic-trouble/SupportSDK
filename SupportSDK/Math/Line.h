#ifndef	__MATH_LINE_H__
#define __MATH_LINE_H__

#include "Vector.h"

namespace SDK
{
	
	namespace Math
	{
		
		template <typename TCoordinateType, size_t TDimension>
		class Line
		{
		public:
			typedef TCoordinateType CoordinateType;
			enum { Dimension = TDimension };	// helper for expression evaluator
			typedef Line<TCoordinateType, Dimension> ThisType;
			typedef Vector<TCoordinateType, Dimension> VertexType;

		private:
			VertexType m_from;
			VertexType m_to;

		public:
			Line()
			{}
			Line(const VertexType& i_from, const VertexType& i_to)
				: m_from(i_from)
				, m_to(i_to)
			{}

			void Set(const VertexType& i_from, const VertexType& i_to)
			{
				m_from = i_from;
				m_to = i_to;
			}

			VertexType& From()
			{
				return m_from;
			}

			VertexType& To()
			{
				return m_to;
			}

			VertexType From() const
			{
				return m_from;
			}

			VertexType To() const
			{
				return m_to;
			}	

			VertexType Direction() const
			{
				return m_to - m_from;
			}
		};

	} // Math

} // SDK

#endif