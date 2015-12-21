#ifndef	__GAMECORE_UICONTROL_H__
#define __GAMECORE_UICONTROL_H__

#include "../GameCoreAPI.h"

namespace SDK
{
	class PropertyElement;

	namespace UI
	{

		class UIControl
		{
		protected:
			UIControl* mp_parent;
			std::vector<UIControl*> m_children;

			// [0;1]
			Vector2 m_global_position;
			// TODO
			Vector2 m_relative_position;

		private:
			virtual void DrawImpl() = 0;
			virtual void UpdateImpl(float i_elapsed_time) = 0;
			
		public:
			GAMECORE_EXPORT UIControl();
			GAMECORE_EXPORT virtual ~UIControl();
			
			GAMECORE_EXPORT void Update(float i_elapsed_time);
			GAMECORE_EXPORT void Draw();

			GAMECORE_EXPORT void SetParent(UIControl* ip_parent);
			UIControl* GetParent() const { return mp_parent; }

			GAMECORE_EXPORT  void AddChild(UIControl* ip_child);
			GAMECORE_EXPORT  void RemoveChild(UIControl* ip_child);

			virtual void Load(const PropertyElement& i_element) = 0;
		};


	} // UI
} // SDK

#endif
