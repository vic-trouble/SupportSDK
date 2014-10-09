#ifndef __EVENTLISTENERNARROWCASTING_H__
#define __EVENTLISTENERNARROWCASTING_H__

#include "WorldInterfacingAPI.h"
#include "TypeDefines.h"

#include "EventListener.h"

#include <bitset>

namespace SDK
	{

	class WORLDINTERFACING_API EventListenerNarrowCasting : public EventListener
		{
		private:
			uint m_interest_code_mask;
			
		public:
			EventListenerNarrowCasting(uint i_interest_code_mask = 0, EventManager* ip_manager = nullptr);
			virtual ~EventListenerNarrowCasting();

			void FlushMask();
			// Usually mask is an enum value so it should be casted to uint normally
			//	Anyway, type that provide operator for casting could be passed as parameter
			template <typename MaskType>
			void SetMask(MaskType i_code_mask);
			template <typename MaskType>
			void AddCode(MaskType i_code);

			virtual bool IsCodeInteresting(const uint i_code) const override;
		};

	///////////////////////////////////////////////////////////////////////////////////////////

	template <typename MaskType>
	void EventListenerNarrowCasting::SetMask(MaskType i_code_mask)
		{
		m_interest_code_mask = static_cast<uint>(i_code_mask);
		}

	template <typename MaskType>
	void EventListenerNarrowCasting::AddCode(MaskType i_code_mask)
		{
		m_interest_code_mask |= static_cast<uint>(i_code_mask);
		}

	} // SDK

#endif