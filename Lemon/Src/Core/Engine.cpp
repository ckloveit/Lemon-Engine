#include "LemonPCH.h"
#include "Engine.h"
#include "SystemManager.h"
#include "Timer.h"
#include "Renderer/Renderer.h"
#include "World/World.h"
#include "Input/InputSystem.h"
#include "Resources/ResourceSystem.h"

using namespace std;

namespace Lemon
{
	Engine::Engine(const WindowData& windowData)
	{
		// Window
		m_WindowData = windowData;

		// Initialize Logger
		Lemon::Logger::Init();

		//SystemManager
		m_SystemManager = CreateRef<SystemManager>();

		//Register System
		m_SystemManager->RegisterSystem<Timer>(this); // must be first so it ticks first
		m_SystemManager->RegisterSystem<ResourceSystem>(this);
		m_SystemManager->RegisterSystem<InputSystem>(this);

		m_SystemManager->RegisterSystem<World>(this);
		m_SystemManager->RegisterSystem<Renderer>(this);

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

	void Engine::SetWindowData(WindowData& windowData)
	{
		m_WindowData = windowData;
		OnWindowMessageEvent.Broadcast(windowData);
	}
}