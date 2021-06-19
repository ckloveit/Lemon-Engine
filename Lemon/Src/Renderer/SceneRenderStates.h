#pragma once
#include "Core/Core.h"
#include "RHI/RHI.h"
#include "RHI/RHICommandList.h"
#include "RHI/RHIResources.h"

namespace Lemon
{
    class LEMON_API SceneRenderStates
    {
    public:
        SceneRenderStates() = default;
        ~SceneRenderStates() = default;
    public:
        void Allocate();
        void SetGlobalSampler(Ref<RHICommandList> CmdList);

    private:
        void AllocateRasterizerState();
        void AllocateSamplerState();
        
    public:
        Ref<RHIRasterizerState> WireframeCullFrontRasterizerState;
        Ref<RHIRasterizerState> WireframeCullBackRasterizerState;
        Ref<RHIRasterizerState> SolidCullFrontRasterizerState;
        Ref<RHIRasterizerState> SolidCullBackRasterizerState;

        Ref<RHISamplerState> PointClampedSamplerState;
        Ref<RHISamplerState> BilinerClampedSamplerState;
        Ref<RHISamplerState> TrilinearClampedSamplerState;
        Ref<RHISamplerState> PointWrapSamplerState;
        Ref<RHISamplerState> BilinerWrapSamplerState;
        Ref<RHISamplerState> TrilinearWrapSamplerState;
        
        
    };
}
