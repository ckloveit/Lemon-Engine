#include "WidgetViewport.h"
#include "Renderer/Renderer.h"

using namespace Lemon;

WidgetViewport::WidgetViewport(Lemon::Engine* engine)
	:Widget(engine)
{
	m_WidgetProp.Title = WidgetGlobal::g_WidgetViewportTitle;
	m_WidgetProp.Size = glm::vec2(400, 250);
	m_WidgetProp.WindowFlags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
	m_WidgetProp.Padding = glm::vec2(4.0f, 4.0f);
	m_Renderer = m_Engine->GetSystem<Renderer>();

}

void WidgetViewport::Tick()
{
	if (!m_Renderer)
		return;



}

