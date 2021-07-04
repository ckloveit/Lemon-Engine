#pragma once
#include "RHI/RHICommandList.h"

namespace Lemon
{
	struct ViewInfo
	{
		Viewport ViewSize;



	};

	class SceneRenderer
	{
	public:
		SceneRenderer(const ViewInfo& InViewInfo) : m_ViewInfo(InViewInfo) {}
		virtual ~SceneRenderer() {}

		virtual void Render(Ref<RHICommandList> RHICmdList) = 0;

	protected:
		ViewInfo m_ViewInfo;
	};

}
