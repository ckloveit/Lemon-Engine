#pragma once
#include "Core.h"
#include <chrono>
#include "ISystem.h"

namespace Lemon
{
	class LEMON_API Timer : public ISystem
	{
	public:
		Timer(Engine* engine);
		~Timer() = default;

		void Tick(float deltaTime) override;

		auto GetDeltaTimeSec()  const { return static_cast<float>(m_DeltaTimeMs / 1000.0); }

		double GetGameTime() const { return m_GameTime;}
	private:
		// Frame time
		std::chrono::high_resolution_clock::time_point m_CurrentFrameTime;
		std::chrono::high_resolution_clock::time_point m_LastFrameTime;
		
		double m_DeltaTimeMs = 0.0f;

		double m_GameTime = 0.0;
	};
}