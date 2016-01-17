#ifndef	__GAMECORE_BASETYPESHASHES_H__
#define __GAMECORE_BASETYPESHASHES_H__

#include <Utilities/HashFunctions.h>

namespace SDK
{
#pragma warning( push )
#pragma warning( disable : 4307)
	constexpr size_t BOOL_HASH = Utilities::hash_str("bool");
	constexpr size_t TRUE_HASH = Utilities::hash_str("true");
	constexpr size_t FALSE_HASH = Utilities::hash_str("false");
	constexpr size_t CHAR_HASH = Utilities::hash_str("char");
	constexpr size_t INT_HASH = Utilities::hash_str("int");
	constexpr size_t FLOAT_HASH = Utilities::hash_str("float");
	constexpr size_t STRING_HASH = Utilities::hash_str("string");
#pragma warning ( pop )
} // SDK

#endif
