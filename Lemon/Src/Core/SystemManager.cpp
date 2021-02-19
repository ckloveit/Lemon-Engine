#include "LemonPCH.h"
#include "SystemManager.h"

namespace Lemon
{
	SystemManager::~SystemManager()
	{
		// Loop in reverse registration order to avoid dependency conflicts
		for (size_t i = m_Systems.size() - 1; i > 0; i--)
		{
			m_Systems[i].reset();
		}
		m_Systems.clear();
	}

	bool SystemManager::Initialize()
	{
		auto result = true;
		for (const auto& system : m_Systems)
		{
			if (!system->Initialize())
			{
				LEMON_CORE_ERROR("Failed to initialize {0}", typeid(*system).name());
				result = false;
			}
		}
		return result;
	}

	void SystemManager::Tick(float deltaTime/* = 0.0f*/)
	{
		for (const auto& subsystem : m_Systems)
		{
			subsystem->Tick(deltaTime);
		}
	}

}