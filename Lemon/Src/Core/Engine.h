#pragma once

#include "Core.h"


namespace Lemon
{
	class SystemManager;
	class Timer;

	struct WindowData
	{
		void* Handle			= nullptr;
		void* hInstance			= nullptr;
		uint32_t Message		= 0;
		float Width				= 0;
		float Height			= 0;
		uint64_t Wparam			= 0;
		int64_t Lparam			= 0;

	};

	class LEMON_API Engine
	{
	public:
		Engine(const WindowData& window_data);
		~Engine();

		// Performs a simulation cycle
		void Tick() const;

		void SetWindowData(WindowData& window_data);

	private:
		WindowData m_WindowData;
		Ref<SystemManager> m_SystemManager;
		Timer* m_Timer;

	};




}


