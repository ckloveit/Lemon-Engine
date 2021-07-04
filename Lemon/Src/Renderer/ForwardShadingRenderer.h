#pragma once

#include "SceneRenderer.h"

namespace Lemon
{
	class ForwardShadingRenderer : public SceneRenderer
	{
	public:
		ForwardShadingRenderer(const ViewInfo& InViewInfo) 
			: SceneRenderer(InViewInfo)
		{ }
		
		virtual void Render(Ref<RHICommandList> RHICmdList) override;

	};
}
