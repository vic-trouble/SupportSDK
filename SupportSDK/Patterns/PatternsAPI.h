#ifndef __PATTERNSAPI_H__
#define __PATTERNSAPI_H__

#ifdef PATTERNS_EXPORTING
# define PATTERNS_API __declspec(dllexport)
#else
# define PATTERNS_API __declspec(dllimport)
#endif

#endif