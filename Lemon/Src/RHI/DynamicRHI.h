#pragma once
#include "Core/Core.h"
#include "RHIDeviceAdapter.h"
#include "RHI.h"
#include "RHIResources.h"
#include "glm/glm.hpp"

namespace Lemon
{
	class RHITexture2D;
	class RHICommandList;
	class Renderer;
	class RHIVertexBuffer;
	class RHIIndexBuffer;
	class RHIVertexShader;
	class RHIPixelShader;
	class RHIVertexDeclaration;
	class RHIBoundShaderState;
	class RHICommandContext;

	class LEMON_API DynamicRHI
	{
	public:
		/** Declare a virtual destructor, so the dynamic RHI can be deleted without knowing its type. */
		virtual ~DynamicRHI() {}

		virtual void Init() = 0;

		/** Shutdown the RHI; handle shutdown and resource destruction before the RHI's actual destructor is called (so that all resources of the RHI are still available for shutdown). */
		virtual void Shutdown() = 0;

		//===================Begin RHI Methods ==================//
		virtual Ref<RHICommandList> RHICreateCommandList(Renderer* renderer) = 0;

		virtual Ref<RHISwapChain> RHICreateSwapChain(void* windowHandle, const uint32_t width, const uint32_t height, const ERHIPixelFormat format) = 0;

		virtual Ref<RHITexture2D> RHICreateTexture2D(uint32_t sizeX, uint32_t sizeY, ERHIPixelFormat format, uint32_t numMips, uint32_t createFlags, RHIResourceCreateInfo& CreateInfo) = 0;
		
		virtual Ref<RHITextureCube> RHICreateTextureCube(uint32_t sizeX, uint32_t sizeY, ERHIPixelFormat format, uint32_t numMips, uint32_t createFlags, RHITextureCubeCreateInfo& CreateInfo) = 0;

		//=========Buffers========//
		virtual Ref<RHIVertexBuffer> RHICreateVertexBuffer(uint32_t size, uint32_t usage, RHIResourceCreateInfo& createInfo) = 0;

		virtual Ref<RHIIndexBuffer> RHICreateIndexBuffer(uint32_t size, uint32_t usage, RHIResourceCreateInfo& createInfo) = 0;

		virtual Ref<RHIUniformBufferBase> RHICreateUniformBuffer(uint32_t size, const std::string& uniformBufferName) = 0;
		
		//=========Shaders=========//
		virtual Ref<RHIVertexShader> RHICreateVertexShader(const std::string& filePath, const std::string& entryPoint,RHIShaderCreateInfo& createInfo) = 0;

		virtual Ref<RHIPixelShader> RHICreatePixelShader(const std::string& filePath, const std::string& entryPoint, RHIShaderCreateInfo& createInfo) = 0;

		//=========Shaders Binds=========//
		virtual Ref<RHIVertexDeclaration> RHICreateVertexDeclaration(Ref<RHIVertexShader> vertexShader, const VertexDeclarationElementList& Elements) = 0;


		//======DepthStencil State======//
		virtual Ref<RHIDepthStencilState> RHICreateDepthStencilState(const DepthStencilStateInitializer& initializer) = 0;

		//=====Blend State===============//
		virtual Ref<RHIBlendState> RHICreateBlendState(const BlendStateInitializer& initializer) = 0;

		//====RasterizerState====//
		virtual Ref<RHIRasterizerState> RHICreateRasterizerState(const RasterizerStateInitializer& initializer) = 0;

		//===SamplerState=======//
		virtual Ref<RHISamplerState> RHICreateSamplerState(const SamplerStateInitializer& initializer) = 0;
		
		// ====Texture Settings====//
		virtual void SetMipTexture(Ref<RHITextureCube> targetTex, int mipIndex, int mipWidth, int mipHeight, std::vector<Ref<RHITexture2D>> mipTextures) = 0;


		//========Just Debug
		virtual void RHIClearRenderTarget(Ref<RHISwapChain> swapChain, glm::vec4 backgroundColor) = 0;
		//===================End RHI Methods ==================//

	public:
		//===================Begin Device Adapter======================//
		void AddDeviceAdapter(const RHIDeviceAdapter& deviceAdapter);
		RHIDeviceAdapter* GetMainDeviceAdapter();
		void SetMainDeviceAdapter(const uint32_t index);
		const std::vector<RHIDeviceAdapter>& GetDeviceAdapters() const { return m_DeviceAdapters; }
		//===================End Device Adapter========================//
		RHICommandContext& GetCommandContext() const { return *m_CommandContext; }
	protected:
		std::vector<RHIDeviceAdapter> m_DeviceAdapters;
		uint32_t m_MainDeviceAdapterIndex = 0;
		Ref<RHICommandContext> m_CommandContext;
	};

