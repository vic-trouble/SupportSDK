#include "stdafx.h"

#include "ObjectSettings.h"
#include "IObjectComposer.h"
#include "../GameObject.h"
#include "../IComponent.h"
#include "IComponentSerializer.h"

#include <Utilities/XmlUtilities.h>

using namespace Xml;

//////////////////////////////////////////////////////////////////////////

namespace GameCore
  {

  ObjectSettings::ObjectSettings (const IObjectComposer& i_composer)
    : m_composer(i_composer)
    , m_object_type(-1)
    {

    }

  ObjectSettings::ObjectSettings(int i_type, const TiXmlElement& i_object_node, const IObjectComposer& i_composer)
    : m_object_type(i_type)
    , m_composer(i_composer)
    {
    Initialize(i_type, i_object_node);
    }

  ObjectSettings::~ObjectSettings()
    {}

  void ObjectSettings::Initialize(int i_type, const Xml::TiXmlElement& i_object_node)
    {
    m_object_type = i_type;
    std::string component_name = "";

    const TiXmlElement* p_child = 0;

    while (p_child = XmlUtilities::IterateChildElements(&i_object_node, p_child))
      {
      component_name = XmlUtilities::GetStringAttribute(p_child, "type", "");
      SerializerPtr p_serializer = std::move(_GetSerializerFromString(component_name, m_composer));
      if (nullptr != p_serializer)
        {
        p_serializer->Parse(*p_child);
        mp_component_serializers.push_back(std::move(p_serializer));
        }
      }
    }

  int ObjectSettings::GetType() const
    {
    return m_object_type;
    }

  void ObjectSettings::SetupObject(GameObject* ip_object) const
    {
    for (const SerializerPtr& serializer : mp_component_serializers)
      {
      std::unique_ptr<IComponent> p_component(serializer->CreateComponent(ip_object));
      if (p_component)
        ip_object->AddComponent(std::move(p_component));
      }
    }
  }