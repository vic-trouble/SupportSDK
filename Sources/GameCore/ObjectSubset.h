#ifndef	__GAMECORE_OBJECTSUBSET_H__
#define __GAMECORE_OBJECTSUBSET_H__

#include "GameCoreAPI.h"

namespace SDK
{

	class EntityManager;

	template <typename HandleType>
	class ObjectSet
	{
	private:
		std::vector<HandleType> m_handles;

	public:
		void AddEntity(HandleType i_handle)
		{
			m_handles.emplace_back(i_handle);
		}
		void RemoveEntity(HandleType i_handle)
		{
			auto it = std::find(m_handles.begin(), m_handles.end(), i_handle);
			if (it != m_handles.end())
				m_handles.erase(it);
		}

		const std::vector<HandleType>& GetHandles() const
		{
			return m_handles;
		}
	};

}

#endif
