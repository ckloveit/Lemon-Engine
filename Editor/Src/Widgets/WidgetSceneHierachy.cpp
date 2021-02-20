#include "WidgetSceneHierachy.h"

WidgetSceneHierachy::WidgetSceneHierachy(Lemon::Engine* engine)
	:Widget(engine)
{
	m_WidgetProp.Title = WidgetGlobal::g_WidgetSceneHierachyTitle;
	m_WidgetProp.WindowFlags |= ImGuiWindowFlags_HorizontalScrollbar;

}

void WidgetSceneHierachy::Tick()
{



}
