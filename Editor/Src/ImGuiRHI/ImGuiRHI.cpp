#include "LemonPCH.h"
#include "ImGuiRHI.h"
#include "RHI/RHISwapChain.h"
#include "RHI/DynamicRHI.h"
#ifdef LEMON_GRAPHICS_D3D11
#include "RHI/D3D11/D3D11DynamicRHI.h"
#include "Implementation/imgui_impl_dx11.h"
#endif

using namespace Lemon;

static Renderer* g_renderer = nullptr;

bool ImGuiRHI::Initialize(Renderer* renderer, const float width, const float height)
{
#if LEMON_GRAPHICS_D3D11
	D3D11DynamicRHI* D3D11RHI = (D3D11DynamicRHI*)g_DynamicRHI;
	ImGui_ImplDX11_Init(D3D11RHI->GetDevice(), D3D11RHI->GetDeviceContext());
#endif
	g_renderer = renderer;
	return true;
}

void ImGuiRHI::NewFrame()
{
#if LEMON_GRAPHICS_D3D11
	D3D11DynamicRHI* D3D11RHI = (D3D11DynamicRHI*)g_DynamicRHI;
	const void* resourceArray[1] = { g_renderer->GetSwapChain()->GetRHIRenderTargetView() };
	D3D11RHI->GetDeviceContext()->OMSetRenderTargets(1, reinterpret_cast<ID3D11RenderTargetView * const*>(&resourceArray), NULL);
	//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	//g_rhi_context->device_context->ClearRenderTargetView(g_rhi_context->g_mainRenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_NewFrame();
#endif
}

void ImGuiRHI::Shutdown()
{
	ImGui::DestroyPlatformWindows();
}

void ImGuiRHI::RenderDrawData(ImDrawData* draw_data)
{
#if LEMON_GRAPHICS_D3D11
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	g_renderer->GetSwapChain()->Present();
}
void ImGuiRHI::OnResize(const float width, const float height)
{
	if (!g_renderer || !g_renderer->GetSwapChain())
		return;
	g_renderer->GetSwapChain()->ReSize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
}
