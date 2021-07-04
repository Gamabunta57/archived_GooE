#pragma once

#ifdef GOOE_PLATFORM_WINDOWS
	#ifdef GOOE_BUILD_STATIC
		#define GOOE_API
	#else
		#ifdef GOOE_BUILD_DLL
			#define GOOE_API  __declspec(dllexport)
		#else
			#define GOOE_API  __declspec(dllimport)
		#endif
	#endif
#else
	#error Windows only is supported!
#endif // GOOE_PLATFORM_WINDOWS

#ifdef GOOE_ENABLE_ASSERTS
	#define GOOE_ASSERT(x, ...) { if (!(x)) {GOOE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define GOOE_CORE_ASSERT(x, ...) { if (!(x)) {GOOE_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define GOOE_ASSERT(x, ...)
	#define GOOE_CORE_ASSERT(x, ...)
#endif

#define GOOE_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)
