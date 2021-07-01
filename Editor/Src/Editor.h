#pragma once
#include <vector>
#include <memory>
#include "Widgets/Widget.h"
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
	void InitImGui(const Lemon::WindowData& windowData);

private:
	void SetDarkThemeColors();
	void WidgetsTick(float deltaTime);
	void WidgetsCreate();
	void BeginDockSpace();
	void EndDockSpace();

private:
	bool m_Initialized = false;
	bool m_bHasInitImGui = false;

	std::unique_ptr<Lemon::Engine> m_Engine;
	std::vector<std::unique_ptr<Widget>> m_Widgets;
};

