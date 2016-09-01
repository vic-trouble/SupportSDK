#include "stdafx.h"

#include "GameObject.h"

#include "GameContext.h"
#include "IComponent.h"
#include "IController.h"

//////////////////////////////////////////////////////////////////////////

namespace
  {

  struct ComponentIDFunctor
    {
    private:
      int m_id;
    public:
      ComponentIDFunctor (int i_id)
        : m_id(i_id)
        {        }

      bool operator () (const GameObject::ComponentUniquePtr& ip_component)
        {
        return m_id == ip_component->GetID();
        }
    };

  } // namespace

//////////////////////////////////////////////////////////////////////////

GameObject::GameObject(long i_id, int i_type, GameContext& i_context, IController* ip_owner)
  : m_id(i_id)
  , m_type(i_type)
  , m_context(i_context)
  , mp_owner(ip_owner)
  , m_has_changes(true)
  , m_selected(false)
  , m_destroyed(false)
  {  }

GameObject::~GameObject()
  {  }

void GameObject::Tick(long i_ms_for_tick)
  {
  TickComponents(i_ms_for_tick);
  }

void GameObject::TickComponents(long i_ms_for_tick)
  {
  for(ComponentUniquePtr& p_component : m_components)
    p_component->Tick(i_ms_for_tick);
  }

void GameObject::AddComponent(ComponentUniquePtr ip_component)
  {
  m_components.push_back(std::move(ip_component));
  }

bool GameObject::ProbeComponents()
  {
  for(ComponentUniquePtr& p_component : m_components)
    {
    if (!p_component->Probe())
      return false;
    }
  return true;
  }

bool GameObject::HasComponent(int i_component_id) const
  {
  return std::find_if(m_components.begin(), m_components.end(), ComponentIDFunctor(i_component_id)) != m_components.end();
  }

IComponent* GameObject::GetComponent(long i_component_id) const
  {
  auto it_component = std::find_if(m_components.begin(), m_components.end(), ComponentIDFunctor(i_component_id));
  if (it_component != m_components.end())
    return (*it_component).get();

  return nullptr;
  }