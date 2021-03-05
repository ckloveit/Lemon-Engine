#include "LemonPCH.h"
#include "D3D11CommandList.h"
#include <glm/gtc/type_ptr.hpp>
#include "RHI/RHIResources.h"
#include "RHI/RHISwapChain.h"

#include <DirectXMath.h>
using namespace DirectX;

namespace Lemon
{
	const D3D11_INPUT_ELEMENT_DESC VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HRESULT CreateShaderFromFile(
		const WCHAR* csoFileNameInOut,
		const WCHAR* hlslFileName,
		LPCSTR entryPoint,
		LPCSTR shaderModel,
		ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		// 寻找是否有已经编译好的顶点着色器
		if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
		{
			return hr;
		}
		else
		{
			DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
			// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
			// 但仍然允许着色器进行优化操作
			dwShaderFlags |= D3DCOMPILE_DEBUG;

			// 在Debug环境下禁用优化以避免出现一些不合理的情况
			dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
			ID3DBlob* errorBlob = nullptr;
			hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
				dwShaderFlags, 0, ppBlobOut, &errorBlob);
			if (FAILED(hr))
			{
				if (errorBlob != nullptr)
				{
					OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
				}
				D3D11::SafeRelease(errorBlob);
				return hr;
			}

			// 若指定了输出文件名，则将着色器二进制信息输出
			if (csoFileNameInOut)
			{
				return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
			}
		}
		return hr;
	}


	D3D11CommandList::D3D11CommandList(D3D11DynamicRHI* D3D11RHI, Renderer* renderer)
		: RHICommandList(renderer)
		, m_D3D11RHI(D3D11RHI)
	{
		// set the triangle 
		VertexPosColor vertices[] =
		{
			{ XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
		};
		D3D11_BUFFER_DESC vbd;
		ZeroMemory(&vbd, sizeof(vbd));
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = sizeof vertices;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory(&InitData, sizeof(InitData));
		InitData.pSysMem = vertices;
		(m_D3D11RHI->GetDevice()->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));


		ComPtr<ID3DBlob> blob;

		// create vertex shader
		(CreateShaderFromFile(L"HLSL\\Triangle_VS.cso", L"HLSL\\Triangle_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
		(m_D3D11RHI->GetDevice()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
		// create vertex input layout
		(m_D3D11RHI->GetDevice()->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout),
			blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));

		// create pixel shader
		(CreateShaderFromFile(L"HLSL\\Triangle_PS.cso", L"HLSL\\Triangle_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
		(m_D3D11RHI->GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));
		// 
		UINT stride = sizeof(VertexPosColor);	// vertex data structure size
		UINT offset = 0;						// offset

		m_D3D11RHI->GetDeviceContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
		// set Draw Primitive Type
		m_D3D11RHI->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_D3D11RHI->GetDeviceContext()->IASetInputLayout(m_pVertexLayout.Get());
	
	/*	D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
		D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
		D3D11SetDebugObjectName(m_pVertexShader.Get(), "Trangle_VS");
		D3D11SetDebugObjectName(m_pPixelShader.Get(), "Trangle_PS");*/

		m_D3D11RHI->GetDeviceContext()->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
		m_D3D11RHI->GetDeviceContext()->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	}

	//===================================RHI RenderCommand Helper function==================================//

	void D3D11CommandList::RHIClearRenderTarget(Ref<RHITexture2D> renderTarget, glm::vec4 backgroundColor)
	{
		const void* rtvs[1] = { renderTarget->GetNativeRenderTargetView() };
		m_D3D11RHI->GetDeviceContext()->ClearRenderTargetView((ID3D11RenderTargetView*)renderTarget->GetNativeRenderTargetView(), glm::value_ptr(backgroundColor));
	}

	void D3D11CommandList::SetRenderTarget(Ref<RHISwapChain> swapChain)
	{
		const void* rtvs[1] = { swapChain->GetRHIRenderTargetView() };

		ID3D11RenderTargetView* renderTargetView = static_cast<ID3D11RenderTargetView*>(swapChain->GetRHIRenderTargetView());
		void* depthStencil = nullptr;
		m_D3D11RHI->GetDeviceContext()->OMSetRenderTargets
		(
			1,
			&renderTargetView,
			static_cast<ID3D11DepthStencilView*>(depthStencil)
		); 
		m_D3D11RHI->GetDeviceContext()->ClearRenderTargetView(renderTargetView, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	}

	void D3D11CommandList::SetGraphicsPipelineState(const GraphicsPipelineStateInitializer& GraphicsPSOInit)
	{
		D3D11VertexShader* vertexShader = D3D11ResourceCast(GraphicsPSOInit.BoundShaderState.VertexShaderRHI.get());
		D3D11PixelShader* pixelShader = D3D11ResourceCast(GraphicsPSOInit.BoundShaderState.PixelShaderRHI.get());
		D3D11VertexDeclaration* vertexDeclaration = D3D11ResourceCast(GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI.get());

		m_CurrentGraphicsPipelineState = GraphicsPSOInit;
		m_CurrentPrimitiveType = GraphicsPSOInit.PrimitiveType;

		// InputLayout
		if (vertexDeclaration)
		{
			m_D3D11RHI->GetDeviceContext()->IASetInputLayout(vertexDeclaration->m_InputLayout);
		}
		// Vertex shader
		if (vertexShader)
		{
			m_D3D11RHI->GetDeviceContext()->VSSetShader(vertexShader->m_Resource, nullptr, 0);
		}
		// Pixel shader
		if (pixelShader)
		{
			m_D3D11RHI->GetDeviceContext()->PSSetShader(pixelShader->m_Resource, nullptr, 0);
		}

		//Blend State

		//Depth Stencil State

		//Rasterizer State

		//PrimitiveTopology
		if (GraphicsPSOInit.PrimitiveType == EPrimitiveType::PT_TriangleList)
		{
			m_D3D11RHI->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
	}

	void D3D11CommandList::SetRenderTarget(Ref<RHITexture2D> colorTarget)
	{
		// render targets
		void* depthStencil = nullptr;
		{
			//TODO:
		}
		if (colorTarget)
		{
			void* renderTargetViews[1];
			renderTargetViews[0] = static_cast<ID3D11RenderTargetView*>(colorTarget->GetNativeRenderTargetView());
			
			m_D3D11RHI->GetDeviceContext()->OMSetRenderTargets
			(
				1,
				reinterpret_cast<ID3D11RenderTargetView *const*>(renderTargetViews),
				static_cast<ID3D11DepthStencilView*>(depthStencil)
			);

			m_D3D11RHI->GetDeviceContext()->ClearRenderTargetView(static_cast<ID3D11RenderTargetView*>(renderTargetViews[0]), glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
		}
	}

	void D3D11CommandList::SetViewport(const Viewport& viewport)
	{
		D3D11_VIEWPORT d3d11Viewport = {};
		d3d11Viewport.TopLeftX = viewport.X;
		d3d11Viewport.TopLeftY = viewport.Y;
		d3d11Viewport.Width = viewport.Width;
		d3d11Viewport.Height = viewport.Height;
		d3d11Viewport.MinDepth = viewport.DepthMin;
		d3d11Viewport.MaxDepth = viewport.DepthMax;
		m_D3D11RHI->GetDeviceContext()->RSSetViewports(1, &d3d11Viewport);
	}

	void D3D11CommandList::SetIndexBuffer(const RHIIndexBuffer* indexBuffer)
	{
		if (!indexBuffer || !indexBuffer->GetNativeResource())
		{
			LEMON_CORE_ERROR("Invalid SetIndexBuffer");
			return;
		}
		// if cache .we don't need set it again

		m_D3D11RHI->GetDeviceContext()->IASetIndexBuffer
		(
			static_cast<ID3D11Buffer*>(indexBuffer->GetNativeResource()),
			/*buffer->Is16Bit()*/false ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT,
			0
		);
	}

	void D3D11CommandList::SetVertexBuffer(int streamIndex, const RHIVertexBuffer* vertexBuffer)
	{
		if (!vertexBuffer || !vertexBuffer->GetNativeResource())
		{
			LEMON_CORE_ERROR("Invalid SetBufferVertex");
			return;
		}

		// if cache .we don't need set it again
		if (!m_CurrentGraphicsPipelineState.IsValid())
		{
			LEMON_CORE_ERROR("Empty Bound States");
			return;
		}
		ID3D11Buffer* buffer = static_cast<ID3D11Buffer*>(vertexBuffer->GetNativeResource());
		uint32_t stride = m_CurrentGraphicsPipelineState.BoundShaderState.VertexDeclarationRHI->m_StreamStrides[streamIndex];
		uint32_t offset = 0;

		m_D3D11RHI->GetDeviceContext()->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	}

	void D3D11CommandList::DrawIndexPrimitive(uint32_t VertexOffset, uint32_t IndexOffset, uint32_t NumPrimitives, uint32_t FirstInstance/* = 0*/, uint32_t NumInstances/* = 1*/)
	{
		uint32_t indexCount = GetIndexCountForPrimitiveCount(NumPrimitives, m_CurrentPrimitiveType);

		if (NumInstances > 1 || FirstInstance != 0)
		{
			m_D3D11RHI->GetDeviceContext()->DrawIndexedInstanced(indexCount, NumInstances, IndexOffset, VertexOffset, FirstInstance);
		}
		else
		{
			m_D3D11RHI->GetDeviceContext()->DrawIndexed(indexCount, IndexOffset, VertexOffset);
		}
	}
	void D3D11CommandList::Flush()
	{
		m_D3D11RHI->GetDeviceContext()->Flush();
	}


	void D3D11CommandList::DebugRenderer(Ref<RHITexture2D> colorTarget)
	{
		static float black[4] = { 0.1f, 0.4f, 0.7f, 1.0f };	// RGBA = (0,0,0,255)
		//ID3D11RenderTargetView* renderTargetView = static_cast<ID3D11RenderTargetView*>(colorTarget->GetNativeRenderTargetView());
		//m_D3D11RHI->GetDeviceContext()->ClearRenderTargetView(renderTargetView, black);
		//m_D3D11RHI->GetDeviceContext()->ClearDepthStencilView(nullptr, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		// 
		m_D3D11RHI->GetDeviceContext()->Draw(3, 0);
		//HR(m_pSwapChain->Present(0, 0));
	}

}