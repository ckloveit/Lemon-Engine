#pragma once
#include "Core/Core.h"
#include <glm/glm.hpp>

namespace Lemon::Math
{
	const float PI = 3.1415926f;

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);
}