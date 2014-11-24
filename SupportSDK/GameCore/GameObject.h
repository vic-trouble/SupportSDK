#pragma once

#include "GameCoreAPI.h"

#include "IMovable.h"

class GameContext;
class IComponent;
class IController;

class GameObject : public IMovable
  {
  public:
    typedef std::unique_ptr<IComponent> ComponentUniquePtr;

  private:
    long                              m_id;
    int                               m_type;
    bool                              m_destroyed;

  protected:
    std::vector<ComponentUniquePtr>   m_components;
    IController*                      mp_owner;
    GameContext&                      m_context;

    bool                              m_has_changes;
    bool                              m_selected;

  protected:
    void            TickComponents(long i_ms_for_tick);

  public:
    GAMECORE_EXPORT GameObject(long i_id, int i_type, GameContext& i_context, IController* ip_owner);
    GAMECORE_EXPORT virtual ~GameObject();

    long                              GetID() const;
    int                               GetType() const;

    GAMECORE_EXPORT virtual void      Tick(long i_ms_for_tick);

    void                              SetOwner(IController* ip_owner);
    IController*                      GetOwner() const;
    bool                              HasOwner() const;

    GameContext&                      GetContext();

    GAMECORE_EXPORT void              AddComponent (ComponentUniquePtr ip_component);
    /// validate state of components
    /// each component checks its state and that all
    /// dependencies are present in object
    GAMECORE_EXPORT bool              ProbeComponents ();

    /// check if component with global id "i_id" is in components collection
    GAMECORE_EXPORT bool              HasComponent(int i_component_id) const;
    GAMECORE_EXPORT IComponent*       GetComponent(long i_component_id) const;

    template <typename ComponentType>
    ComponentType*                    GetComponent();

    void                              Destroy();
    bool                              IsDestroyed() const;

    void                              Select();
    void                              Deselect();
    bool                              IsSelected() const;

    bool                              HasChanges() const;
    void                              Validate();
    void                              StateChanged();
  };

//////////////////////////////////////////////////////////////////////////

template <typename ComponentType>
ComponentType* GameObject::GetComponent()
  {
  for(ComponentUniquePtr& p_component : m_components)
    {
    if(typeid(*p_component) == typeid(ComponentType))
      return static_cast<ComponentType*>(p_component.get());
    }
  return nullptr;
  }

inline void GameObject::SetOwner(IController* ip_owner)
  {
  mp_owner = ip_owner;
  }

inline IController* GameObject::GetOwner() const
  {
  return mp_owner;
  }

inline bool GameObject::HasOwner() const
  {
  return mp_owner != nullptr;
  }

inline GameContext& GameObject::GetContext()
  {
  return m_context;
  }

inline void GameObject::Destroy()
  {
  m_destroyed = true;
  }

inline bool GameObject::IsDestroyed() const
  {
  return m_destroyed;
  }

inline void GameObject::Select()
  {
  m_selected = true;
  }

inline void GameObject::Deselect()
  {
  m_selected = false;
  }

inline bool GameObject::IsSelected() const
  {
  return m_selected;
  }

inline bool GameObject::HasChanges() const
  {
  return m_has_changes;
  }

inline void GameObject::Validate()
  {
  m_has_changes = false;
  }

inline void GameObject::StateChanged()
  {
  m_has_changes = true;
  }

inline long GameObject::GetID() const
  {
  return m_id;
  }

inline int GameObject::GetType() const
  {
  return m_type;
  }