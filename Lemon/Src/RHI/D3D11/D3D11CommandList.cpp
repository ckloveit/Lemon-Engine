#include "LemonPCH.h"
#include "D3D11CommandList.h"
#include <glm/gtc/type_ptr.hpp>
#include "RHI/RHITexture.h"

namespace Lemon
{
	D3D11CommandList::D3D11CommandList(D3D11DynamicRHI* D3D11RHI, Renderer* renderer)
		: RHICommandList(renderer)
		, m_D3D11RHI(D3D11RHI)
	{

	}

	//===================================RHI RenderCommand Helper function==================================//

	void D3D11CommandList::RHIClearRenderTarget(Ref<RHITexture2D> renderTarget, glm::vec4 backgroundColor)
	{
		const void* rtvs[1] = { renderTarget->GetNativeRenderTargetView() };
		/*m_D3D11RHI->GetDeviceContext()->OMSetRenderTargets
		(
			static_cast<UINT>(1),
			reinterpret_cast<ID3D11RenderTargetView * const*>(&rtvs),
			nullptr
		);*/
		m_D3D11RHI->GetDeviceContext()->ClearRenderTargetView((ID3D11RenderTargetView*)renderTarget->GetNativeRenderTargetView(), glm::value_ptr(backgroundColor));
	}
	//=======================================================================================================//
}