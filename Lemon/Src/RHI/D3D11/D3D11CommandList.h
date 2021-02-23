#pragma once
#include "Core/Core.h"
#include "D3D11DynamicRHI.h"
#include "RHI/RHICommandList.h"

namespace Lemon
{
	class Renderer;
	class LEMON_API D3D11CommandList : public RHICommandList
	{
	public:
		D3D11CommandList(D3D11DynamicRHI* D3D11RHI, Renderer* renderer);

		//===================================RHI RenderCommand Helper function==================================//
		virtual void RHIClearRenderTarget(Ref<RHITexture2D> renderTarget, glm::vec4 backgroundColor) override;

		//=======================================================================================================//
	private:
		D3D11DynamicRHI* m_D3D11RHI;
	};
}