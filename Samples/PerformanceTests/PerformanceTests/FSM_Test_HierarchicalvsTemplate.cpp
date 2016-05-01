#include "stdafx.h"

#include "FSM_Test_HierarchicalvsTemplate.h"

#include <Patterns/StateMachine/StateMachine.h>

#include <Utilities/lexical_cast.h>

namespace Hiararchical
{
	struct Event
	{
		// do not forget about virtual destructor
		virtual ~Event() {}
	};
	class BaseState
	{
	public:
		virtual ~BaseState() {}

		virtual void OnEnter(const Event&) {}
		virtual void OnExit() {}

		virtual void OnUpdate(float dt) {}
	};

	struct Transition
	{
		size_t m_hash_from;
		size_t m_hash_to;
		size_t m_hash_event;
	};

	template <typename StateFrom, typename StateTo, typename Event>
	Transition _row()
	{
		return{ typeid(StateFrom).hash_code(), typeid(StateTo).hash_code(), typeid(Event).hash_code() };
	}

	class StateMachine : public BaseState
	{
	private:
		std::vector<std::unique_ptr<BaseState>> m_states;

		BaseState* mp_current;
		BaseState* mp_next;

		std::vector<Transition> m_transitions;
		std::unique_ptr<Event> mp_cashed_event;

	private:
		void CheckNextStateTransition()
		{
			if (mp_next == nullptr)
				return;
			if (mp_current != nullptr)
				mp_current->OnExit();
			mp_current = mp_next;
			if (mp_cashed_event)
				mp_next->OnEnter(*mp_cashed_event);
			else
			{
				Event base_event;
				mp_next->OnEnter(base_event);
			}
			mp_next = nullptr;
			mp_cashed_event.reset();
		}

		BaseState* FindState(size_t i_hash)
		{
			auto it = std::find_if(m_states.begin(), m_states.end(), [i_hash](std::unique_ptr<BaseState>& ip_state)
			{
				return i_hash == typeid(*ip_state).hash_code();
			});

			if (it != m_states.end())
				return it->get();
			return nullptr;
		}

	public:
		StateMachine()
			: mp_current(nullptr)
			, mp_next(nullptr)
			, mp_cashed_event(nullptr)
		{}
		virtual ~StateMachine() {}

		template<typename State>
		void SetFirst()
		{
			mp_next = FindState(typeid(State).hash_code());
		}

		void AddState(std::unique_ptr<BaseState> ip_state)
		{
			m_states.emplace_back(std::move(ip_state));
		}

		void SetStates(std::vector<std::unique_ptr<BaseState>> ip_states)
		{
			for (auto it = ip_states.begin(); it != ip_states.end(); ++it)
			{
				//std::unique_ptr<BaseState> p_state(it->release());
				//m_states.push_back(std::move(p_state));
			}
		}

		void AddTransition(Transition i_trans)
		{
			m_transitions.push_back(i_trans);
		}

		void SetTransitions(std::initializer_list<Transition> transitions)
		{
			std::copy(transitions.begin(), transitions.end(), std::back_inserter(m_transitions));
		}

		template <typename State>
		void SetNext()
		{
			mp_next = FindState(typeid(State).hash_code());
		}

		void ProcessEvent(std::unique_ptr<Event> ip_event)
		{
			if (mp_current == nullptr)
				return;
			const size_t cur_hash = typeid(*mp_current).hash_code();
			const size_t ev_hash = typeid(*ip_event).hash_code();
			for (auto& transition : m_transitions)
			{
				if (cur_hash == transition.m_hash_from && ev_hash == transition.m_hash_event)
				{
					auto p_next = FindState(transition.m_hash_to);
					if (p_next)
					{
						mp_next = p_next;
						mp_cashed_event = std::move(ip_event);
					}
					break;
				}
			}
		}

		virtual void OnUpdate(float dt) override
		{
			CheckNextStateTransition();
			if (mp_current)
				mp_current->OnUpdate(dt);
		}

		template <typename State>
		bool IsCurrentState() const
		{
			if (mp_current == nullptr)
				return false;
			return typeid(State).hash_code() == typeid(*mp_current).hash_code();
		}
	};
} // Hiararchical

