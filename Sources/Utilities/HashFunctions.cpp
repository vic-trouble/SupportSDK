#include "stdafx.h"

#include "HashFunctions.h"

namespace SDK
{
	namespace Utilities
	{
#if defined(CPP_11)
		static std::hash<std::string> hash_func;
		size_t hash_function(const std::string& str)
		{
			return hash_func(str);
		}
#else
		size_t hash_function(const std::string& str)
		{
			size_t h = 31 /* also prime */;
			for (char c : str)
				h = (h * A) ^ (c * B);			
			return h; // or return h % C;
		}
#endif

	} // Utilities
} // SDK