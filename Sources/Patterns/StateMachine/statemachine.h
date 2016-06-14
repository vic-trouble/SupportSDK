#ifndef	__PATTERNS_STATEMACHINE_H__
#define __PATTERNS_STATEMACHINE_H__

#include "BaseState.h"
#include "Transitions.h"

namespace SDK
{
	namespace details
	{
		struct Visitor
		{
			size_t m_state_hash;
			size_t m_ev_hash;

			Visitor(size_t i_state, size_t i_ev_hash)
				: m_state_hash(i_state)
				, m_ev_hash(i_ev_hash)
			{}
			inline bool Compare(size_t i_state, size_t i_event) const
			{
				return m_state_hash == i_state && m_ev_hash == i_event;
			}
			virtual void Execute() = 0;
		};

		template <typename State, typename Ev>
		struct VisitorImpl : Visitor
		{
			State& m_state;
			Ev m_cached_ev;

			VisitorImpl(size_t i_state_hash, size_t i_ev_hash, State& i_state, const Ev& i_event)
				: Visitor(i_state_hash, i_ev_hash)
				, m_state(i_state)
				, m_cached_ev(i_event)
			{}

			virtual void Execute() override
			{
				m_state.OnEnter(m_cached_ev);
			}
		};

		template <typename State>
		struct VisitorVoidImpl : Visitor
		{
			State& m_state;

			VisitorVoidImpl(size_t i_state_hash, State& i_state)
				: Visitor(i_state_hash, 0)
				, m_state(i_state)
			{}
			virtual void Execute() override
			{
				m_state.OnEnter();
			}
		};

		using VisitorPtr = std::unique_ptr<Visitor>;
		using Visitors = std::vector<VisitorPtr>;
	} // details


	template <
		size_t StatesCount,
		typename TransitionTable,
		typename FirstStateType,
		typename OnUpdateParam = float,
		typename BaseStateType = BaseState<OnUpdateParam>,
		class PtrType = std::unique_ptr<BaseStateType>	
	>
	class StateMachine
	{
	public:
		constexpr static size_t _StatesCount = StatesCount;
		static_assert(_StatesCount > 0, "Size of states must be greater than 0");
		using _ThisMachine = typename StateMachine<StatesCount, TransitionTable, FirstStateType, OnUpdateParam, BaseStateType, PtrType>;
		using _FirstState = FirstStateType;
		using _BaseStateType = BaseStateType;
		using _OnUpdateParam = OnUpdateParam;
		static constexpr int INVALID_EXECUTOR_INDEX = -1;

	private:
		constexpr static size_t NullState = _StatesCount;
		constexpr static size_t NullNextState = _StatesCount + 1;

	private:
		PtrType m_states[_StatesCount];
		size_t m_states_hashes[_StatesCount];
		TransitionTable m_transitions;
		size_t m_current;
		size_t m_next;
		size_t m_prev;

		mutable details::Visitors m_visitors;
		int m_next_executor;

	private:
		void ChangeStateIfNeeded()
		{
			if (m_next == NullNextState)
				return;

			if (m_current != NullState)
				m_states[m_current]->OnExit();

			m_prev = m_current;
			m_current = m_next;
			if (m_current != NullState && m_next_executor != INVALID_EXECUTOR_INDEX)
			{
				m_visitors[m_next_executor]->Execute();
				m_next_executor = INVALID_EXECUTOR_INDEX;
			}

			m_next = NullNextState;
		}

		template <typename State>
		void SetNext()
		{
			const size_t next_state = typeid(State).hash_code();
			SetNext(std::make_pair(
				next_state,
				CacheVisit<State>()
				));
		}

