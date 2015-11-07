#ifndef __GAMECORE_PLATFORM_H__
#define __GAMECORE_PLATFORM_H__

#if defined(_WIN32) || defined(WINDOWS)
# define TARGET_PLATFORM_WIN32
#elif defined(__ANDROID__) || defined(ANDROID) 
# define TARGET_PLATFORM_ANDROID
#endif

// Compiler features definition

#if defined(TARGET_PLATFORM_WIN32)
# if _MSC_VER >= 1800
#  define CPP_11
# else
# endif
#endif

#endif