namespace HiararchicalSample
{
	using namespace Hiararchical;	

	// events
	struct idle_action_completed : public Event {};
	struct stop : public Event {};
	struct walk_away : public Event
	{
		std::pair<int, int> position = { 0, 0 };
		walk_away(std::pair<int, int> i_pos) : position(i_pos) {}
	};
	struct run_away : public Event
	{
		std::pair<int, int> position = { 0, 0 };
		run_away(std::pair<int, int> i_pos) : position(i_pos) {}
	};
	struct position_achieved : public Event {};
	struct attack : public Event
	{
		int enemy_id = 0;
		attack(int i_id) : enemy_id(i_id) {}
	};
	struct stop_attacking : public Event {};

	// idle states

	struct Dance : public BaseState
	{
		StateMachine& m_parent_fsm;
		Dance(StateMachine& i_fsm) : m_parent_fsm(i_fsm) {}
		virtual void OnUpdate(float dt) override
		{
			std::cout << "\t[Dance] OnUpdate" << std::endl;
			m_parent_fsm.ProcessEvent(std::make_unique<idle_action_completed>());
		}
	};

	struct Joke : public BaseState
	{
		StateMachine& m_parent_fsm;
		Joke(StateMachine& i_fsm) : m_parent_fsm(i_fsm) {}
		virtual void OnUpdate(float dt) override
		{
			std::cout << "\t[Joke] OnUpdate" << std::endl;
			m_parent_fsm.ProcessEvent(std::make_unique<idle_action_completed>());
		}
	};

	struct Wait : public BaseState
	{
		StateMachine& m_parent_fsm;
		Wait(StateMachine& i_fsm) : m_parent_fsm(i_fsm) {}
		virtual void OnUpdate(float dt) override
		{
			std::cout << "\t[Wait] OnUpdate" << std::endl;
			m_parent_fsm.ProcessEvent(std::make_unique<idle_action_completed>());
		}
	};

	struct Idle : public StateMachine
	{
		Idle()
		{
			AddState(std::make_unique<Wait>(*this));
			AddState(std::make_unique<Dance>(*this));
			AddState(std::make_unique<Joke>(*this));
			SetTransitions({
				_row<Wait, Dance, idle_action_completed>(),
				_row<Dance, Joke, idle_action_completed>(),
				_row<Joke, Wait, idle_action_completed>(),
			});
		}

		virtual void OnEnter(const Event&)
		{
			SetNext<Wait>();
		}
	};

	// separate states

	struct Walk : public BaseState
	{
		StateMachine& m_parent_fsm;
		size_t m_steps_count;

		std::pair<int, int> m_pos;
		bool m_arrived;

		Walk(StateMachine& i_fsm)
			: m_parent_fsm(i_fsm)
			, m_steps_count(0)
		{}

		virtual void OnEnter(const Event& i_ev)
		{
			std::cout << "[Walk] OnEnter" << std::endl;
			// we cannot be sure that event is walk_away, so additional check needed
			if (const walk_away* p_ev = static_cast<const walk_away*>(&i_ev))
			{
				m_pos = p_ev->position;
				m_arrived = false;
				m_steps_count = 0;
			}
			else
			{
				m_arrived = true;
			}
		}

		virtual void OnUpdate(float dt) override
		{
			std::cout << "[Walk] OnUpdate; steps: " << m_steps_count << std::endl;
			if (m_arrived)
				m_parent_fsm.ProcessEvent(std::make_unique<position_achieved>());
			// we are not worry about how we arrive at position
			++m_steps_count;
			m_arrived = m_steps_count > 3;
			
		}
	};

	// same as walk state - but for transition map they should have different types;
	//		solution: 1. Inheritance: MoveState (has all functionality) - two children: Run; Walk
	//				  2. Template with partially specialization: Move<MoveType::Walk>; Move<MoveType::Run>, where MoveType is enum class
	// Guess what I would prefer
	struct Run : public BaseState
	{
		StateMachine& m_parent_fsm;
		size_t m_steps_count;

		std::pair<int, int> m_pos;
		bool m_arrived;

		Run(StateMachine& i_fsm)
			: m_parent_fsm(i_fsm)
			, m_steps_count(0)
		{}

