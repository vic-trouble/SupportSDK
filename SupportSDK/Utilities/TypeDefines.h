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
  typedef unsigned char  uchar;

	typedef unsigned __int64 uint64;
	typedef signed __int64	int64;

#ifdef FLOAT_PRECISION
	typedef float					 real;
#elif DOUBLE_PRECISION
	typedef double				 real;
#endif

	}

#endif