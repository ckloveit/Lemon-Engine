#pragma once
#include <vector>
#include <memory>

#include <Core/Engine.h>

namespace Lemon
{
	class Engine;

}


class Editor
{
public:
	Editor() = default;
	~Editor();

	void OnWindowMessage(Lemon::WindowData& windowData);
	void OnTick();

private:
	bool m_Initialized = false;

	std::unique_ptr<Lemon::Engine> m_Engine;
};