		virtual void OnEnter(const Event& i_ev)
		{
			std::cout << "[Run] OnEnter" << std::endl;
			// we cannot be sure that event is walk_away, so additional check needed
			if (const run_away* p_ev = static_cast<const run_away*>(&i_ev))
			{
				m_pos = p_ev->position;
				m_arrived = false;
				m_steps_count = 0;
			}
			else
			{
				m_arrived = true;
			}
		}

		virtual void OnUpdate(float dt) override
		{
			std::cout << "[Run] OnUpdate; steps: " << m_steps_count << std::endl;
			if (m_arrived)
				m_parent_fsm.ProcessEvent(std::make_unique<position_achieved>());
			// we are not worry about how we arrive at position
			++m_steps_count;
			m_arrived = m_steps_count > 3;
		}
	};

	struct Attack : public BaseState
	{
		StateMachine& m_parent_fsm;
		int m_enemy_id;

		bool m_done;

		Attack(StateMachine& i_fsm)
			: m_parent_fsm(i_fsm)
			, m_enemy_id(0)
			, m_done(false)
		{}

		virtual void OnEnter(const Event& i_ev)
		{
			std::cout << "[Attack] OnEnter" << std::endl;
			// we cannot be sure that event is walk_away, so additional check needed
			if (const attack* p_ev = static_cast<const attack*>(&i_ev))
			{
				m_enemy_id = p_ev->enemy_id;
				m_done = false;
			}
			else
			{
				m_done = true;
			}
		}

		virtual void OnUpdate(float dt) override
		{
			std::cout << "[Attack] OnUpdate" << std::endl;
			if (m_done)
				m_parent_fsm.ProcessEvent(std::make_unique<stop_attacking>());
			m_done = true;
		}
	};

	struct CharacterFSM : public StateMachine
	{
		CharacterFSM()
		{
			AddState(std::make_unique<Idle>());
			AddState(std::make_unique<Walk>(*this));
			AddState(std::make_unique<Run>(*this));
			AddState(std::make_unique<Attack>(*this));			

			SetTransitions({
				_row<Idle, Walk, walk_away>(),
				_row<Idle, Walk, run_away>(),
				_row<Idle, Attack, attack>(),

				_row<Walk, Idle, position_achieved>(),
				_row<Walk, Run, run_away>(),
				_row<Walk, Attack, attack>(),
				_row<Walk, Idle, stop>(),

				_row<Run, Idle, position_achieved>(),
				_row<Run, Walk, walk_away>(),
				_row<Run, Attack, attack>(),
				_row<Run, Idle, stop>(),

				_row<Attack, Idle, stop_attacking>(),
				_row<Attack, Walk, walk_away>(),
				_row<Attack, Run, run_away>(),
				_row<Attack, Idle, stop>(),
			});
		}
	};

	void Test()
	{
		CharacterFSM char_fsm;
		char_fsm.SetNext<Idle>();

		// move to Wait
		char_fsm.OnUpdate(.1f);
		// move to Dance
		char_fsm.OnUpdate(.1f);
		// move to Joke
		char_fsm.OnUpdate(.1f);
		// move back to Wait
		char_fsm.OnUpdate(.1f);

		// walk away
		char_fsm.ProcessEvent(std::make_unique<walk_away>(std::make_pair(1, 1)));
		char_fsm.OnUpdate(.1f);
		// when run away
		char_fsm.ProcessEvent(std::make_unique<run_away>(std::make_pair(1, 1)));
		char_fsm.OnUpdate(.1f);
		// wait for idle state
		while (!char_fsm.IsCurrentState<Idle>()) char_fsm.OnUpdate(.1f);
		// attack
		char_fsm.ProcessEvent(std::make_unique<attack>(1));
		char_fsm.OnUpdate(.1f);
		// when run
		char_fsm.ProcessEvent(std::make_unique<run_away>(std::make_pair(1, 1)));
		char_fsm.OnUpdate(.1f);
	}

}

namespace TemplateSample
{

} // TemplateSample


namespace HiararchicalvsTemplate
{
	void Test()
	{
		HiararchicalSample::Test();
	}
} // HiararchicalvsTemplate