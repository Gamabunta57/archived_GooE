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

#define BIT(x) (1 << x)
