#ifndef __MATHRECTANGLE_H__
#define __MATHRECTANGLE_H__

#include "Vector.h"

namespace SDK
{

	namespace Math
	{

		template <typename CoordinateType>
		class Rectangle
		{
		public:
			typedef Vector<CoordinateType, 2> Vector2;
			typedef CoordinateType CoordinateType;
		private:
			Vector2			m_center;
			CoordinateType	m_size[2];

		public:
			Rectangle()
				: m_center()
			{
				m_size[0] = 0;
				m_size[1] = 0;
			}
			Rectangle(const Vector2& i_center, CoordinateType i_width, CoordinateType i_height)
				: m_center(i_center)
			{
				m_size[0] = i_width;
				m_size[1] = i_height;
			}

			void			SetCenter(const Vector2& i_center) { m_center = i_center; }
			void			SetWidth(CoordinateType i_width) { m_size[0] = i_width; }
			void			SetHeight(CoordinateType i_height) { m_size[1] = i_height; }
			void			SetSize(CoordinateType i_width, CoordinateType i_height) { m_size[0] = i_width; m_size[1] = i_height; }

			CoordinateType	Width() const { return m_size[0]; }
			CoordinateType	Height() const { return m_size[1]; }
			Vector2			Center() const { return m_center; }
		};

	} // Math
} // SDK

#endif