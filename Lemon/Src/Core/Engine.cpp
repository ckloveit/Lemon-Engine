#include "LemonPCH.h"
#include "Engine.h"
#include "SystemManager.h"
#include "Timer.h"

using namespace std;

namespace Lemon
{
	Engine::Engine(const WindowData& window_data)
	{
		// Window
		m_WindowData = window_data;

		// Initialize Logger
		Lemon::Logger::Init();

		//SystemManager
		m_SystemManager = CreateRef<SystemManager>();

		//Register System
		m_SystemManager->RegisterSystem<Timer>(); // must be first so it ticks first

		//Initialize Systems
		m_SystemManager->Initialize();

		// Get the Timer
		m_Timer = m_SystemManager->GetSystem<Timer>();

		LEMON_CORE_INFO("LemonEngine Initialize");
	}

	Engine::~Engine()
	{
	
	}
	void Engine::Tick() const
	{
		m_SystemManager->Tick(static_cast<float>(m_Timer->GetDeltaTimeSec()));
	}

	void Engine::SetWindowData(WindowData& window_data)
	{
		m_WindowData = window_data;
	//	FIRE_EVENT(EventType::WindowData);
	}
}