#include "stdafx.h"

#include "FSM_Test_HierarchicalvsTemplate.h"

#include <Patterns/StateMachine/StateMachine.h>

#include <Utilities/lexical_cast.h>

namespace Hierarchical
{
	struct Event
	{
		// do not forget about virtual destructor
		virtual ~Event() {}

		// method for coping events
		virtual std::unique_ptr<Event> Copy() const { return nullptr; }
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

	class StateMachine
	{
	private:
		std::vector<std::unique_ptr<BaseState>> m_states;

		BaseState* mp_current;
		BaseState* mp_next;

		bool m_terminate;

		std::vector<Transition> m_transitions;
		std::unique_ptr<Event> mp_cashed_event;

	private:
		void CheckNextStateTransition()
		{
			if (mp_next == nullptr && !m_terminate)
				return;

			if (mp_current != nullptr)
				mp_current->OnExit();
			mp_current = mp_next;			

			if (mp_cashed_event)
				mp_current->OnEnter(*mp_cashed_event);
			else
			{
				Event base_event;
				mp_current->OnEnter(base_event);
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
			, m_terminate(false)
		{}
		StateMachine(StateMachine&& other)
			: m_states(std::move(other.m_states))
			, mp_current(other.mp_current)
			, mp_next(other.mp_next)
			, m_transitions(std::move(other.m_transitions))
			, mp_cashed_event(std::move(other.mp_cashed_event))
			, m_terminate(false)
		{
		}
		virtual ~StateMachine() {}

		template<typename State>
		void Start()
		{
			mp_next = FindState(typeid(State).hash_code());
			if (mp_next)
				mp_cashed_event.reset(new Event());
			m_terminate = mp_next == nullptr;
		}

		template<typename State>
		void Start(const Event& i_evt)
		{
			mp_next = FindState(typeid(State).hash_code());
			if (mp_next)
				mp_cashed_event = std::move(i_evt.Copy());
			m_terminate = mp_next == nullptr;
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

		virtual void OnUpdate(float dt)
		{
			CheckNextStateTransition();
			if (mp_current)
				mp_current->OnUpdate(dt);
		}

		virtual void OnExit()
		{
			mp_next = nullptr;
			m_terminate = true;
		}

		template <typename State>
		bool IsCurrentState() const
		{
			if (mp_current == nullptr)
				return false;
			return typeid(State).hash_code() == typeid(*mp_current).hash_code();
		}
	};

	// enters in some state from SetInternalMachin
	//	and continue from place of exit;
	// Tag is for identification of different compound states in FSM (as we use typeid)
	template <typename Tag>
	struct ContinuesCompoundState : public BaseState
	{
		StateMachine m_internal_fsm;

		ContinuesCompoundState(void(*SetInternalMachine)(StateMachine&))
		{
			SetInternalMachine(m_internal_fsm);
		}

		ContinuesCompoundState(std::function<void(StateMachine&)> SetInternalMachine)
		{
			SetInternalMachine(m_internal_fsm);
		}

		virtual void OnUpdate(float dt) override
		{
			m_internal_fsm.OnUpdate(dt);
		}
	};

	// breaks OnExit and continue from scratch
	// Tag is for identification of different compound states in FSM (as we use typeid)
	template <typename Tag, typename FirstState>
	struct CompoundState : public BaseState
	{
		StateMachine m_internal_fsm;

		CompoundState(void(*SetInternalMachine)(StateMachine&))
		{
			SetInternalMachine(m_internal_fsm);
		}
		
		CompoundState(std::function<void(StateMachine&)> SetInternalMachine)
		{
			SetInternalMachine(m_internal_fsm);
		}

		virtual void OnEnter(const Event& i_evt) override
		{
			/*struct SpecificEvent : public Event
			{
				virtual std::unique_ptr<Event> Copy() const override
				{
					return std::make_unique<SpecificEvent>();
				}
			};
			SpecificEvent ev;*/
			m_internal_fsm.Start<FirstState>(i_evt);
		}

		virtual void OnUpdate(float dt) override
		{
			m_internal_fsm.OnUpdate(dt);
		}

		virtual void OnExit() override
		{
			// if we want to start from scratch on evert entry to state
			//	then OnExit; if no - call nothing;
			//	when OnUpdate will call next time, fsm will continue from prev place
			m_internal_fsm.OnExit();
		}
	};

} // Hiararchical

namespace HierarchicalSample
{
	using namespace Hierarchical;	

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

	struct Idle : public BaseState
	{
		StateMachine m_internal_fsm;

		Idle()
		{
			m_internal_fsm.AddState(std::make_unique<Wait>(m_internal_fsm));
			m_internal_fsm.AddState(std::make_unique<Dance>(m_internal_fsm));
			m_internal_fsm.AddState(std::make_unique<Joke>(m_internal_fsm));
			m_internal_fsm.SetTransitions({
				_row<Wait, Dance, idle_action_completed>(),
				_row<Dance, Joke, idle_action_completed>(),
				_row<Joke, Wait, idle_action_completed>(),
			});
		}

		virtual void OnEnter(const Event&) override
		{
			struct SpecificEvent : public Event
			{
				virtual std::unique_ptr<Event> Copy() const override
				{
					return std::make_unique<SpecificEvent>();
				}
			};
			SpecificEvent ev;
			m_internal_fsm.Start<Wait>(ev);
			// another overload is Start(void)
		}

		virtual void OnUpdate(float dt) override
		{
			m_internal_fsm.OnUpdate(dt);
		}

		virtual void OnExit() override
		{
			// if we want to start from scratch on evert entry to state
			//	then OnExit; if no - call nothing;
			//	when OnUpdate will call next time, fsm will continue from prev place
			m_internal_fsm.OnExit();
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
			if (const walk_away* p_ev = dynamic_cast<const walk_away*>(&i_ev))
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

	void IdleFsm(StateMachine& o_fsm)
	{
		o_fsm.AddState(std::make_unique<Wait>(o_fsm));
		o_fsm.AddState(std::make_unique<Dance>(o_fsm));
		o_fsm.AddState(std::make_unique<Joke>(o_fsm));
		o_fsm.SetTransitions({
			_row<Wait, Dance, idle_action_completed>(),
			_row<Dance, Joke, idle_action_completed>(),
			_row<Joke, Wait, idle_action_completed>(),
		});
		o_fsm.Start<Wait>();
	}

	struct IdleTag {};
	// if use this than each OnEntry will start from Wait
	using IdleCompound = CompoundState<IdleTag, Wait>;
	// if use this than each OnEntry will continue from State in which machine was before
	//	switching to another state
	using IdleCompoundCont = ContinuesCompoundState<IdleTag>;

	using IntFSM = IdleCompoundCont;

	void CharacterFSM(StateMachine& o_fsm)
	{
		o_fsm.AddState(std::make_unique<IntFSM>(&IdleFsm));
		o_fsm.AddState(std::make_unique<Walk>(o_fsm));
		o_fsm.AddState(std::make_unique<Run>(o_fsm));
		o_fsm.AddState(std::make_unique<Attack>(o_fsm));

		o_fsm.SetTransitions({
			_row<IntFSM, Walk, walk_away>(),
			_row<IntFSM, Run, run_away>(),
			_row<IntFSM, Attack, attack>(),

			_row<Walk, IntFSM, position_achieved>(),
			_row<Walk, Run, run_away>(),
			_row<Walk, Attack, attack>(),
			_row<Walk, IntFSM, stop>(),

			_row<Run, IntFSM, position_achieved>(),
			_row<Run, Walk, walk_away>(),
			_row<Run, Attack, attack>(),
			_row<Run, IntFSM, stop>(),

			_row<Attack, IntFSM, stop_attacking>(),
			_row<Attack, Walk, walk_away>(),
			_row<Attack, Run, run_away>(),
			_row<Attack, IntFSM, stop>(),
		});
	}

	void Test()
	{
		StateMachine char_fsm;
		CharacterFSM(char_fsm);
		char_fsm.Start<IntFSM>();

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
		while (!char_fsm.IsCurrentState<IntFSM>()) char_fsm.OnUpdate(.1f);
		// attack
		char_fsm.ProcessEvent(std::make_unique<attack>(1));
		char_fsm.OnUpdate(.1f);
		// when run
		char_fsm.ProcessEvent(std::make_unique<run_away>(std::make_pair(1, 1)));
		char_fsm.OnUpdate(.1f);
	}
	//////////////////////////////////////////////////////////
	struct test_event : public Event {};

	struct First : public BaseState {};
	struct Second : public BaseState {};
	struct Third : public BaseState {};

	void TestEventsFSM(StateMachine& o_fsm)
	{
		o_fsm.AddState(std::make_unique<First>());
		o_fsm.AddState(std::make_unique<Second>());
		o_fsm.AddState(std::make_unique<Third>());
		o_fsm.SetTransitions({
			_row<First, Second, test_event>(),
			_row<Second, Third, test_event>(),
			_row<Third, First, test_event>()
		});
	}

	void TestEvents(int count)
	{
		StateMachine fsm;
		TestEventsFSM(fsm);
		fsm.Start<First>();
		for (int i = 0; i < count; ++i)
		{
			fsm.OnUpdate(0.1f);
			fsm.ProcessEvent(std::make_unique<test_event>());
		}
	}

}

namespace TemplateSample
{
	using namespace SDK;
	// events
	struct idle_action_completed {};
	struct stop {};
	struct walk_away
	{
		std::pair<int, int> position = { 0, 0 };
		walk_away(std::pair<int, int> i_pos) : position(i_pos) {}
	};
	struct run_away
	{
		std::pair<int, int> position = { 0, 0 };
		run_away(std::pair<int, int> i_pos) : position(i_pos) {}
	};
	struct position_achieved {};
	struct attack
	{
		int enemy_id = 0;
		attack(int i_id) : enemy_id(i_id) {}
	};
	struct stop_attacking {};


	// idle states
	struct Wait;
	struct Dance;
	struct Joke;
	using Tr = TransitionsTable<
		_row<Wait, Dance, idle_action_completed>,
		_row<Dance, Joke, idle_action_completed>,
		_row<Joke, Wait, idle_action_completed>
	>;
	struct Idle : public StateMachine<Idle, BaseState<>, 3, Tr, Wait>
	{
		Idle();
		void OnEnter()
		{

		}
		DEFINE_BASE_FUNCTIONS();
	};

	using IntFSM = Idle;

	struct Dance : public BaseState<>
	{
		Idle& m_parent_fsm;
		Dance(Idle& i_fsm) : m_parent_fsm(i_fsm) {}
		virtual void OnUpdate(float dt) override
		{
			std::cout << "\t[Dance] OnUpdate" << std::endl;
			m_parent_fsm.ProcessEvent(idle_action_completed());
		}
	};

	struct Joke : public BaseState<>
	{
		Idle& m_parent_fsm;
		Joke(Idle& i_fsm) : m_parent_fsm(i_fsm) {}
		virtual void OnUpdate(float dt) override
		{
			std::cout << "\t[Joke] OnUpdate" << std::endl;
			m_parent_fsm.ProcessEvent(idle_action_completed());
		}
	};

	struct Wait : public BaseState<>
	{
		Idle& m_parent_fsm;
		Wait(Idle& i_fsm) : m_parent_fsm(i_fsm) {}
		
		virtual void OnUpdate(float dt) override
		{
			std::cout << "\t[Wait] OnUpdate" << std::endl;
			m_parent_fsm.ProcessEvent(idle_action_completed());
		}
	};
	
	Idle::Idle()
	{
		SetStates(
			std::make_unique<Wait>(*this),
			std::make_unique<Dance>(*this),
			std::make_unique<Joke>(*this)
			);
	}

	// separate states

	template <typename StateMachine>
	struct Walk_ : public BaseState<>
	{
		StateMachine& m_parent_fsm;
		size_t m_steps_count;

		std::pair<int, int> m_pos;
		bool m_arrived;

		Walk_(StateMachine& i_fsm)
			: m_parent_fsm(i_fsm)
			, m_steps_count(0)
		{}
		DEFINE_BASE_FUNCTIONS();
		void OnEnter(const walk_away& i_ev)
		{
			std::cout << "[Walk] OnEnter" << std::endl;
			// we cannot be sure that event is walk_away, so additional check needed
			m_pos = i_ev.position;
			m_arrived = false;
			m_steps_count = 0;
		}

		virtual void OnUpdate(float dt) override
		{
			std::cout << "[Walk] OnUpdate; steps: " << m_steps_count << std::endl;
			if (m_arrived)
				m_parent_fsm.ProcessEvent(position_achieved());
			// we are not worry about how we arrive at position
			++m_steps_count;
			m_arrived = m_steps_count > 3;

		}
	};

	// same as walk state - but for transition map they should have different types;
	//		solution: 1. Inheritance: MoveState (has all functionality) - two children: Run; Walk
	//				  2. Template with partially specialization: Move<MoveType::Walk>; Move<MoveType::Run>, where MoveType is enum class
	// Guess what I would prefer
	template <typename StateMachine>
	struct Run_ : public BaseState<>
	{
		StateMachine& m_parent_fsm;
		size_t m_steps_count;

		std::pair<int, int> m_pos;
		bool m_arrived;

		Run_(StateMachine& i_fsm)
			: m_parent_fsm(i_fsm)
			, m_steps_count(0)
		{}

		DEFINE_BASE_FUNCTIONS();

		virtual void OnEnter(const run_away& i_ev)
		{
			std::cout << "[Run] OnEnter" << std::endl;
			// we cannot be sure that event is walk_away, so additional check needed
			m_pos = i_ev.position;
			m_arrived = false;
			m_steps_count = 0;
		}

		virtual void OnUpdate(float dt) override
		{
			std::cout << "[Run] OnUpdate; steps: " << m_steps_count << std::endl;
			if (m_arrived)
				m_parent_fsm.ProcessEvent(position_achieved());
			// we are not worry about how we arrive at position
			++m_steps_count;
			m_arrived = m_steps_count > 3;
		}
	};

	template <typename StateMachine>
	struct Attack_ : public BaseState<>
	{
		StateMachine& m_parent_fsm;
		int m_enemy_id;

		bool m_done;

		Attack_(StateMachine& i_fsm)
			: m_parent_fsm(i_fsm)
			, m_enemy_id(0)
			, m_done(false)
		{}
		DEFINE_BASE_FUNCTIONS();
		virtual void OnEnter(const attack& i_ev)
		{
			std::cout << "[Attack] OnEnter" << std::endl;
			m_enemy_id = i_ev.enemy_id;
			m_done = false;
		}

		virtual void OnUpdate(float dt) override
		{
			std::cout << "[Attack] OnUpdate" << std::endl;
			if (m_done)
				m_parent_fsm.ProcessEvent(stop_attacking());
			m_done = true;
		}
	};

	struct CharacherFSM;
	using Walk = Walk_<CharacherFSM>;
	using Run = Run_<CharacherFSM>;
	using Attack = Attack_<CharacherFSM>;

	using CharTr = TransitionsTable<
		_row<IntFSM, Walk, walk_away>,
		_row<IntFSM, Run, run_away>,
		_row<IntFSM, Attack, attack>,

		_row<Walk, IntFSM, position_achieved>,
		_row<Walk, Run, run_away>,
		_row<Walk, Attack, attack>,
		_row<Walk, IntFSM, stop>,

		_row<Run, IntFSM, position_achieved>,
		_row<Run, Walk, walk_away>,
		_row<Run, Attack, attack>,
		_row<Run, IntFSM, stop>,

		_row<Attack, IntFSM, stop_attacking>,
		_row<Attack, Walk, walk_away>,
		_row<Attack, Run, run_away>,
		_row<Attack, IntFSM, stop>
	>;

	struct CharacherFSM : public StateMachine<int, BaseState<>, 4, CharTr, IntFSM>
	{
		CharacherFSM()
		{
			SetStates(
				std::make_unique<IntFSM>(),
				std::make_unique<Walk>(*this),
				std::make_unique<Run>(*this),
				std::make_unique<Attack>(*this)
				);
		}
	};

	

	////////////////////////////////////////////////////

	void Test()
	{
		CharacherFSM char_fsm;
		// move to Wait
		char_fsm.OnUpdate(.1f);
		// move to Dance
		char_fsm.OnUpdate(.1f);
		// move to Joke
		char_fsm.OnUpdate(.1f);
		// move back to Wait
		char_fsm.OnUpdate(.1f);

		// walk away
		char_fsm.ProcessEvent(walk_away(std::make_pair(1, 1)));
		char_fsm.OnUpdate(.1f);
		// when run away
		char_fsm.ProcessEvent(run_away(std::make_pair(1, 1)));
		char_fsm.OnUpdate(.1f);
		// wait for idle state
		while (!char_fsm.IsStateCurrent<IntFSM>()) char_fsm.OnUpdate(.1f);
		// attack
		char_fsm.ProcessEvent(attack(1));
		char_fsm.OnUpdate(.1f);
		// when run
		char_fsm.ProcessEvent(run_away(std::make_pair(1, 1)));
		char_fsm.OnUpdate(.1f);
	}

	////////////////////////////////////////////////////

	struct test_event {};

	struct First : public BaseState<> {};
	struct Second : public BaseState<> {};
	struct Third : public BaseState<> {};

	using TestEvTr = TransitionsTable<
		_row<First, Second, test_event>,
		_row<Second, Third, test_event>,
		_row<Third, First, test_event>
	>;

	struct TestEventsFSM : public StateMachine<TestEventsFSM, BaseState<>, 3, TestEvTr, First>
	{
		TestEventsFSM()
		{
			SetStates(
				std::make_unique<First>(),
				std::make_unique<Second>(),
				std::make_unique<Third>()
				);
		}
	};

	void TestEvents(int count)
	{
		TestEventsFSM fsm;
		for (int i = 0; i < count; ++i)
		{
			fsm.OnUpdate(0.1f);
			fsm.ProcessEvent(test_event());
		}
	}

} // TemplateSample


namespace HiararchicalvsTemplate
{
	clock_t Runner(std::function<void(int)> func, size_t i_nums)
	{
		clock_t begin = clock();
		func(i_nums);
		return clock() - begin;
	}
	void Test()
	{
		HierarchicalSample::Test();
		std::cout << "-----------------------------------" << std::endl;
		TemplateSample::Test();
		std::cout << "-----------------------------------" << std::endl;

		constexpr static size_t CALL_COUNT = 100000;
		const clock_t hier_t = Runner(&HierarchicalSample::TestEvents, CALL_COUNT);
		const clock_t template_t = Runner(&TemplateSample::TestEvents, CALL_COUNT);

		std::cout << "\tHierarchical: " << hier_t << std::endl
			<< "\tTemplate: " << template_t << std::endl;
	}
} // HiararchicalvsTemplate