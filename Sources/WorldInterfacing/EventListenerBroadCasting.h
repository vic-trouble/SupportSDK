#ifndef __EVENTLISTENERBROADCASTING_H__
#define __EVENTLISTENERBROADCASTING_H__

#include "WorldInterfacingAPI.h"
#include "TypeDefines.h"

#include "EventListener.h"

namespace SDK
	{

	/*
	EventListenerBroadCasting provides possibility to manage vector of codes that will be passed
	Typical usage:
		1. AddCode - use this first -> it will push to vector
		2. Initizlize - will sort added codes so search will be faster
	*/
	class EventListenerBroadCasting : public EventListener
		{
		private:
			std::vector<uint> m_intetest_codes;
			bool							m_initized;

		public:
			WORLDINTERFACING_API EventListenerBroadCasting(EventManager* ip_manager = nullptr);
			WORLDINTERFACING_API virtual ~EventListenerBroadCasting();

			WORLDINTERFACING_API void Flush();
			WORLDINTERFACING_API void AddCode(uint i_code);
			WORLDINTERFACING_API void Initialize();

			WORLDINTERFACING_API virtual bool IsCodeInteresting(const uint i_code) const override;
		};

	} // SDK

#endif