#ifndef	__GAMECORE_GLOBALOBJECTBASE_H__
#define __GAMECORE_GLOBALOBJECTBASE_H__

namespace SDK
{
	class GlobalObjectBase
	{
	private:
		size_t m_hash_code;

	public:		
		virtual ~GlobalObjectBase() {}

		size_t GetTypeHashCode() const { return m_hash_code; }
		void RecalcHashCode() { m_hash_code = typeid(*this).hash_code(); }
	};

} // SDK

#endif
