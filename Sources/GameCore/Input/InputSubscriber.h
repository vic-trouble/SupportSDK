#ifndef	__CORE_INPUTSUBSCRIBER_H__
#define __CORE_INPUTSUBSCRIBER_H__

namespace SDK
{
	struct KeyEvent;
	struct MouseEvent;

	class InputSubscriber
	{
	public:
		virtual ~InputSubscriber(){}

		virtual bool KeyPressed(const KeyEvent& i_evt) { return false; }
		virtual bool KeyReleased(const KeyEvent& i_evt) { return false; }

		virtual bool MouseMoved(const MouseEvent& i_evt) { return false; }
		virtual bool MousePressed(const MouseEvent& i_evt) { return false; }
		virtual bool MouseReleased(const MouseEvent& i_evt) { return false; }
	};


} // SDK

#endif