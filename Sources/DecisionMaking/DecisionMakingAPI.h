#ifndef __DECISIONMAKINGAPI_H__
#define __DECISIONMAKINGAPI_H__

#ifdef DECISIONMAKING_EXPORTING
# define DECISIONMAKING_API __declspec(dllexport)
#else
# define DECISIONMAKING_API __declspec(dllimport)
#endif

#endif