#ifndef	__UTILITIES_HASHFUNCTIONS_H__
#define __UTILITIES_HASHFUNCTIONS_H__

#include "UtilitiesAPI.h"

#include "ConstString.h"

namespace SDK
{
	namespace Utilities
	{
		namespace impl
		{
			constexpr size_t A = 54059; /* a prime */
			constexpr size_t B = 76963; /* another prime */
			constexpr size_t C = 86969; /* yet another prime */
		} // impl
		
		constexpr size_t hash_str(ConstString s, size_t h = 31, size_t i = 0)
		{
			return i == s.size() 
				? h // or h % impl::C
				: hash_str(s,
					(h * impl::A) ^ (s[i] * impl::B),
					i + 1);
		}

		template <typename hash_value>
		struct ConstStrHash
		{
			static constexpr size_t hash = hash_value;
		};

		UTILITIES_API size_t hash_function(const std::string& str);

	} // Utilities
} // SDK
#endif
