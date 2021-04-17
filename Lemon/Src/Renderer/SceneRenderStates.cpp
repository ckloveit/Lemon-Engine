#include "LemonPCH.h"
#include "SceneRenderStates.h"

#include "RHI/RHIStaticStates.h"

namespace Lemon
{
    void SceneRenderStates::Allocate()
    {
        AllocateRasterizerState();
        AllocateSamplerState();
        
    }

    void SceneRenderStates::AllocateRasterizerState()
    {
        WireframeCullFrontRasterizerState = TStaticRasterizerState<RFM_Wireframe, RCM_Front>::CreateRHI();
        WireframeCullBackRasterizerState = TStaticRasterizerState<RFM_Wireframe, RCM_Back>::CreateRHI();
        SolidCullFrontRasterizerState = TStaticRasterizerState<RFM_Solid, RCM_Front>::CreateRHI();
        SolidCullBackRasterizerState = TStaticRasterizerState<RFM_Solid, RCM_Back>::CreateRHI();
        
    }

    void SceneRenderStates::AllocateSamplerState()
    {
        PointClampedSamplerState = TStaticSamplerState<SF_Point>::CreateRHI();
        BilinerClampedSamplerState = TStaticSamplerState<SF_Bilinear>::CreateRHI();
    }


    void SceneRenderStates::SetGlobalSampler(Ref<RHICommandList> CmdList)
    {
        CmdList->SetSamplerState(0, PointClampedSamplerState);
        CmdList->SetSamplerState(1, BilinerClampedSamplerState);
        
    }
}
