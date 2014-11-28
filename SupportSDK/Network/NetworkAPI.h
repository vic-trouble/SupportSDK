#ifndef __NETWORKAPI_H__
#define __NETWORKAPI_H__

#ifdef NETWORK_EXPORTING
# define NETWORK_API __declspec(dllexport)
#else
# define NETWORK_API __declspec(dllimport)
#endif

#endif