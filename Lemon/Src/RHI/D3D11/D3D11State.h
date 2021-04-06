#include "Core/Core.h"
#include "RHI/RHI.h"
#include "RHI/RHIResources.h"
#include "RHI/D3D11/D3D11RHI.h"
#include "RHI/D3D11/D3D11DynamicRHI.h"

namespace Lemon
{
    class D3D11SamplerState : public RHISamplerState
    {
    public:
        D3D11SamplerState(const SamplerStateInitializer& Init)
           :RHISamplerState(Init)
           ,m_Resource(nullptr)
        {
            
        }
        virtual ~D3D11SamplerState()
        {
            D3D11::SafeRelease(m_Resource);
        }
		void* GetNativeResource() const override { return m_Resource; }
    public:
        ID3D11SamplerState* m_Resource;
    };
    
    class D3D11DepthStencilState : public RHIDepthStencilState
    {
    public:
        D3D11DepthStencilState(const DepthStencilStateInitializer& Init)
            :RHIDepthStencilState(Init)
            ,m_Resource(nullptr)
        {
            
        }
        virtual ~D3D11DepthStencilState()
        {
            D3D11::SafeRelease(m_Resource);
        }
		void* GetNativeResource() const override { return m_Resource; }
    public:
        ID3D11DepthStencilState* m_Resource;
        DepthStencilOperation AccessType;
    };

    class D3D11BlendState : public RHIBlendState
    {
    public:
        D3D11BlendState(const BlendStateInitializer& Init)
            :RHIBlendState(Init)
            ,m_Resource(nullptr)
        {
            
        }
        virtual ~D3D11BlendState()
        {
            D3D11::SafeRelease(m_Resource);
        }
		void* GetNativeResource() const override { return m_Resource; }
    public:
        ID3D11BlendState* m_Resource;
    };

    class D3D11RasterizerState : public RHIRasterizerState
    {
    public:
        D3D11RasterizerState(const RasterizerStateInitializer& initializer)
            :RHIRasterizerState(initializer)
            ,m_Resource(nullptr)
        {
            
        }
        virtual ~D3D11RasterizerState()
        {
            D3D11::SafeRelease(m_Resource);
        }
		void* GetNativeResource() const override { return m_Resource; }
    public:
        ID3D11RasterizerState* m_Resource;
    };
    
}
