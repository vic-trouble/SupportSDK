#pragma once

#include "GameCoreAPI.h"

#include <Utilities/TemplateChecks.h>

class GameObject;

class IComponent
  {
  private:
    int m_type;

  protected:
    GameObject* mp_owner;

  public:
    IComponent (int i_type, GameObject* ip_owner)
      : m_type(i_type)
      , mp_owner(ip_owner)
      {      }
    virtual ~IComponent() { }

    int           GetID() const;
    GameObject*   GetOwner() const;

    template <typename OwnerType>
    OwnerType*    Owner() const;

    virtual void  Tick(long i_ms_for_tick) { }
    virtual bool  Probe() { return true; }
  };

//////////////////////////////////////////////////////////////////////////

inline int IComponent::GetID() const
  {
  return m_type;
  }

inline GameObject* IComponent::GetOwner() const
  {
  return mp_owner;
  }

template <typename OwnerType>
inline OwnerType* IComponent::Owner() const
  {
  DerivedFrom<GameObject, OwnerType>();
  return static_cast<OwnerType*>(mp_owner);
  }