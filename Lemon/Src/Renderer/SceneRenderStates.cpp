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
        TrilinearClampedSamplerState = TStaticSamplerState<SF_Trilinear>::CreateRHI();

        PointWrapSamplerState = TStaticSamplerState<SF_Point, AM_Wrap, AM_Wrap, AM_Wrap>::CreateRHI();
        BilinerWrapSamplerState = TStaticSamplerState<SF_Bilinear, AM_Wrap, AM_Wrap, AM_Wrap>::CreateRHI();
        TrilinearWrapSamplerState = TStaticSamplerState<SF_Trilinear, AM_Wrap, AM_Wrap, AM_Wrap>::CreateRHI();
    }


    void SceneRenderStates::SetGlobalSampler(Ref<RHICommandList> CmdList)
    {
        CmdList->SetSamplerState(0, PointClampedSamplerState);
        CmdList->SetSamplerState(1, BilinerClampedSamplerState);
        CmdList->SetSamplerState(2, TrilinearClampedSamplerState);
        CmdList->SetSamplerState(3, PointWrapSamplerState);
        CmdList->SetSamplerState(4, BilinerWrapSamplerState);
        CmdList->SetSamplerState(5, TrilinearWrapSamplerState);
    }
}
