#pragma once

constexpr auto EngineVersion = "LemonEngineV01";

#include "Core.h"
#include "SystemManager.h"
#include "Delegate.h"

namespace Lemon
{
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

		// WindowData
		const WindowData& GetWindowData() const { return m_WindowData; }
		void SetWindowData(WindowData& windowData);

		template<typename T>
		T* GetSystem() const
		{
			return m_SystemManager->GetSystem<T>();
		}

	public:
		//Delegate
		MultiDelegate<void, WindowData> OnWindowMessageEvent;

	private:
		WindowData m_WindowData;
		Ref<SystemManager> m_SystemManager;
		Timer* m_Timer;


	};




}


