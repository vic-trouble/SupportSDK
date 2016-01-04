#ifndef	__PATTERNS_STATEMACHINE_H__
#define __PATTERNS_STATEMACHINE_H__

#include "Event.h"

#include <Utilities/type_index.h>

namespace SDK
{
	/*
	template <typename Type, typename PtrType>
	struct PtrGetter
	{
	static Type* Get(const PtrType&);
	};

	template <typename Type>
	struct PtrGetter <Type, std::unique_ptr<Type>>
	{
	static Type* Get(const PtrType& ip_ptr)
	{
	return ip_ptr.get();
	}
	};

	template <typename Type>
	struct PtrGetter <Type, Type*>
	{
	static Type* Get(const PtrType& ip_ptr)
	{
	return ip_ptr;
	}
	};
	*/

	template <typename OnUdateParam = float>
	class BaseState
	{
	public:
		virtual ~BaseState() {}
		virtual void OnEnter() {}
		virtual void OnExit() {}
		virtual void OnUpdate(OnUdateParam i_elapsed_time) {}
	};
	
	template <typename StateMachine>
	struct NoTransition
	{
		std::type_index GetNextState() const
		{
			return typeid(*this);
		}
	};

	template <typename FirstTransition, typename SecondTransition>
	struct CompoundTransition
	{
		FirstTransition first;
		SecondTransition second;

		template <typename EventType, typename StateMachine>
		std::type_index GetNextState(const StateMachine& i_fsm)
		{
			const std::type_index first_result = first.GetNextState<EventType, StateMachine>(i_fsm);

			if (first_result != typeid(first))
				return first_result;
			const std::type_index second_result = second.GetNextState<EventType, StateMachine>(i_fsm);
			if (second_result != typeid(second))
				return second_result;
			
			return typeid(*this);
		}
	};
	
	template <typename StateFrom, typename StateTo, typename TargetEventType>
	struct Transition
	{
		template <typename EventType, typename StateMachine>
		std::type_index GetNextState(const StateMachine& i_fsm) const
		{
			if (i_fsm.IsStateCurrent<StateFrom>() && typeid(EventType) == typeid(TargetEventType))
				return typeid(StateTo);
			return typeid(*this);
		}
	};	
	
	template <typename Derived,
		typename PtrType,
		size_t StatesCount,
		typename TransitionTable,
		typename OnUpdateParam = float
	>
	class StateMachine
	{
		constexpr static size_t _StatesCount = StatesCount;
		static_assert(_StatesCount > 0, "Size of states must be greater than 0");
		constexpr static size_t NullState = _StatesCount;
		constexpr static size_t NullNextState = _StatesCount + 1;		
	public:
		typedef typename StateMachine<Derived, PtrType, StatesCount, TransitionTable, OnUpdateParam> ThisMachine;

	private:
		PtrType m_states[_StatesCount];
		TransitionTable m_transitions;
		size_t m_current;
		size_t m_next;
		size_t m_prev;

	private:
		void ChangeStateIfNeeded()
		{
			if (m_next == NullNextState)
				return;

			if (m_current != NullState)
				m_states[m_current]->OnExit();

			m_prev = m_current;
			m_current = m_next;
			if (m_current != NullState)
				m_states[m_current]->OnEnter();

			m_next = NullNextState;
		}

		void SetNext(const std::type_index& i_next_state)
		{
			m_next = NullState;
			for (size_t i = 0; i < _StatesCount; ++i)
			{
				const auto& state = m_states[i];
				if (std::type_index(typeid(*state)) == i_next_state)
				{
					m_next = i;
					break;
				}
			}
		}

	public:
		/////////////////////////////////////////////////////////////
		// Construction

		StateMachine()
			: m_current(NullState)
			, m_next(NullNextState)
			, m_prev(NullState)
		{}
		template <typename... Ptrs>
		StateMachine(Ptrs... i_states)
			: StateMachine{}
		{
			SetStates(std::move(i_states)...);
		}
		virtual ~StateMachine() {}

		template <typename... Ptrs>
		void SetStates(Ptrs... i_states)
		{
			static_assert(sizeof...(i_states) == _StatesCount, "Size of arguments must be same as size of states");
			PtrType states[] = { std::move(i_states)... };
			for (size_t i = 0; i < _StatesCount; ++i)
				m_states[i] = std::move(states[i]);

			m_current = NullState;
			m_next = NullNextState;
			m_prev = NullState;
		}

		/////////////////////////////////////////////////////////////
		// Switching between states
		
		template <typename Event>
		void ProcessEvent()
		{
			const std::type_index& result = m_transitions.GetNextState<Event, ThisMachine>(dynamic_cast<Derived&>(*this));
			if (std::type_index(typeid(m_transitions)) != result)
				SetNext(result);
		}

		void SetNext(size_t i_next)
		{
			if (i_next >= _StatesCount)
			{
				m_next = NullState;
				return;
			}
			m_next = i_next;
		}

		template <typename State>
		void SetNext()
		{
			const std::type_info& next_state = typeid(State);
			SetNext(typeid(State));
		}

		/////////////////////////////////////////////////////////////
		// Accessors

		size_t GetPrev() const { return m_prev; }
		size_t GetCurrent() const { return m_current; }

		template <typename State>
		bool IsCurrent() const
		{
			if (m_current == NullState)
				return false;
			return typeid(m_states[m_current]) == typeid(State);
		}

		template <typename State>
		bool IsStateCurrent() const
		{
			if (m_current == NullState)
				return false;
			return typeid(*m_states[m_current]) == typeid(State);
		}
		template <typename State>
		bool IsStatePrevious() const
		{
			if (m_prev == NullState)
				return false;
			return typeid(*m_states[m_prev]) == typeid(State);
		}

		/////////////////////////////////////////////////////////////
		// BaseState overrides

		void OnEnter()
		{
			if (m_current != NullState)
				m_states[m_current]->OnEnter();
		}

		void OnExit()
		{
			if (m_current != NullState)
				m_states[m_current]->OnExit();
		}

		void OnUpdate(OnUpdateParam i_param)
		{
			ChangeStateIfNeeded();
			if (m_current != NullState)
				m_states[m_current]->OnUpdate(i_param);
		}
	};

} // SDK


#endif