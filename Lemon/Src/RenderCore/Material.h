#pragma once
#include "Core/Core.h"
#include <vector>
#include <glm/glm.hpp>
#include "RHI/RHI.h"
#include "RHI/DynamicRHI.h"
#include "RHI/RHIResources.h"

namespace Lemon
{
    class LEMON_API Material
    {
    public:
        Material();

        //=== Render Stage====//
        const Ref<RHIRasterizerState>& GetRasterizerState() const { return m_RasterizerState; }
        void SetRasterizerState(Ref<RHIRasterizerState>& state) { m_RasterizerState = state; }

        const Ref<RHIBlendState>& GetBlendState() const { return m_BlendState; }
        void SetBlendState(Ref<RHIBlendState>& state) { m_BlendState = state; }

        const Ref<RHIDepthStencilState>& GetDepthStencilState() const { return m_DepthStencilState; }
        void SetDepthStencilState(Ref<RHIDepthStencilState>& state) { m_DepthStencilState = state; }

        const EPrimitiveType GetPrimitiveType() const { return m_PrimitiveType; }
        void SetPrimitiveType(EPrimitiveType primitiveType) { m_PrimitiveType = primitiveType; }

        uint32_t GetTextureStartSlot() const { return m_TextureStartSlot; }
        std::vector<Ref<RHITexture>>& GetTextures() { return m_Textures; }
        const std::vector<Ref<RHITexture>> GetTextures() const { return m_Textures; }

    public:
		//PBR Material Properties
        glm::vec3 Albedo;
        float Metallic;
        float Roughness;
        float AO;

    private:
        // Render State
        Ref<RHIBlendState> m_BlendState = nullptr;
        Ref<RHIRasterizerState> m_RasterizerState = nullptr;
        Ref<RHIDepthStencilState> m_DepthStencilState = nullptr;
        EPrimitiveType m_PrimitiveType = EPrimitiveType::PT_TriangleList;
        // Textures
        uint32_t m_TextureStartSlot = 0;
        std::vector<Ref<RHITexture>> m_Textures;

    };
}