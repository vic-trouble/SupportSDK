#ifndef	__PATTERNS_STATEMACHINE_TRANSITIONS_H__
#define __PATTERNS_STATEMACHINE_TRANSITIONS_H__

#include "Executor.h"

namespace SDK
{

	typedef std::unique_ptr<ExBase> ExecutorPtr;
	typedef size_t NextStateType;
	typedef std::pair<NextStateType, ExecutorPtr> TransitionGetterResult;
	template <typename StateMachine>
	struct NoTransition
	{
		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			return std::make_pair(std::type_index(typeid(*this)).hash_code(), nullptr);
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

			if (first_result.first != std::type_index(typeid(first)).hash_code())
				return first_result;
			Result second_result = std::move(second.GetNextState<EventType, StateMachine>(i_event, i_fsm));
			if (second_result.first != std::type_index(typeid(second)).hash_code())
				return second_result;

			return std::make_pair(std::type_index(typeid(*this)).hash_code(), nullptr);
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct Transition
	{
		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm) const
		{
			if (i_fsm.IsStateCurrent<StateFrom>() && typeid(EventType).hash_code() == typeid(TargetEventType).hash_code())
				return std::make_pair(std::type_index(typeid(StateTo)).hash_code(), std::make_unique<Executor<EventType, StateTo>>(i_event, i_fsm.GetState<StateTo>()));
			return std::make_pair(std::type_index(typeid(*this)).hash_code(), nullptr);
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
		template <typename EventType, typename StateMachine, typename row>
		TransitionGetterResult CheckTransition(const EventType& i_event, const StateMachine& i_fsm, bool& io_result, row&)
		{
			static size_t this_index = std::type_index(typeid(*this)).hash_code();
			if (io_result)
				return std::make_pair(this_index, nullptr);
			auto res = row::GetNextState<EventType, StateMachine>(i_event, i_fsm);
			io_result = res.second != nullptr;
			return res;
		}

		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			bool res = false;
			TransitionGetterResult tr_results[] = { CheckTransition(i_event, i_fsm, res, Transitions())... };

			for (auto& result : tr_results)
			{
				if (result.second != nullptr)
					return std::move(result);
			}
			static size_t this_index = std::type_index(typeid(*this)).hash_code();
			return std::make_pair(this_index, nullptr);
		}

	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct TransitionRow
	{
		template <typename EventType, typename StateMachine>
		static TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			static size_t type_to = typeid(StateTo).hash_code();
			static size_t type_this = typeid(TransitionRow<StateFrom, StateTo, TargetEventType>).hash_code();
			static size_t hash_from = typeid(StateFrom).hash_code();
			static size_t hash_event = typeid(TargetEventType).hash_code();

			auto p_state = i_fsm.GetState<StateTo>();
			if (i_fsm.IsStateCurrent<StateFrom>() && typeid(EventType).hash_code() == hash_event)
				return std::make_pair(type_to, std::make_unique<Executor<EventType, StateTo>>(i_event, i_fsm.GetState<StateTo>()));
			return std::make_pair(type_this, nullptr);
		}
	};

	
	template <typename StateFrom, typename StateTo, typename TargetEventType>
	using _row = TransitionRow<StateFrom, StateTo, TargetEventType>;

} // SDK

#endif
