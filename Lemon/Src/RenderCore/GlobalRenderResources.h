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
		static void Init();
		static GlobalRenderResources* GetInstance() { return s_Instance; }
	private:
		GlobalRenderResources() = default;
		~GlobalRenderResources()
		{
			delete s_Instance;
			s_Instance = nullptr;
		};

	public:
		VertexDeclarationElementList StandardMeshVertexDeclarationElementList;
		std::shared_ptr<RHIVertexBuffer> FullScreenVertexBuffer;
		std::shared_ptr<RHIIndexBuffer> FullScreenIndexBuffer;

	private:
		static GlobalRenderResources* s_Instance;
	};
}