	extern LEMON_API DynamicRHI* g_DynamicRHI;

	/**
	*	Each platform that utilizes dynamic RHIs should implement this function
	*	Called to create the instance of the dynamic RHI.
	*/
	DynamicRHI* PlatformCreateDynamicRHI();

	//===================================RHI Resource Create Helper function==================================//

	FORCEINLINE Ref<RHICommandList> RHICreateCommandList(Renderer* renderer)
	{
		return g_DynamicRHI->RHICreateCommandList(renderer);
	}

	FORCEINLINE Ref<RHISwapChain> RHICreateSwapChain(void* windowHandle, const uint32_t width, const uint32_t height, const ERHIPixelFormat format)
	{
		return g_DynamicRHI->RHICreateSwapChain(windowHandle, width, height, format);
	}

	FORCEINLINE Ref<RHITexture2D> RHICreateTexture2D(uint32_t sizeX, uint32_t sizeY, ERHIPixelFormat format, uint32_t numMips, uint32_t createFlags, RHIResourceCreateInfo& CreateInfo)
	{
		return g_DynamicRHI->RHICreateTexture2D(sizeX, sizeY, format, numMips, createFlags, CreateInfo);
	}

	FORCEINLINE Ref<RHITextureCube> RHICreateTextureCube(uint32_t sizeX, uint32_t sizeY, ERHIPixelFormat format, uint32_t numMips, uint32_t createFlags, RHITextureCubeCreateInfo& CreateInfo)
	{
		return g_DynamicRHI->RHICreateTextureCube(sizeX, sizeY, format, numMips, createFlags, CreateInfo);
	}

	FORCEINLINE Ref<RHIVertexBuffer> RHICreateVertexBuffer(uint32_t size, uint32_t usage, RHIResourceCreateInfo& createInfo)
	{
		return g_DynamicRHI->RHICreateVertexBuffer(size, usage, createInfo);
	}

	FORCEINLINE Ref<RHIIndexBuffer> RHICreateIndexBuffer(uint32_t size, uint32_t usage, RHIResourceCreateInfo& createInfo)
	{
		return g_DynamicRHI->RHICreateIndexBuffer(size, usage, createInfo);
	}

	FORCEINLINE Ref<RHIVertexShader> RHICreateVertexShader(const std::string& filePath, const std::string& entryPoint, RHIShaderCreateInfo& createInfo)
	{
		return g_DynamicRHI->RHICreateVertexShader(filePath, entryPoint, createInfo);
	}

	FORCEINLINE Ref<RHIPixelShader> RHICreatePixelShader(const std::string& filePath, const std::string& entryPoint, RHIShaderCreateInfo& createInfo)
	{
		return g_DynamicRHI->RHICreatePixelShader(filePath, entryPoint, createInfo);
	}

	FORCEINLINE Ref<RHIVertexDeclaration> RHICreateVertexDeclaration(Ref<RHIVertexShader> vertexShader, const VertexDeclarationElementList& Elements)
	{
		return g_DynamicRHI->RHICreateVertexDeclaration(vertexShader, Elements);
	}

	FORCEINLINE void RHIClearRenderTarget(Ref<RHISwapChain> swapChain, glm::vec4 backgroundColor)
	{
		return g_DynamicRHI->RHIClearRenderTarget(swapChain, backgroundColor);
	}
	
	//======DepthStencil State======//
	FORCEINLINE Ref<RHIDepthStencilState> RHICreateDepthStencilState(const DepthStencilStateInitializer& initializer)
	{
		return g_DynamicRHI->RHICreateDepthStencilState(initializer);
	}

	template<typename T>
	Ref<RHIUniformBufferBase> RHICreateUniformBuffer(const std::string& uniformBufferName)
	{
		uint32_t size = GetValidateUniformBufferSize(static_cast<uint32_t>(sizeof(T)));
		return g_DynamicRHI->RHICreateUniformBuffer(size, uniformBufferName);
	}

	//=====Blend State===============//
	FORCEINLINE Ref<RHIBlendState> RHICreateBlendState(const BlendStateInitializer& initializer)
	{
		return g_DynamicRHI->RHICreateBlendState(initializer);
	}

	//====RasterizerState====//
	FORCEINLINE Ref<RHIRasterizerState> RHICreateRasterizerState(const RasterizerStateInitializer& initializer)
	{
		return g_DynamicRHI->RHICreateRasterizerState(initializer);
	}

	//===SamplerState=======//
	FORCEINLINE Ref<RHISamplerState> RHICreateSamplerState(const SamplerStateInitializer& initializer)
	{
		return g_DynamicRHI->RHICreateSamplerState(initializer);
	}

}

