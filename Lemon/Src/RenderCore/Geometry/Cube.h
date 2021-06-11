#pragma once
#include "Core/Core.h"
#include "RenderCore/Mesh.h"

namespace Lemon
{
	class LEMON_API Cube : public Mesh
	{
	public:
		Cube(float cubeSize = 1.0f, bool bCompileDefaultShader = true);

	private:
		void BuildCube(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices);

	private:
		float m_CubeSize = 1.0f;
	};
}
