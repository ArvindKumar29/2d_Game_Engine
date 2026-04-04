#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HAZLE_API __declspec(dllexport)
	#else
		#define HAZLE_API __declspec(dllimport)
	#endif
#else	
	#error Hazle only supports Windows!
#endif