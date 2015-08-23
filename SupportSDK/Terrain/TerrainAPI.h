#ifndef __TERRAINAPI_H__
#define __TERRAINAPI_H__

#ifdef TERRAIN_EXPORTING
# define TERRAIN_API __declspec(dllexport)
#else
# define TERRAIN_API __declspec(dllimport)
#endif

#endif