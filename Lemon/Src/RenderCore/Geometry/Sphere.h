#pragma once
#include "Core/Core.h"
#include "RenderCore/Mesh.h"

namespace Lemon
{
	class LEMON_API Sphere : public Mesh
	{
	public:
		Sphere(float radius = 1.0f, bool bCompileDefaultShader = true, int slices = 20, int stacks = 20);

	private:
		void BuildSphere(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices);

	private:
		float m_Radius = 1.0f;
		int m_Slices = 20;
		int m_Stacks = 20;
	};
}
