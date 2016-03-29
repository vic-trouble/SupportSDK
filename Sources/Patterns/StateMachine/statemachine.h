#ifndef	__PATTERNS_STATEMACHINE_H__
#define __PATTERNS_STATEMACHINE_H__

#include "Executor.h"
#include "Transitions.h"
#include "BaseState.h"

#include <Utilities/type_index.h>

namespace SDK
{
	template <typename Derived,
		typename BaseStateType,
		size_t StatesCount,
		typename TransitionTable,
		typename FirstStateType,
		class PtrType = std::unique_ptr<BaseStateType>,
		typename OnUpdateParam = float
	>
	class StateMachine 
		: public BaseState<OnUpdateParam> // inheritance to use StateMachine as substate
	{
		constexpr static size_t _StatesCount = StatesCount;
		static_assert(_StatesCount > 0, "Size of states must be greater than 0");
		constexpr static size_t NullState = _StatesCount;
		constexpr static size_t NullNextState = _StatesCount + 1;		
	public:
		typedef typename StateMachine<Derived, BaseStateType, StatesCount, TransitionTable, FirstStateType, PtrType, OnUpdateParam> _ThisMachine;
		typedef typename FirstStateType _FirstState;
	private:
		PtrType m_states[_StatesCount];
		TransitionTable m_transitions;
		size_t m_current;
		size_t m_next;
		size_t m_prev;

		std::unique_ptr<ExBase> mp_next_executor;

	private:
		void ChangeStateIfNeeded()
		{
			if (m_next == NullNextState)
				return;

			if (m_current != NullState)
				m_states[m_current]->OnExit();

			m_prev = m_current;
			m_current = m_next;
			if (m_current != NullState && mp_next_executor)
			{
				mp_next_executor->Execute();
				mp_next_executor.reset();
			}

			m_next = NullNextState;
		}

		void SetNext(TransitionGetterResult i_result)
		{
			m_next = NullState;
			for (size_t i = 0; i < _StatesCount; ++i)
			{
				const auto& state = m_states[i];
				if (std::type_index(typeid(*state)) == i_result.first)
				{
					m_next = i;
					mp_next_executor = std::move(i_result.second);
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

			SetNext<_FirstState>();
		}

		/////////////////////////////////////////////////////////////
		// Switching between states
		
		template <typename EventType>
		void ProcessEvent(const EventType& i_evt)
		{
			auto result = std::move(m_transitions.GetNextState<EventType, _ThisMachine>(i_evt, dynamic_cast<Derived&>(*this)));
			if (std::type_index(typeid(m_transitions)) != result.first)
				SetNext(std::move(result));
		}

		template <typename State>
		void SetNext()
		{
			const std::type_info& next_state = typeid(State);
			SetNext(
				std::make_pair( std::type_index(typeid(State)),
				std::make_unique<VoidExecutor<State>>(GetState<State>()) )
				);
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

		template <typename State>
		State* GetState() const
		{
			const std::type_index next_state = typeid(State);
			for (size_t i = 0; i < _StatesCount; ++i)
			{
				const auto& state = m_states[i];
				if (std::type_index(typeid(*state)) == next_state)
					return static_cast<State*>(&(*state));
			}
			return nullptr;
		}

		/////////////////////////////////////////////////////////////
		// BaseState overrides

		template <typename EventType>
		void OnEnter(const EventType& i_event)
		{
			if (m_current != NullState)
				m_states[m_current]->OnEnter(i_event);
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