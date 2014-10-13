#ifndef __TYPE_DEFINES_H__
#define __TYPE_DEFINES_H__

#ifndef DOUBLE_PRECISION
# define FLOAT_PRECISION
#endif

namespace SDK
	{

	typedef unsigned short ushort;
	typedef unsigned int	 uint;
	typedef unsigned long	 ulong;

#ifdef FLOAT_PRECISION
	typedef float					 real;
#elif DOUBLE_PRECISION
	typedef double				 real;
#endif

	}

#endif