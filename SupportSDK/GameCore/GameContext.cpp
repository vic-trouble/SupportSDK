#include "stdafx.h"

#include "GameContext.h"
#include "IController.h"

#include "GameObject.h"

//////////////////////////////////////////////////////////////////////////

namespace
  {

  struct ControllersInformation_SearchByMask
    {
    int m_mask;
    ControllersInformation_SearchByMask(int i_mask_searched)
      : m_mask(i_mask_searched)
      {}
    bool operator () (const GameContext::ControllerInformation& i_information)
      {
      return i_information.mp_controller->GetMask() == m_mask;
      }
    };

  } // namespace

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// GameContext::ControllerInformation

GameContext::ControllerInformation::ControllerInformation()
  : mp_controller(nullptr)
  {

  }

GameContext::ControllerInformation::ControllerInformation(ControllerInformation&& i_information)
  : mp_controller(std::move(i_information.mp_controller))
  , m_needed_resources(std::move(i_information.m_needed_resources))
  {

  }

bool GameContext::ControllerInformation::operator == (const ControllerInformation& i_other)
  {
  return mp_controller->GetMask() == i_other.mp_controller->GetMask();
  }

//////////////////////////////////////////////////////////////////////////

GameContext::GameContext (const std::string& i_context_name)
  : m_context_name(i_context_name)
  , m_next_object_id(0)
  {

  }

GameContext::~GameContext()
  {
  }

void GameContext::TickPerformed(long i_ms_for_tick)
  {
  // update objects
  for (auto it = m_objects.begin(); it != m_objects.end();)
  {
	  if (it->second->IsDestroyed())
		  it = m_objects.erase(it);
	  else
	  {
		  it->second->Tick(i_ms_for_tick);
		  ++it;
	  }
  }
  }

void GameContext::ReleaseContext()
  {
  // custom actions of derived class
  }

void GameContext::Release()
  {
  ReleaseContext();
  m_objects.clear();
  m_controllers.clear();
  }

void GameContext::RemoveObject(GameObject* ip_object)
  {
	  ip_object->Destroy();
  }

void GameContext::RegisterController(std::unique_ptr<IController> ip_controller)
  {
  ControllerInformation controller_info;
  controller_info.mp_controller = std::move(ip_controller);
  m_controllers.push_back(std::move(controller_info));
  }

void GameContext::RegisterResources (int i_type, const ResourcesCountSet& i_resources)
  {
  std::for_each(m_controllers.begin(), m_controllers.end(), [this, i_type, &i_resources](ControllerInformation& i_information)
    {
    std::copy(i_resources.begin(), i_resources.end(), i_information.m_needed_resources[i_type].begin());
    });
  }

const ResourcesCountSet& GameContext::GetResourcesFor (int i_type, int i_controller_mask)
  {
  std::vector<GameContext::ControllerInformation>::iterator it = std::find_if(m_controllers.begin(), m_controllers.end(), ControllersInformation_SearchByMask(i_controller_mask));

  if (it == m_controllers.end())
    {
    assert (false && "Who asked for this");
    throw std::logic_error("No controller with specific mask");
    }

  if ((*it).m_needed_resources.find(i_type) == (*it).m_needed_resources.end())
    {
    assert (false && "Type is not registered");
    throw std::logic_error("Type is not registered");
    }

  return (*it).m_needed_resources[i_type];
  }