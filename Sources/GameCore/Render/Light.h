#ifndef	__GAMECORE_LIGHT_H__
#define __GAMECORE_LIGHT_H__

namespace SDK
{
	namespace Render
	{

		enum class LightType
		{
			Point,/*Point light sources give off light equally in all directions, so require only position not direction.*/
			Directional,/*Directional lights simulate parallel light beams from a distant source, hence have direction but no position.*/
			Spotlight/*Spotlights simulate a cone of light from a source so require position and direction, plus extra values for falloff.*/
		};

		struct Light
		{
			float m_ambient[4];
			float m_diffuse[4];
			float m_specular[4];
			float m_position[4]; // x, y, z, w
			bool m_enabled;
			LightType m_type;
			Light()
				: m_enabled(false)
			{}
		};		
	} // Render
} // SDK

#endif
