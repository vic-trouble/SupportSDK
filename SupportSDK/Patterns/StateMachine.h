#ifndef	__PATTERNS_STATEMACHINE_H__
#define __PATTERNS_STATEMACHINE_H__

#include <initializer_list>

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

	template <typename PtrType, 
		size_t StatesCount,
		typename OnUpdateParam = float
	>
	class StateMachine
	{
		constexpr static size_t _StatesCount = StatesCount;
		static_assert(_StatesCount > 0, "Size of states must be greater than 0");
		constexpr static size_t NullState = _StatesCount;
		constexpr static size_t NullNextState = _StatesCount + 1;

	private:
		PtrType m_states[_StatesCount];
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

	public:
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
			m_next = NullState;

			const std::type_info& next_state = typeid(State);
			for (size_t i = 0; i < _StatesCount; ++i)
			{
				const auto& state = m_states[i];
				if (typeid(*state) == next_state)
				{
					m_next = i;
					break;
				}
			}			
		}

		size_t GetPrev() const { return m_prev; }
		size_t GetCurrent() const { return m_current; }

		template <typename State>
		bool IsStateCurrent() const
		{
			if (m_current == NullState)
				return false;
			return typeid(m_states[m_current]) == typeid(State);
		}
		template <typename State>
		bool IsStatePrevious() const
		{
			if (m_prev == NullState)
				return false;
			return typeid(m_states[m_prev]) == typeid(State);
		}

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