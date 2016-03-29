#ifndef __EVENT_H__
#define __EVENT_H__

#include "WorldInterfacingAPI.h"

#include "TypeDefines.h"

#include <memory>

namespace SDK
	{

	struct WORLDINTERFACING_API EventBase
		{
		const uint	m_code;
		const void* mp_data;

		EventBase (uint i_code, void* ip_data = nullptr);
		virtual ~EventBase();
		};

#pragma region DeletableEvent
	//template <typename DataType, typename Deleter = std::default_delete<DataType>>
	//struct DeletableEvent : EventBase
	//	{
	//	public:
	//		typedef Deleter TypeDeleter;
	//	
	//	private:
	//		Deleter			m_deleter;
	//		const bool	m_is_own_data;

	//	protected:
	//		virtual EventBase CopyFrom (const EventBase& i_event) override;
	//		virtual EventBase MoveFrom (EventBase&& i_event) override;

	//	public:
	//		DeletableEvent(const DeletableEvent& i_event);
	//		DeletableEvent(DeletableEvent&& i_event);
	//		DeletableEvent (uint i_code, void* ip_data = nullptr, bool i_take_ownage = false, Deleter i_deleter = TypeDeleter());
	//		virtual ~DeletableEvent();
	//	};

	//////////////////////////////////////////////////////////////////////////////////////////////

	//template <typename DataType, typename Deleter>
	//DeletableEvent<DataType, Deleter>::DeletableEvent(const DeletableEvent& i_event)
	//	: EventBase(i_event)
	//	, m_deleter(i_event.m_deleter)
	//	, m_is_own_data(i_event.m_is_own_data)
	//	{		}

	//template <typename DataType, typename Deleter>
	//DeletableEvent<DataType, Deleter>::DeletableEvent(DeletableEvent&& i_event)
	//	: EventBase(i_event)
	//	, m_deleter(i_event.m_deleter)
	//	, m_is_own_data(i_event.m_is_own_data)
	//	{
	//	*const_cast<bool*>(&i_event.m_is_own_data ) = false;
	//	}
	//
	//template <typename DataType, typename Deleter>
	//DeletableEvent<DataType, Deleter>::DeletableEvent (uint i_code, void* ip_data /*= nullptr*/, bool i_take_ownage /*= false*/, Deleter i_deleter /*= TypeDeleter*/)
	//	: EventBase(i_code, ip_data, i_take_ownage)
	//	, m_deleter(i_deleter)
	//	, m_is_own_data(i_take_ownage)
	//	{		}

	//template <typename DataType, typename Deleter>
	//DeletableEvent<DataType, Deleter>::~DeletableEvent()
	//	{
	//	if (mp_data != nullptr && m_is_own_data)
	//		{
	//		m_deleter(const_cast<void*>(mp_data));
	//		mp_data = nullptr;
	//		}
	//	}

	//template <typename DataType, typename Deleter>
	//void DeletableEvent<DataType, Deleter>::CopyFrom(const EventBase& i_event)
	//	{
	//	m_is_own_data = false;
	//	}

	//template <typename DataType, typename Deleter>
	//void DeletableEvent<DataType, Deleter>::MoveFrom(EventBase&& i_event)
	//	{
	//	if (typeid(DeletableEvent<DataType, Deleter>) != typeid(i_event))
	//		return;
	//	DeletableEvent<DataType, Deleter>* p_ev = static_cast(&i_event);
	//	*const_cast<bool*>(&p_ev->m_is_own_data ) = false;
	//	}
#pragma endregion

	} // SDK

#endif