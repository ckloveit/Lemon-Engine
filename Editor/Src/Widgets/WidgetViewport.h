#pragma once

#include "Widget.h"

namespace Lemon
{
	class Renderer;
}

class WidgetViewport : public Widget
{
public:
	WidgetViewport(Lemon::Engine* engine);
	virtual void Tick() override;

	glm::vec2 GetViewportSize() const { return m_ViewportSize; }
private:
	Lemon::Renderer* m_Renderer = nullptr;

	glm::vec2 m_ViewportSize = {0, 0};
};