#ifndef	__PATTERNS_STATEMACHINE_TRANSITIONS_H__
#define __PATTERNS_STATEMACHINE_TRANSITIONS_H__

#include "Executor.h"

namespace SDK
{

	typedef std::unique_ptr<ExBase> ExecutorPtr;
	typedef std::type_index NextStateType;
	typedef std::pair<NextStateType, ExecutorPtr> TransitionGetterResult;
	template <typename StateMachine>
	struct NoTransition
	{
		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			return std::make_pair(std::type_index(typeid(*this)), nullptr);
		}
	};

	template <typename FirstTransition, typename SecondTransition>
	struct CompoundTransition
	{
		FirstTransition first;
		SecondTransition second;

		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			typedef std::pair<NextStateType, ExecutorPtr> Result;
			Result first_result = std::move(first.GetNextState<EventType, StateMachine>(i_event, i_fsm));

			if (first_result.first != typeid(first))
				return first_result;
			Result second_result = std::move(second.GetNextState<EventType, StateMachine>(i_event, i_fsm));
			if (second_result.first != typeid(second))
				return second_result;

			return std::make_pair(std::type_index(typeid(*this)), nullptr);
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct Transition
	{
		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm) const
		{
			if (i_fsm.IsStateCurrent<StateFrom>() && typeid(EventType) == typeid(TargetEventType))
				return std::make_pair(std::type_index(typeid(StateTo)), std::make_unique<Executor<EventType, StateTo>>(i_event, i_fsm.GetState<StateTo>()));
			return std::make_pair(std::type_index(typeid(*this)), nullptr);
		}
	};

	template <typename FirstTrans, typename SecondTrans>
	using _comp_tr = SDK::CompoundTransition<FirstTrans, SecondTrans>;
	template <typename FrState, typename ToState, typename Event>
	using _tr = SDK::Transition<FrState, ToState, Event>;

	////////////////////////////////////////////////////////////////////
	// Transitions table
	template <typename... Transitions>
	struct TransitionsTable
	{

		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			TransitionGetterResult tr_results[] = { Transitions::GetNextState<EventType, StateMachine>(i_event, i_fsm)... };

			for (auto& result : tr_results)
			{
				if (result.second != nullptr)
					return std::move(result);
			}

			return std::make_pair(std::type_index(typeid(*this)), nullptr);
		}

	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct TransitionRow
	{
		template <typename EventType, typename StateMachine>
		static TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			if (i_fsm.IsStateCurrent<StateFrom>() && typeid(EventType) == typeid(TargetEventType))
				return std::make_pair(std::type_index(typeid(StateTo)), std::make_unique<Executor<EventType, StateTo>>(i_event, i_fsm.GetState<StateTo>()));
			return std::make_pair(std::type_index(typeid(Transition<StateFrom, StateTo, TargetEventType>)), nullptr);
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	using _row = TransitionRow<StateFrom, StateTo, TargetEventType>;

} // SDK

#endif
