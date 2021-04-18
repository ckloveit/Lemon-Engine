#pragma once
#include "Widget.h"
#include "World/World.h"
#include "World/Entity.h"

class WidgetProperties : public Widget
{
public:
	WidgetProperties(Lemon::Engine* engine);
	virtual void Tick(float deltaTime) override;

private:
	void DrawEntity(Lemon::Entity entity) const;
	
};
