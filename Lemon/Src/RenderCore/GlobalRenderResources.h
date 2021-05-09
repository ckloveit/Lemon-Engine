#pragma once
#include "Core/Core.h"
#include <vector>
#include "RHI/RHI.h"
#include "RHI/RHIResources.h"

namespace Lemon
{
	class RHIVertexDeclaration;

	class LEMON_API GlobalRenderResources
	{
	public:
		GlobalRenderResources() { s_Instance = this; };
		~GlobalRenderResources() { s_Instance = nullptr; };

		void Init();

		static GlobalRenderResources* GetInstance() { return s_Instance; }

	public:
		VertexDeclarationElementList StandardMeshVertexDeclarationElementList;
		std::shared_ptr<RHIVertexBuffer> FullScreenVertexBuffer;
		std::shared_ptr<RHIIndexBuffer> FullScreenIndexBuffer;

	private:
		static GlobalRenderResources* s_Instance;
	};
}