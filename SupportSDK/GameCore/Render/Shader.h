#ifndef	__GAMECORE_SHADER_H__
#define __GAMECORE_SHADER_H__

namespace SDK
{
	namespace Render
	{

		class Shader
		{
		private:
			uint m_program_id;
		public:
			Shader(uint i_program_id)
				: m_program_id(i_program_id)
			{}

			Shader()
				: m_program_id(0)
			{}

			void Reset()
			{
				m_program_id = 0;
			}

			bool IsValid() const
			{
				return m_program_id != 0;
			}

			uint GetId() const
			{
				return m_program_id;
			}
		};

	} // Render
} // SDK

#endif
