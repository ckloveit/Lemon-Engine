#pragma once
#include "Core/Core.h"
#include "RenderCore/Mesh.h"

namespace Lemon
{
	class CameraComponent;
	class LEMON_API GridGizmo : public Mesh
	{
	public:
		GridGizmo();

		static void ComputeWorldAndScaleWithSnap(const CameraComponent& camera, glm::vec3& outTranslation, glm::vec3& outScale);

	private:
		void BuildGrid(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices);


	private:
		uint32_t m_GridHeight;
		uint32_t m_GridWidth;
	};
}
