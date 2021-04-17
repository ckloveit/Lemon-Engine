#pragma once
#include "Widget.h"

class WidgetSceneHierachy : public Widget
{
public:
	WidgetSceneHierachy(Lemon::Engine* engine);
	virtual void Tick(float deltaTime) override;

};