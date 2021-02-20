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


private:
	Lemon::Renderer* m_Renderer = nullptr;

};