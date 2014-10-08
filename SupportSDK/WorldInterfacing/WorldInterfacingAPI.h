#ifndef __EXECUTIONAPI_H__
#define __EXECUTIONAPI_H__

#ifdef WORLDINTERFACING_EXPORTING
# define WORLDINTERFACING_API __declspec(dllexport)
#else
# define WORLDINTERFACING_API __declspec(dllimport)
#endif

#endif