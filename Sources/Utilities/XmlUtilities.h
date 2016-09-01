#ifndef __XMLUTILITIES_H__
#define __XMLUTILITIES_H__

#include "UtilitiesAPI.h"

#include <tinyxml2.h>

#include "noncopyable.h"

using namespace tinyxml2;
class XmlUtilities : SDK::Utilities::noncopyable
{
private:
  XmlUtilities();
  ~XmlUtilities();

public:
  static UTILITIES_API  bool					              LoadXmlDocument(const std::string& i_file_name, XMLDocument& io_document);
  static UTILITIES_API  const XMLElement*	      IterateChildElements(const XMLElement* i_parent_element, const XMLElement* i_child_element);

	static UTILITIES_API std::pair<float, float>    GetXY(const XMLElement* i_xml_element);
  static UTILITIES_API float				              GetRealAttribute(const XMLElement* i_xml_element, const char* i_name, float i_default_value = .0f);
	static UTILITIES_API std::string	              GetStringAttribute(const XMLElement* i_xml_element, const char* i_name);
	static UTILITIES_API std::string	              GetStringAttribute(const XMLElement* i_xml_element, const char* i_name, const char* i_default_value);
	static UTILITIES_API int					              GetIntAttribute(const XMLElement* i_xml_element, const char* i_name, int i_default_value);
	static UTILITIES_API bool					              GetBoolAttribute(const XMLElement* i_xml_element, const char* i_name, bool i_default_value);
  /*
	* Recursively searches the XML element tree, counting elements with the specified i_name
	* @param element [in] - The start element. This element is included in the count
	* @param elementName [in] - The element i_name to search for
	* @return Returns the number of elements with the specified i_name
	*/
	static UTILITIES_API size_t                     GetElementCount(const XMLElement* i_xml_element, const std::string& i_element_name);
	static UTILITIES_API size_t                     GetChildElementCount(const XMLElement* i_xml_element, const std::string& i_element_name);
};

#endif