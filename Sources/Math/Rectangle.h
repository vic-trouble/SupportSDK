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
			void			SetBottomLeft(const Vector2& i_bot_left) { m_center = { i_bot_left[0] + m_size[0] / 2, i_bot_left[1] + m_size[1] / 2 }; }
			Vector2			GetBottomLeft() const { return{ m_center[0] - m_size[0] / 2, m_center[1] - m_size[1] / 2 }; }
			void			SetWidth(CoordinateType i_width) { m_size[0] = i_width; }
			void			SetHeight(CoordinateType i_height) { m_size[1] = i_height; }
			void			SetSize(CoordinateType i_width, CoordinateType i_height) { m_size[0] = i_width; m_size[1] = i_height; }

			CoordinateType	Width() const { return m_size[0]; }
			CoordinateType	Height() const { return m_size[1]; }
			Vector2			Center() const { return m_center; }

			bool			IsInside(const Vector2& i_point) const
			{
				const CoordinateType half_size[] = { m_size[0] / 2, m_size[1] / 2 };
				return m_center[0] - half_size[0] < i_point[0] && m_center[0] + half_size[0] > i_point[0]
					&& m_center[1] - half_size[1] < i_point[1] && m_center[1] + half_size[1] > i_point[1];
			}
		};

	} // Math
} // SDK

#endif