#pragma once
#include "RHI/DynamicRHI.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/RHIDefinitions.h"
#include "D3D11Resources.h"

#include <wrl/client.h>
#include <glm/glm.hpp>

namespace Lemon
{
	class Renderer;

	class D3D11DynamicRHI : public DynamicRHI
	{
		friend class D3D11CommandList;

	public:
		D3D11DynamicRHI();

		virtual void Init() override;

		/** Shutdown the RHI; handle shutdown and resource destruction before the RHI's actual destructor is called (so that all resources of the RHI are still available for shutdown). */
		virtual void Shutdown()  override;


		//===================Begin RHI Methods ==================//
		virtual Ref<RHICommandList> RHICreateCommandList(Renderer* renderer) override;
		

		virtual Ref<RHISwapChain> RHICreateSwapChain(void* windowHandle, const uint32_t width, const uint32_t height, const ERHIPixelFormat format) override;

		virtual Ref<RHITexture2D> RHICreateTexture2D(uint32_t sizeX, uint32_t sizeY, ERHIPixelFormat format, uint32_t numMips, uint32_t flags, RHIResourceCreateInfo& CreateInfo) override;

		virtual Ref<RHIVertexBuffer> RHICreateVertexBuffer(uint32_t size, uint32_t usage, RHIResourceCreateInfo& createInfo) override;

		virtual Ref<RHIIndexBuffer> RHICreateIndexBuffer(uint32_t size, uint32_t usage, RHIResourceCreateInfo& createInfo) override;
		
		virtual Ref<RHIUniformBufferBase> RHICreateUniformBuffer(uint32_t size, const std::string& uniformBufferName) override;

		virtual Ref<RHIVertexShader> RHICreateVertexShader(const std::string& filePath, const std::string& entryPoint, RHIShaderCreateInfo& createInfo) override;

		virtual Ref<RHIPixelShader> RHICreatePixelShader(const std::string& filePath, const std::string& entryPoint, RHIShaderCreateInfo& createInfo) override;

		virtual Ref<RHIVertexDeclaration> RHICreateVertexDeclaration(Ref<RHIVertexShader> vertexShader, const VertexDeclarationElementList& Elements) override;

		
		//========Just Debug
		virtual void RHIClearRenderTarget(Ref<RHISwapChain> swapChain, glm::vec4 backgroundColor) override;

		//===================End RHI Methods ==================//

		ID3D11Device5* GetDevice() const { return m_Direct3DDevice.Get(); }
		ID3D11DeviceContext4* GetDeviceContext() const { return m_Direct3DDeviceIMContext.Get(); }

	private:
		void InitD3DDevice();



	private:
		ComPtr<ID3D11Device5> m_Direct3DDevice;
		ComPtr<ID3D11DeviceContext4> m_Direct3DDeviceIMContext;




	};

}