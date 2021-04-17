#include "WidgetProperties.h"

WidgetProperties::WidgetProperties(Lemon::Engine* engine)
	:Widget(engine)
{
	m_WidgetProp.Title = WidgetGlobal::g_WidgetPropertiesTitle;
	m_WidgetProp.MinSize = glm::vec2(370.0f, -1.0f);

}

void WidgetProperties::Tick(float deltaTime)
{



}