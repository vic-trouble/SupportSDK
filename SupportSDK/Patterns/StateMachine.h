#ifndef	__PATTERNS_STATEMACHINE_H__
#define __PATTERNS_STATEMACHINE_H__

#include "Event.h"

#include <Utilities/type_index.h>

namespace SDK
{
	template <typename OnUdateParam = float>
	class BaseState
	{
	public:
		virtual ~BaseState() {}
		template <typename EventType>
		void OnEnter(const EventType&) {}
		virtual void OnExit() {}
		virtual void OnUpdate(OnUdateParam i_elapsed_time) {}
	};
	
	struct ExBase
	{
		const std::type_index m_handler_type;
		ExBase(const std::type_index& i_handler_type)
			: m_handler_type(i_handler_type)
		{}
		virtual void Execute() = 0;
	};

	template <typename EventType, typename Handler>
	struct Executor : public ExBase
	{
		EventType m_cached;
		Handler* mp_handler;
		Executor(EventType cached, Handler* ip_handler)
			: ExBase(typeid(Handler))
			, m_cached(cached)
			, mp_handler(ip_handler)
		{}
		virtual void Execute() override
		{
			mp_handler->OnEnter(m_cached);
		}
	};

	typedef std::unique_ptr<ExBase> ExecutorPtr;
	typedef std::type_index NextStateType;
	typedef std::pair<NextStateType, ExecutorPtr> TransitionGetterResult;
	template <typename StateMachine>
	struct NoTransition
	{
		std::type_index GetNextState() const
		{
			return std::type_index(typeid(*this));
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
	
	template <typename Derived,
		typename StateBaseType,
		size_t StatesCount,
		typename TransitionTable,
		typename PtrType,
		//typename PtrType = std::unique_ptr<StateBaseType>::_Myt,
		typename OnUpdateParam = float
	>
	class StateMachine
	{
		constexpr static size_t _StatesCount = StatesCount;
		static_assert(_StatesCount > 0, "Size of states must be greater than 0");
		constexpr static size_t NullState = _StatesCount;
		constexpr static size_t NullNextState = _StatesCount + 1;		
	public:
		typedef typename StateMachine<Derived, StateBaseType, StatesCount, TransitionTable, PtrType, OnUpdateParam> ThisMachine;

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
		}

		/////////////////////////////////////////////////////////////
		// Switching between states
		
		template <typename EventType>
		void ProcessEvent(const EventType& i_evt)
		{
			auto result = std::move(m_transitions.GetNextState<EventType, ThisMachine>(i_evt, dynamic_cast<Derived&>(*this)));
			if (std::type_index(typeid(m_transitions)) != result.first)
				SetNext(std::move(result));
		}

		template <typename State>
		void SetNext()
		{
			const std::type_info& next_state = typeid(State);
			SetNext(std::make_pair(std::type_index(typeid(State)), nullptr));
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

		void OnEnter(const Event& i_event)
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