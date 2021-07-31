#pragma once

#include <memory>

//Platform detection
#ifdef _WIN32
	#ifdef _WIN64
		#define GOOE_PLATFORM_WINDOWS
	#else
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "iOS simulator is not supported yet!"
	#elif TARGET_OS_IPHONE == 1
		#define GOOE_PLATFORM_IOS
		#error "iOS is not supported yet!"
	#elif TARGET_OS_MAC == 1
		#define GOOE_PLATFORM_MACOS
		#error "MacOS is not supported yet!"
	#else
		#error "Unsupported Apple platform!"
	#endif
#elif defined(__ANDROID__)
	#define GOOE_PLATFORM_ANDROID
	#error "Android is not supported yet!"
#elif defined(__linux__)
	#define GOOE_PLATFORM_LINUX
	#error "Linux is not supported yet!"
#else
	#error "Unknown platform!"
#endif

#ifdef GOOE_PLATFORM_WINDOWS
	#ifdef GOOE_BUILD_STATIC
		#define GOOE_API
	#else
		#ifdef GOOE_BUILD_DLL
			#define GOOE_API  __declspec(dllexport)
		#else
			#define GOOE_API  __declspec(dllimport)
		#endif
		#error "Building GooE as DLL is not supported yet!"
	#endif
#else
	#error "Windows only is supported at the moment!"
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

namespace GooE {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}