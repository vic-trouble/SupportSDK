#ifndef	__UTILITIES_CONSTSTRING_H__
#define __UTILITIES_CONSTSTRING_H__

#include <stdexcept>

namespace SDK
{
	namespace Utilities
	{
		
		class ConstString
		{
			const char* p;
			size_t sz;
		public:
			template<std::size_t N>
			constexpr ConstString(const char(&a)[N]) : p(a), sz(N - 1) {}

			constexpr char operator[](size_t n) const
			{
				return n < sz ? p[n] : throw std::out_of_range("");
			}
			constexpr std::size_t size() const { return sz; }
		};

	} // Uitlities
} // SDK
#endif
