#ifndef __UTILITIESAPI_H__
#define __UTILITIESAPI_H__

#ifdef UTILITIES_EXPORTING
# define UTILITIES_API __declspec(dllexport)
#else
# define UTILITIES_API __declspec(dllimport)
#endif

#endif