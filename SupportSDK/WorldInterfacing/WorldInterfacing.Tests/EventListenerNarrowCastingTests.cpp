#include "stdafx.h"

#include <Event.h>
#include <EventManager.h>
#include <EventListenerNarrowCasting.h>

using namespace SDK;

namespace
	{

	class NarrowCastListenerrMock : public EventListenerNarrowCasting
	{
	public:
		virtual void HandleEvent(const SDK::EventBase& i_event) override
			{			}
	};

	enum class CodeMask
		{
		First = 1,
		Second = 2,
		Third = 4
		};

	} // namespace

TEST(EventListenerNarrowCastingTests, Defaults_ShouldFalseForAll)
	{
	NarrowCastListenerrMock listener;

	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::First)));
	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Second)));
	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Third)));
	}

TEST(EventListenerNarrowCastingTests, ShouldTrueForFirstAndSecond_FalseForThird)
	{
	NarrowCastListenerrMock listener;
	listener.AddCode(CodeMask::First);
	listener.AddCode(CodeMask::Second);

	EXPECT_TRUE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::First)));
	EXPECT_TRUE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Second)));
	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Third)));
	}

TEST(EventListenerNarrowCastingCastingTests, Flush_ShouldFalseForAll)
	{
	NarrowCastListenerrMock listener;
	listener.AddCode(CodeMask::First);
	listener.AddCode(CodeMask::Second);

	listener.FlushMask();

	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::First)));
	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Second)));
	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Third)));
	}

TEST(EventListenerNarrowCastingCastingTests, SetMask_CorrectMasks)
	{
	NarrowCastListenerrMock listener;
	listener.SetMask(CodeMask::First);
	listener.SetMask(static_cast<uint>(CodeMask::First) | static_cast<uint>(CodeMask::Second));

	EXPECT_TRUE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::First)));
	EXPECT_TRUE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Second)));
	EXPECT_FALSE(listener.IsCodeInteresting(static_cast<uint>(CodeMask::Third)));
	}