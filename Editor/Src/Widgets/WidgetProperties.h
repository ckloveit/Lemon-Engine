#pragma once
#include "Widget.h"

class WidgetProperties : public Widget
{
public:
	WidgetProperties(Lemon::Engine* engine);
	virtual void Tick() override;

};