#ifndef __BASEMOCKS_H__
#define __BASEMOCKS_H__

#include <ISheduler.h>

class ActionHandlerMock
	{
	public:
		MOCK_METHOD2(TestMethod, void(SDK::ulong,SDK::ulong));
	};

struct ActionRecordData
	{
	SDK::ushort		m_frequency;
	SDK::ushort		m_phase;
	SDK::real			m_priority;

	ActionRecordData(SDK::ushort i_frequency,
									 SDK::ushort i_phase,
									 SDK::real i_priority);

	bool operator == (const ActionRecordData& i_other) const;
	};

class ShedulerMock : public SDK::ISheduler
	{
	public:
		MOCK_METHOD1(AddAction_, int(ActionRecordData));
		MOCK_METHOD1(RemoveAction, void(int));
		MOCK_METHOD1(Execute, void(SDK::ulong));
		virtual int AddAction(const SDK::ActionRecord& i_action) override
			{
			return AddAction_(ActionRecordData(i_action.m_frequency, i_action.m_phase, i_action.m_priority));
			}
	};

#endif