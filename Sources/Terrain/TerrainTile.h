#ifndef	__TERRAINTILE_H__
#define __TERRAINTILE_H__

#include "TerrainAPI.h"

namespace SDK
	{
	namespace Terrain
		{

		class Tile
			{
			private:
				int m_type;
				Math::Vector3Real m_position;

			public:
				TERRAIN_API Tile();
				TERRAIN_API ~Tile();

				int GetType() { return m_type; }
				void SetType(int i_type) { m_type = i_type; }
				Math::Vector3Real GetPosition() const { return m_position; }
				void SetPosition(const Math::Vector3Real& i_position) { m_position = i_position; }


			};

		} // Terrain
	} // SDK

#endif