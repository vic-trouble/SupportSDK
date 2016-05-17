#ifndef	__PATTERNS_STATEMACHINE_TRANSITIONS_H__
#define __PATTERNS_STATEMACHINE_TRANSITIONS_H__

// disable warning about name truncating;
// The correctness of the program, however, is unaffected by the truncated name. And hopefully you will not debug this code
// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(C4503)&rd=true
#pragma warning (disable : 4503)

namespace SDK
{
	typedef std::function<void()> ExecFunction;
	typedef size_t NextStateType;
	typedef std::pair<NextStateType, ExecFunction> TransitionGetterResult;
	template <typename StateMachine>
	struct NoTransition
	{
		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			return std::make_pair(typeid(*this).hash_code(), ExecFunction());
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType, typename Event, typename Enable = void>
	struct RowChecker {};

	template <typename StateFrom, typename StateTo, typename TargetEventType, typename Event>
	struct RowChecker <StateFrom, StateTo, TargetEventType, Event, typename std::enable_if<std::is_same<TargetEventType, Event>::value>::type>
	{
		template <typename StateMachine>
		static TransitionGetterResult GetNextState(const TargetEventType& i_event, const StateMachine& i_fsm, const size_t i_caller_type)
		{
			static size_t type_to = typeid(StateTo).hash_code();
			static void(StateTo::*enter_func)(const TargetEventType&) = &StateTo::OnEnter;

			if (i_fsm.IsStateCurrent<StateFrom>())
				return std::make_pair(
					type_to,
					std::bind(enter_func, i_fsm.GetState<StateTo>(), static_cast<const TargetEventType&>(i_event))
					);
			return std::make_pair(i_caller_type, ExecFunction());
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType, typename Event>
	struct RowChecker <StateFrom, StateTo, TargetEventType, Event, typename std::enable_if<!std::is_same<TargetEventType, Event>::value>::type>
	{
		template <typename StateMachine>
		static TransitionGetterResult GetNextState(const Event& i_event, const StateMachine& i_fsm, const size_t i_caller_type)
		{
			return std::make_pair(i_caller_type, ExecFunction());
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
			typedef std::pair<NextStateType, ExecFunction> Result;
			Result first_result = first.GetNextState<EventType, StateMachine>(i_event, i_fsm);

			if (first_result.first != typeid(first).hash_code())
				return first_result;
			Result second_result = second.GetNextState<EventType, StateMachine>(i_event, i_fsm);
			if (second_result.first != typeid(second).hash_code())
				return second_result;

			return std::make_pair(typeid(*this).hash_code(), ExecFunction());
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct Transition
	{
		using _ThisType = Transition<StateFrom, StateTo, TargetEventType>;
		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm) const
		{
			static size_t caller_type = typeid(_ThisType).hash_code();
			return RowChecker<StateFrom, StateTo, TargetEventType, EventType>::GetNextState(i_event, i_fsm, caller_type);
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
			static size_t this_index = typeid(*this).hash_code();
			if (io_result)
				return std::make_pair(this_index, ExecFunction());
			auto res = row::GetNextState<EventType, StateMachine>(i_event, i_fsm);
			io_result = static_cast<bool>(res.second);
			return res;
		}

		template <typename EventType, typename StateMachine>
		TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			bool res = false;
			TransitionGetterResult tr_results[] = { CheckTransition(i_event, i_fsm, res, Transitions())... };

			for (auto& result : tr_results)
			{
				if (result.second)
					return result;
			}
			static size_t this_index = typeid(*this).hash_code();
			return std::make_pair(this_index, ExecFunction());
		}

	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct TransitionRow
	{
		using _ThisType = TransitionRow<StateFrom, StateTo, TargetEventType>;
		template <typename EventType, typename StateMachine>
		static TransitionGetterResult GetNextState(const EventType& i_event, const StateMachine& i_fsm)
		{
			static size_t caller_type = typeid(_ThisType).hash_code();
			return RowChecker<StateFrom, StateTo, TargetEventType, EventType>::GetNextState(i_event, i_fsm, caller_type);
		}
	};

	
	template <typename StateFrom, typename StateTo, typename TargetEventType>
	using _row = TransitionRow<StateFrom, StateTo, TargetEventType>;

} // SDK

#endif