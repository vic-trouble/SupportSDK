#ifndef	__GAMECORE_TRANSFORMATIONSYSTEM_H__
#define __GAMECORE_TRANSFORMATIONSYSTEM_H__

#include "../GameCoreAPI.h"

#include "../Component.h"
#include "../Entity.h"
#include "../System.h"

#include "../GenericHandle.h"

namespace SDK
{

	class TransformationsSystem;

	struct Transform
	{
	public:
		static constexpr int ID = static_cast<int>(ComponentClass::TransformClass);
		typedef TransformationsSystem ProcessorSystem;

	public:
		Vector3 m_position;
		// rotation
		// scale
		// http://bitsquid.blogspot.com/2012/07/matrices-rotation-scale-and-drifting.html

		Transform() {}
		Transform(Vector3 i_position)
			: m_position(i_position)
		{}

		// Extension for EntityManager
		EntityHandler m_entity;
		void SetEntity(EntityHandler i_entity) { m_entity = i_entity; }
		EntityHandler GetEntity() const { return m_entity; }
	};

	struct TransformHandlerTag {};
	typedef GenericHandle<int, 12, 20, TransformHandlerTag> TransformHandler;

	class TransformationsSystem : public System
	{
	private:
		// separate vectors for trnasform;aabb and other, or one handler and {transform;aabb;etc.}
		std::vector<TransformHandler> m_handlers;		
		// TODO: vector or ?
		std::vector<Transform> m_transforms;

		// aabbs
		// partition

	public:
		// System
		virtual void Update(float i_elapsed_time) override;

		virtual bool Requires(Action i_aciton) const { return i_aciton == Action::Update; }

		GAMECORE_EXPORT TransformHandler CreateInstance();
		// if you want to change something - use this method. On one frame it is guaranteed that 
		//	pointer will be valid if not nullptr
		GAMECORE_EXPORT Transform* GetInstance(TransformHandler i_handler);
		GAMECORE_EXPORT void RemoveInstance(TransformHandler i_handler);

		// TODO: Custom mesh
		//MeshHandler Register(const std::string& i_name, Mesh i_mesh);
	// Extension for entity manager
	public:
		GAMECORE_EXPORT static Transform* Get(int i_in_system_id, int i_in_system_generation);
		GAMECORE_EXPORT static void Remove(int i_in_system_id, int i_in_system_generation);
	};

	// TODO: global object: decide in what format user should access objects like this
	GAMECORE_EXPORT extern TransformationsSystem g_transforms_system;

} // SDK

#endif
