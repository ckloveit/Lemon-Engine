#pragma once
#include "Core/Core.h"
#include "World/Entity.h"

namespace Lemon
{
	class LEMON_API IComponent
	{
	public:
		Entity m_Entity;
	};
}