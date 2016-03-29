#include "stdafx.h"

#include "GLLightsController.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>

namespace SDK
{
	namespace Render
	{
		constexpr static int MAX_LIGHTS = GL_LIGHT7 - GL_LIGHT0;
		constexpr int LIGHTS_FIRST_SIZE = MAX_LIGHTS;
		

		GLLightsController::GLLightsController()
			: m_lights_enabled(false)
			, m_active_lights(0)
		{			
			m_lights.resize(LIGHTS_FIRST_SIZE);
		}

		void GLLightsController::EnableLighting()
		{
			m_lights_enabled = true;
			glEnable(GL_LIGHTING);
		}

		void GLLightsController::DisableLighting()
		{
			glDisable(GL_LIGHTING);
			m_lights_enabled = false;
		}

		bool GLLightsController::LightingEnabled() const
		{
			return m_lights_enabled;
		}

		void GLLightsController::ApplyLights()
		{
			if (m_lights_enabled == false)
				return;


		}

		void GLLightsController::ResetLights()
		{

		}

		LightHandle GLLightsController::AddLight(const Light& i_light)
		{			
			if (m_lights.size() == MAX_LIGHTS)
				return INVALID_LIGHT_HANDLE;

			int light_num = GL_LIGHT0 + m_lights.size();
			m_lights.emplace_back(i_light);
			
			// set up light colors (ambient, diffuse, specular)
			GLfloat lightKa[] = { .0f, .0f, .0f, 1.0f };      // ambient light
			GLfloat lightKd[] = { .9f, .9f, .9f, 1.0f };      // diffuse light
			GLfloat lightKs[] = { 1, 1, 1, 1 };               // specular light
			glLightfv(light_num, GL_AMBIENT, i_light.m_ambient);
			glLightfv(light_num, GL_DIFFUSE, i_light.m_diffuse);
			glLightfv(light_num, GL_SPECULAR, i_light.m_specular);

			// position the light in eye space
			float lightPos[4] = { 0, 0, 1, 0 };               // directional light
			glLightfv(light_num, GL_POSITION, i_light.m_position);

			glEnable(light_num);                            // MUST enable each light source after configuration

			return m_lights.size() - 1;
		}

		void GLLightsController::RemoveLight(LightHandle i_handle)
		{

		}

	} // Render
} // SDK