#pragma once
#include "Widget.h"
#include "World/Entity.h"

class WidgetSceneHierachy : public Widget
{
public:
	WidgetSceneHierachy(Lemon::Engine* engine);
	
	virtual void Tick(float deltaTime) override;

public:
	static Lemon::Entity SelectEntity;

private:
	void DrawHierachyEntityTree();
};
