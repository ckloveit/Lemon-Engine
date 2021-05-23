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
	virtual void Tick(float deltaTime) override;

	glm::vec2 GetViewportSize() const { return m_ViewportSize; }

private:
	void DrawToolBar();

	void DrawGizmoHandle();

	void DrawAxisGizmo();
private:
	Lemon::Renderer* m_Renderer = nullptr;

	glm::vec2 m_ViewportSize = {0, 0};

	bool m_bWidgetFocus = false;
	
	// GizmoType
	int m_GizmoType = -1;
	// GizmoMode
	int m_GizmoMode = 0;// 0 -local ; 1 - world
};