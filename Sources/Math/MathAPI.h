#ifndef __MATHAPI_H__
#define __MATHAPI_H__

#ifdef MATH_EXPORTING
# define MATH_API __declspec(dllexport)
#else
# define MATH_API __declspec(dllimport)
#endif

#endif