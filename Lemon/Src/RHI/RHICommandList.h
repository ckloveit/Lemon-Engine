#pragma once
#include "Core/Core.h"
#include "RHI.h"
#include "glm/glm.hpp"

namespace Lemon
{
	class Renderer;
	class RHITexture2D;

	class LEMON_API RHICommandList
	{
	public:
		RHICommandList(Renderer* renderer);

		//=====Static function====================
		static RHICommandList& Get();


		//===================================RHI RenderCommand Helper function==================================//
		virtual void RHIClearRenderTarget(Ref<RHITexture2D> renderTarget, glm::vec4 backgroundColor) = 0;

		//=======================================================================================================//
	private:
		static RHICommandList* s_Instance;
	};

}