#pragma once
#include "Core/Core.h"
// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Lemon
{
	class LEMON_API Logger
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

#if LEMON_DEBUG
	//define Core Some Macro
#define LEMON_CORE_TRACE(...)    ::Lemon::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define LEMON_CORE_INFO(...)     ::Lemon::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LEMON_CORE_WARN(...)     ::Lemon::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LEMON_CORE_ERROR(...)    ::Lemon::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LEMON_CORE_FATAL(...)    ::Lemon::Logger::GetCoreLogger()->critical(__VA_ARGS__)

#define LEMON_CLIENT_TRACE(...)          ::Lemon::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LEMON_CLIENT_INFO(...)           ::Lemon::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LEMON_CLIENT_WARN(...)           ::Lemon::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LEMON_CLIENT_ERROR(...)          ::Lemon::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LEMON_CLIENT_FATAL(...)          ::Lemon::Logger::GetClientLogger()->critical(__VA_ARGS__)
#else
#define LEMON_CORE_TRACE(...)    
#define LEMON_CORE_INFO(...)     
#define LEMON_CORE_WARN(...)     
#define LEMON_CORE_ERROR(...)    
#define LEMON_CORE_FATAL(...)    

#define LEMON_CLIENT_TRACE(...)          
#define LEMON_CLIENT_INFO(...)           
#define LEMON_CLIENT_WARN(...)           
#define LEMON_CLIENT_ERROR(...)          
#define LEMON_CLIENT_FATAL(...)          

#endif
}