		void SetNext(TransitionGetterResult i_result)
		{
			m_next = NullNextState;
			m_next_executor = INVALID_EXECUTOR_INDEX;
			const size_t next_state = i_result.first;
			for (size_t i = 0; i < _StatesCount; ++i)
			{
				const auto& state = m_states[i];
				if (next_state == m_states_hashes[i])
				{
					m_next = i;
					m_next_executor = i_result.second;
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
			, m_next_executor(INVALID_EXECUTOR_INDEX)
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
			{
				m_states[i] = std::move(states[i]);
				m_states_hashes[i] = typeid(*m_states[i]).hash_code();
			}

			m_current = NullState;
			m_next = NullNextState;
			m_prev = NullState;

			SetNext<_FirstState>();
		}

		/////////////////////////////////////////////////////////////
		// Caching visits to states::OnEnter methods
		//	Events should have copy constructor; ideally - POD structures

		template <
			typename StateTo,
			typename TargetEventType,
			typename = std::enable_if_t<!std::is_same<TargetEventType, void>::value>
		>
		int CacheVisit(const TargetEventType& i_event) const
		{
			static const size_t type_to = typeid(StateTo).hash_code();
			static const size_t ev_hash = typeid(TargetEventType).hash_code();
			static void(StateTo::*enter_func)(const TargetEventType&) = &StateTo::OnEnter;

			for (int i = 0; i < static_cast<int>(m_visitors.size()); ++i)
			{
				if (m_visitors[i]->Compare(type_to, ev_hash))
				{
					static_cast<details::VisitorImpl<StateTo, TargetEventType>*>(m_visitors[i].get())->m_cached_ev = i_event;
					return i;
				}
			}

			m_visitors.emplace_back(new details::VisitorImpl<StateTo, TargetEventType>(type_to, ev_hash, *GetState<StateTo>(), i_event));

			return static_cast<int>(m_visitors.size() - 1);
		}

		template <typename StateTo>
		int CacheVisit() const
		{
			static size_t type_to = typeid(StateTo).hash_code();
			static void(StateTo::*enter_func)() = &StateTo::OnEnter;
			static size_t func_hash = typeid(enter_func).hash_code();
			for (int i = 0; i < static_cast<int>(m_visitors.size()); ++i)
			{
				if (m_visitors[i]->Compare(type_to, 0))
					return i;
			}

			m_visitors.emplace_back(new details::VisitorVoidImpl<StateTo>(type_to, *GetState<StateTo>()));

			return static_cast<int>(m_visitors.size() - 1);
		}

		/////////////////////////////////////////////////////////////
		// Switching between states
		
		template <typename EventType>
		void ProcessEvent(const EventType& i_evt)
		{
			TransitionGetterResult result(0, INVALID_EXECUTOR_INDEX);
			m_transitions.GetNextState<EventType, _ThisMachine>(result, i_evt, *this);
			if (result.second != INVALID_EXECUTOR_INDEX)
				SetNext(result);
		}

		template <typename State>
		void Start()
		{
			const size_t next_state = typeid(State).hash_code();
			SetNext(std::make_pair(
				next_state,
				CacheVisit<State>()
				));
		}

		template <typename State, typename Event>
		void Start(const Event& i_event)
		{
			static size_t type_to = typeid(State).hash_code();
			static void(State::*enter_func)(const Event&) = &State::OnEnter;
			SetNext(std::make_pair(
				type_to,
				CacheVisit<State, Event>(i_event)
				));
		}

		void Stop()
		{
			m_next = NullState;
			m_next_executor = INVALID_EXECUTOR_INDEX;
		}

		/////////////////////////////////////////////////////////////
		// Accessors

		size_t GetPrev() const { return m_prev; }
		size_t GetCurrent() const { return m_current; }

		template <typename State>
		bool IsStateCurrent() const
		{
			return IsStateCurrent(typeid(State).hash_code());
		}
		template <typename State>
		bool IsStatePrevious() const
		{
			IsStatePrevious(typeid(State).hash_code());
		}

		inline bool IsStateCurrent(size_t i_type_hash) const
		{
			if (m_current == NullState)
				return false;
			return m_states_hashes[m_current] == i_type_hash;
		}

		inline bool IsStatePrevious(size_t i_type_hash) const
		{
			if (m_prev == NullState)
				return false;
			return m_states_hashes[m_prev] == i_type_hash;
		}

		template <typename State>
		State* GetState() const
		{
			static const size_t next_state = typeid(State).hash_code();
			for (size_t i = 0; i < _StatesCount; ++i)
			{
				const auto& state = m_states[i];
				if (m_states_hashes[i] == next_state)
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

	/////////////////////////////////////////////////////////////////////////
	// Helper classes

	template <
		size_t StatesCount,
		typename TransitionTable,
		typename FirstStateType,
		typename OnUpdateParam = float,
		typename BaseStateType = BaseState<OnUpdateParam>,
		class PtrType = std::unique_ptr<BaseStateType>
	>
	class CompoundState : public BaseStateType
	{
	protected:
		using _InternalFSM = StateMachine<StatesCount, TransitionTable, FirstStateType, OnUpdateParam, BaseStateType, PtrType>;
		_InternalFSM m_fsm;

	public:
		template <typename... Ptrs>
		CompoundState(Ptrs... i_states)
		{
			m_fsm.SetStates(std::move(i_states)...);
		}

		CompoundState()
		{}

		template <typename EventType>
		void ProcessEvent(const EventType& i_evt)
		{
			m_fsm.ProcessEvent<EventType>(i_evt);
		}

		virtual void OnUpdate(OnUpdateParam i_dt) override
		{
			m_fsm.OnUpdate(i_dt);
		}

		template <typename Event>
		void OnEnter(const Event& i_evt)
		{
			m_fsm.Start<FirstStateType>(i_evt);
		}

		void OnEnter()
		{
			m_fsm.Start<FirstStateType>();
		}

		virtual void OnExit() override
		{
			m_fsm.Stop();
		}
	};

	template <typename InternalFSM>
	class StateMachineWrapper : public InternalFSM::_BaseStateType
	{
	private:
		InternalFSM m_fsm;
	public:
		template <typename EventType>
		void ProcessEvent(const EventType& i_evt)
		{
			m_fsm.ProcessEvent<EventType>(i_evt);
		}

		virtual void OnUpdate(typename InternalFSM::_OnUpdateParam i_dt) override
		{
			m_fsm.OnUpdate(i_dt);
		}

		template <typename Event>
		void OnEnter(const Event& i_evt)
		{
			m_fsm.Start<InternalFSM::_FirstState>(i_evt);
		}

		void OnEnter()
		{
			m_fsm.Start<InternalFSM::_FirstState>();
		}

		virtual void OnExit() override
		{
			m_fsm.Stop();
		}
	};

} // SDK


#endif