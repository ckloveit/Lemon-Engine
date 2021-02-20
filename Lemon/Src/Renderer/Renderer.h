#pragma once
#include "Core/Core.h"
#include "Core/ISystem.h"

#include <glm/glm.hpp>

#include "RHI/RHIViewport.h"
#include "RHI/RHI.h"

namespace Lemon
{
	class DynamicRHI;

	class LEMON_API Renderer : public ISystem
	{
	public:
		Renderer(Engine* engine);
		~Renderer();


		bool Initialize() override;
		void Tick(float deltaTime) override;


		//====SwapChain=============================//
		const auto& GetSwapChain() const { return m_RHISwapChain; }


	private:
		RHIViewport m_Viewport = { 0, 0, 1920, 1080 };
		Ref<RHISwapChain> m_RHISwapChain;

	};
}
