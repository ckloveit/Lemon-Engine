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

		static const glm::mat4& ComputeWorldMatrix(const CameraComponent& camera);

	private:
		void BuildGrid(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices);


	private:
		uint32_t m_GridHeight;
		uint32_t m_GridWidth;
	};
}
