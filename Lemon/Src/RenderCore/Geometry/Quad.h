#pragma once
#include "Core/Core.h"
#include "RenderCore/Mesh.h"

namespace Lemon
{
	class LEMON_API Quad : public Mesh
	{
	public:
		Quad(float quadSize = 1.0f);

	private:
		void BuildQuad(std::vector<StandardMeshVertex>* vertices, std::vector<uint32_t>* indices);

	private:
		float m_QuadSize = 1.0f;
	};
}
