#ifndef __GAMECORE_PLATFORM_H__
#define __GAMECORE_PLATFORM_H__

#if defined(_WIN32) || defined(WINDOWS)
# define TARGET_PLATFORM_WIN32
#elif defined(__ANDROID__) || defined(ANDROID) 
# define TARGET_PLATFORM_ANDROID
#endif

#endif