#ifndef __GRAPHSAPI_H__
#define __GRAPHSAPI_H__

#ifdef GRAPHS_EXPORTING
# define GRAPHS_API __declspec(dllexport)
#else
# define GRAPHS_API __declspec(dllimport)
#endif

#endif