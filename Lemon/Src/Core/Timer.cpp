#include "LemonPCH.h"
#include "Timer.h"

using namespace std;

namespace Lemon
{
	Timer::Timer(Engine* engine)
		:ISystem(engine)
	{
		m_CurrentFrameTime = chrono::high_resolution_clock::now();
		m_LastFrameTime = chrono::high_resolution_clock::now();
	}

	void Timer::Tick(float deltaTime)
	{
		// Get time
		m_LastFrameTime = m_CurrentFrameTime;
		m_CurrentFrameTime = chrono::high_resolution_clock::now();

		// Compute durations
		chrono::duration<double, milli> timeDelta = m_CurrentFrameTime - m_LastFrameTime;

		m_DeltaTimeMs = static_cast<double>(timeDelta.count());
	}

}