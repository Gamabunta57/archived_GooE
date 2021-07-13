#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "Core.h"

namespace GooE {
	class GOOE_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger;  }
		
	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clientLogger;
	};
}

#ifndef DIST
	#define GOOE_CORE_FATAL(...) ::GooE::Log::GetCoreLogger()->fatal(__VA_ARGS__)
	#define GOOE_CORE_ERROR(...) ::GooE::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define GOOE_CORE_WARN(...)  ::GooE::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define GOOE_CORE_INFO(...)  ::GooE::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define GOOE_CORE_TRACE(...) ::GooE::Log::GetCoreLogger()->trace(__VA_ARGS__)

	#define GOOE_FATAL(...) ::GooE::Log::GetClientLogger()->fatal(__VA_ARGS__)
	#define GOOE_ERROR(...) ::GooE::Log::GetClientLogger()->error(__VA_ARGS__)
	#define GOOE_WARN(...)  ::GooE::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define GOOE_INFO(...)  ::GooE::Log::GetClientLogger()->info(__VA_ARGS__)
	#define GOOE_TRACE(...) ::GooE::Log::GetClientLogger()->trace(__VA_ARGS__)
#else
	#define GOOE_CORE_FATAL(...)
	#define GOOE_CORE_ERROR(...)
	#define GOOE_CORE_WARN(...)
	#define GOOE_CORE_INFO(...)
	#define GOOE_CORE_TRACE(...)

	#define GOOE_FATAL(...)
	#define GOOE_ERROR(...)
	#define GOOE_WARN(...)
	#define GOOE_INFO(...)
	#define GOOE_TRACE(...)
#endif
