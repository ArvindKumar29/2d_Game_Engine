#pragma once
#include <memory>

// For use by Hazle applications
#ifdef HZ_PLATFORM_WINDOWS // defining windows platform
	
	#ifdef HZ_DYNAMIC_LINK // defining dynamic link library
		#ifdef HZ_BUILD_DLL // defining build dll
			#define HAZLE_API __declspec(dllexport) // export dll
		#else
			#define HAZLE_API __declspec(dllimport) // import dll
		#endif
	#else
		#define HAZLE_API
	#endif
#else	
	#error Hazle only supports Windows! // error if not windows platform
#endif

#ifdef HZ_ENABLE_ASSERTS // defining asserts for dubugging
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }	
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x) // defining bit shift for event handling

#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1) // binding event functions to the event dispatcher

namespace Hazle
{
	template <typename T>
	using Scope = std::unique_ptr<T>; // defining scope pointer for unique ownership

	template <typename T>
	using Ref = std::shared_ptr<T>; // defining reference pointer for shared ownership

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args) // creating a scope pointer with perfect forwarding
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args) // creating a reference pointer with perfect forwarding
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}