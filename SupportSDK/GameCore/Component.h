#ifndef __GAMECORE_COMPONENT_H__
#define __GAMECORE_COMPONENT_H__

namespace SDK
{

	enum class ComponentClass
	{
		MeshClass,
		TransformClass
	};

	typedef void (*RemoveFunction)(int i_in_system_id, int i_in_system_generation);

	struct Component
	{
		int m_in_system_id;
		int m_in_system_generation;
		int m_component_id;
		RemoveFunction m_remove_function;
	};



} // SDK

#endif
