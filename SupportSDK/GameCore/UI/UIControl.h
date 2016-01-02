#ifndef	__GAMECORE_UICONTROL_H__
#define __GAMECORE_UICONTROL_H__

#include "../GameCoreAPI.h"
#include "UITypes.h"

namespace SDK
{
	class PropertyElement;

	namespace UI
	{
		class UIControlSystem;

		class UIControl
		{
		protected:
			UIControlHandler m_this_handler;

			std::string m_name;
			UIControlHandler m_parent;
			std::vector<UIControlHandler> m_children;
			
			// [0;1]
			Vector2 m_relative_position;
			Vector2 m_scale;
			Vector2 m_rotation;
			Vector2 m_size;			
			// [0-width; 0-height] - TODO: is it needed
			Vector2i m_global_position;
			Vector2i m_global_size;
			// precalculated rectangle
			IRect m_rect;

		private:
			virtual void DrawImpl() = 0;
			virtual void UpdateImpl(float i_elapsed_time) = 0;
			virtual void LoadImpl(const PropertyElement& i_element) = 0;
			
		public:			
			GAMECORE_EXPORT UIControl();
			GAMECORE_EXPORT virtual ~UIControl();
			
			GAMECORE_EXPORT std::string GetName() const { return m_name; }

			GAMECORE_EXPORT void Update(float i_elapsed_time);
			GAMECORE_EXPORT void Draw();
			
			GAMECORE_EXPORT void SetParent(UIControlHandler i_parent);
			UIControlHandler GetParent() const { return m_parent; }
			GAMECORE_EXPORT UIControl* GetParentPtr() const;

			GAMECORE_EXPORT  void AddChild(UIControlHandler i_child);
			GAMECORE_EXPORT  void RemoveChild(UIControlHandler i_child);

			// Return in pixels [0-width; 0-height]
			GAMECORE_EXPORT Vector2i GetGlobalPosition() const;			
			GAMECORE_EXPORT Vector2i GetGlobalSize() const;

			virtual bool IsHited(const Vector2i& i_hit) const;
			virtual void OnResize(const IRect& i_new_size);

			// return [0, 1] values
			Vector2 GetRelativeScale() const { return m_scale; }
			Vector2 GetRelativePosition() const { return m_relative_position; }
			Vector2 GetRelativeSize() const { return m_size; }

			void Load(const PropertyElement& i_element);

			void InitializeThisHandler();			
		};


	} // UI
} // SDK

#endif
