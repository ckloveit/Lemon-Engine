#pragma once
#include "Widget.h"

class WidgetMenuBar : public Widget
{
public:
	WidgetMenuBar(Lemon::Engine* engine);
	virtual void Tick(float deltaTime) override;

private:
	void ShowAboutWindow();


private:
	bool m_ShowDemoWindow = false;
	bool m_ImGuiStyle = false;
	bool m_ImGuiMetrics = false;
	bool m_ShowAboutWindow = false;
};