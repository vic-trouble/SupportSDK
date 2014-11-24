#pragma once

#include "../GameCoreAPI.h"

#include "IComponentSerializer.h"

class TiXmlElement;
class GameObject;

namespace GameCore
  {

  class IObjectComposer;
  class IComponentSerializer;
  /*
  Settings that are read from .xml configuration
  node or .xml document.
  Contains information:
    - what components object consists of
    - settings for components
  */

  class ObjectSettings
    {
    public:
      typedef std::unique_ptr<IComponentSerializer> SerializerPtr;
      typedef std::vector<SerializerPtr> ComponentsContainer;

    private:
      int                 m_object_type;
      ComponentsContainer mp_component_serializers;

      const IObjectComposer& m_composer;

    private:
      virtual SerializerPtr _GetSerializerFromString(const std::string& i_type, const IObjectComposer& i_composer) const = 0;

    public:
      GAMECORE_EXPORT ObjectSettings (const IObjectComposer& i_composer);
      // construct object settings for i_type
      // parse i_object_node for getting list of components
      GAMECORE_EXPORT ObjectSettings (int i_type, const TiXmlElement& i_object_node, const IObjectComposer& i_composer);

      GAMECORE_EXPORT void Initialize(int i_type, const TiXmlElement& i_object_node);

      GAMECORE_EXPORT int                    GetType() const;
      GAMECORE_EXPORT void                   SetupObject(GameObject* ip_object) const;

      const IObjectComposer& GetComposer() const;

      template <typename SpecificType>
      SpecificType CastType() const
        {
        return static_cast<SpecificType>(m_object_type);
        }
    };

  //////////////////////////////////////////////////////////////////////////

  const IObjectComposer& ObjectSettings::GetComposer() const
    {
    return m_composer;
    }

  }