#pragma once

#ifdef GAMECORE_EXPORTING
#define GAMECORE_EXPORT __declspec(dllexport)
#else 
#define GAMECORE_EXPORT __declspec(dllimport)
#endif