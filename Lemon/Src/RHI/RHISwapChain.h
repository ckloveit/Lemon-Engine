#pragma once
#include "Core/Core.h"
#include "RHI.h"

namespace Lemon
{
	class LEMON_API RHISwapChain
	{
	public:
		virtual ~RHISwapChain() = default;

		virtual bool Present() = 0;

		virtual void* GetRHISwapChain() = 0;
		virtual void* GetRHIRenderTargetView() = 0;



	};
}