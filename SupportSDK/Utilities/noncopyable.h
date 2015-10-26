#ifndef	__UTILITIES_NONCOPYABLE_H__
#define __UTILITIES_NONCOPYABLE_H__

namespace SDK
{

	namespace Utilities
	{
		
		class noncopyable
		{
#if _MSC_VER >= 1800
		protected:
			noncopyable() = default;
			~noncopyable() = default;
			noncopyable(const noncopyable&) = delete;
			noncopyable& operator=(const noncopyable&) = delete;
#else
		protected:
			noncopyable() {}
			~noncopyable() {}
		private:
			noncopyable(const noncopyable&);
			noncopyable& operator=(const noncopyable&);
#endif
		};

	} // Utilities

} // SDK

#endif