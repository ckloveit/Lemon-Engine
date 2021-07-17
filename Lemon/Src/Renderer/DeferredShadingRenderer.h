#pragma once

#include "SceneRenderer.h"

namespace Lemon
{
	class DeferredShadingRenderer : public SceneRenderer
	{
	public:
		DeferredShadingRenderer(const ViewInfo& InViewInfo)
			: SceneRenderer(InViewInfo)
		{ }

		virtual void Render(Ref<RHICommandList> RHICmdList) override;


	private:
		void PreDepthPass(Ref<RHICommandList> RHICmdList);
		void GBufferGeometryPass(Ref<RHICommandList> RHICmdList);
		void GBufferLightingPass(Ref<RHICommandList> RHICmdList);



	};
}
