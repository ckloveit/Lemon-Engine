#pragma once
#include "ISystem.h"
#include "Log/Log.h"

namespace Lemon
{
	class LEMON_API SystemManager
	{
	public:
		SystemManager() = default;
		~SystemManager();

		//Register System
		template <class T>
		void RegisterSystem(Engine* engine)
		{
			ValidateSystemType<T>();
			m_Systems.emplace_back(CreateRef<T>(engine));
		}

		// Initialize subsystems
		bool Initialize();

		template<typename T>
		T* GetSystem() const
		{
			ValidateSystemType<T>();
			for (const auto& system : m_Systems)
			{
				if (system)
				{
					if (typeid(T) == typeid(*system))
						return static_cast<T*>(system.get());
				}
			}
			return nullptr;
		}
	
		// Tick
		void Tick(float deltaTime = 0.0f);

	private:
		std::vector<Ref<ISystem>> m_Systems;
	};
}