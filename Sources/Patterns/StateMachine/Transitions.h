#ifndef	__PATTERNS_STATEMACHINE_TRANSITIONS_H__
#define __PATTERNS_STATEMACHINE_TRANSITIONS_H__

// disable warning about name truncating;
// The correctness of the program, however, is unaffected by the truncated name. And hopefully you will not debug this code
// https://msdn.microsoft.com/query/dev14.query?appId=Dev14IDEF1&l=EN-US&k=k(C4503)&rd=true
#pragma warning (disable : 4503)

namespace SDK
{
	typedef size_t NextStateType;
	typedef std::pair<NextStateType, int> TransitionGetterResult;
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
		static void GetNextState(TransitionGetterResult& o_result, const TargetEventType& i_event, const StateMachine& i_fsm, const size_t i_caller_type)
		{
			static size_t type_to = typeid(StateTo).hash_code();
			static size_t type_from = typeid(StateFrom).hash_code();
			static void(StateTo::*enter_func)(const TargetEventType&) = &StateTo::OnEnter;
			
			if (i_fsm.IsStateCurrent(type_from))
			{
				o_result.first = type_to;
				o_result.second = i_fsm.CacheVisit<StateTo, TargetEventType>(i_event);// std::bind(enter_func, i_fsm.GetState<StateTo>(), static_cast<const TargetEventType&>(i_event));
			}
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType, typename Event>
	struct RowChecker <StateFrom, StateTo, TargetEventType, Event, typename std::enable_if<!std::is_same<TargetEventType, Event>::value>::type>
	{
		template <typename StateMachine>
		static void GetNextState(TransitionGetterResult& o_result, const Event& i_event, const StateMachine& i_fsm, const size_t i_caller_type)
		{
		}
	};

	////////////////////////////////////////////////////////////////////
	// Compound transitions

	template <typename FirstTransition, typename SecondTransition>
	struct CompoundTransition
	{
		FirstTransition first;
		SecondTransition second;

		template <typename EventType, typename StateMachine>
		void GetNextState(TransitionGetterResult& o_result, const EventType& i_event, const StateMachine& i_fsm)
		{
			static const size_t first_type = typeid(first).hash_code();
			static const size_t second_type = typeid(second).hash_code();
			static const size_t this_type = typeid(*this).hash_code();

			first.GetNextState<EventType, StateMachine>(o_result, i_event, i_fsm);
			if (o_result.first != first_type)
				return;
			second.GetNextState<EventType, StateMachine>(o_result, i_event, i_fsm);
			if (o_result.first != second_type)
				return;

			o_result.first = this_type;
			o_result.second = StateMachine::INVALID_EXECUTOR_INDEX;
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct Transition
	{
		using _ThisType = Transition<StateFrom, StateTo, TargetEventType>;
		template <typename EventType, typename StateMachine>
		void GetNextState(TransitionGetterResult& o_result, const EventType& i_event, const StateMachine& i_fsm) const
		{
			static size_t caller_type = typeid(_ThisType).hash_code();
			RowChecker<StateFrom, StateTo, TargetEventType, EventType>::GetNextState(o_result, i_event, i_fsm, caller_type);
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
		bool CheckTransition(TransitionGetterResult& o_result, const EventType& i_event, const StateMachine& i_fsm)
		{
			if (o_result.second != StateMachine::INVALID_EXECUTOR_INDEX)
				return false;
			row::GetNextState<EventType, StateMachine>(o_result, i_event, i_fsm);
			return true;
		}

		template <typename EventType, typename StateMachine>
		void GetNextState(TransitionGetterResult& o_result, const EventType& i_event, const StateMachine& i_fsm)
		{
			static size_t this_index = typeid(*this).hash_code();
			o_result.first = this_index;
			bool tr_results[] = { CheckTransition<EventType, StateMachine, Transitions>(o_result, i_event, i_fsm)... };
		}

		template <typename StateMachine, typename row>
		bool Cache(StateMachine& o_fsm)
		{
			row::Cache(o_fsm);
			return true;
		}

		template <typename StateMachine>
		void CacheStates(StateMachine& o_fsm)
		{
			bool res[] = { Cache<StateMachine, Transitions>(o_fsm)... };
		}
	};

	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct TransitionRow
	{
		using _ThisType = TransitionRow<StateFrom, StateTo, TargetEventType>;

		template <typename StateMachine>
		void Cache(StateMachine& o_fsm)
		{
		}

		template <typename EventType, typename StateMachine>
		static void GetNextState(TransitionGetterResult& o_result, const EventType& i_event, const StateMachine& i_fsm)
		{
			static size_t caller_type = typeid(_ThisType).hash_code();
			RowChecker<StateFrom, StateTo, TargetEventType, EventType>::GetNextState(o_result, i_event, i_fsm, caller_type);
		}
	};

	
	template <typename StateFrom, typename StateTo, typename TargetEventType>
	using _row = TransitionRow<StateFrom, StateTo, TargetEventType>;

} // SDK

#endif