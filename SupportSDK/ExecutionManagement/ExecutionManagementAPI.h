#ifndef __EXECUTIONAPI_H__
#define __EXECUTIONAPI_H__

#ifdef EXECUTIONMANAGEMENT_EXPORTING
# define EXECUTIONMANAGEMENT_API __declspec(dllexport)
#else
# define EXECUTIONMANAGEMENT_API __declspec(dllimport)
#endif

#endif