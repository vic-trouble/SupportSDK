#pragma once

#include "GameCoreAPI.h"

#include "TimeController.h"
#include "Types.h"

#include <Utilities/noncopyable.h>

class GameObject;
class IController;

class GameContext : SDK::Utilities::noncopyable
                  , public SDK::TickListener
  {
  public:
    typedef std::unique_ptr<GameObject> GameObjectUniquePtr;
  
  public:

	  struct ControllerInformation : SDK::Utilities::noncopyable
      {
      std::unique_ptr<IController>                    mp_controller;
      /// resources that will be needed for object creation of object
      std::map<int/*object type*/, ResourcesCountSet>  m_needed_resources;

      GAMECORE_EXPORT ControllerInformation();
      GAMECORE_EXPORT ControllerInformation(ControllerInformation&& i_information);
      GAMECORE_EXPORT bool operator == (const ControllerInformation& i_other);
      };

  protected:
    std::map<long, GameObjectUniquePtr> m_objects;
    std::string                         m_context_name;
    long                                m_next_object_id;

    std::vector<ControllerInformation>  m_controllers;

  private:
    virtual void ReleaseContext();

  public:
    GAMECORE_EXPORT GameContext(const std::string& i_context_name);
    GAMECORE_EXPORT virtual ~GameContext();

    GAMECORE_EXPORT virtual void                TickPerformed(long i_ms_for_tick) override;

    GAMECORE_EXPORT void                        Release();

    /// add object to the dead pool
    /// dead pool is clearing in TickPerformed method before updating all objects
    GAMECORE_EXPORT void						RemoveObject(GameObject* ip_object);

    GAMECORE_EXPORT void                        RegisterController(std::unique_ptr<IController> ip_controller);
    GAMECORE_EXPORT void                        RegisterResources (int i_type, const ResourcesCountSet& i_resources);

    /// @param i_type - type of object for which resources needed
    /// @param i_controller_mask - because price for different controllers can vary due to
    ///                             development of technologies or something else
    ///                             resources for each controller can be different
    GAMECORE_EXPORT const ResourcesCountSet&    GetResourcesFor (int i_type, int i_controller_mask);

    std::string                                 GetName() const;
  };

//////////////////////////////////////////////////////////////////////////

inline std::string GameContext::GetName() const
  {
  return m_context_name;
  }