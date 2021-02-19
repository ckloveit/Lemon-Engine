#include "Editor.h"
#include <Core/Engine.h>

using namespace std;
using namespace Lemon;

Editor::~Editor()
{

}

void Editor::OnWindowMessage(WindowData& windowData)
{
	// During window creation, Windows fire off a couple of messages, m_initializing is to prevent that spamming.
	if (!m_Initialized)
	{
		// Create engine
		m_Engine = make_unique<Engine>(windowData);

		m_Initialized = true;
	}
	else if (m_Initialized)
	{
		// Updated ImGui with message (if showing)
		if (true)
		{
			/*ImGui_ImplWin32_WndProcHandler(
				static_cast<HWND>(window_data.handle),
				static_cast<uint32_t>(window_data.message),
				static_cast<int64_t>(window_data.wparam),
				static_cast<uint64_t>(window_data.lparam)
			);*/
		}

		// Passing zero dimensions will cause the swapchain to not present at all
		uint32_t width = static_cast<uint32_t>(windowData.Width);
		uint32_t height = static_cast<uint32_t>(windowData.Height);

		/*if (!_editor::swapchain->PresentEnabled() || _editor::swapchain->GetWidth() != width || _editor::swapchain->GetHeight() != height)
		{
			_editor::swapchain->Resize(static_cast<uint32_t>(width), static_cast<uint32_t>(height));
		}*/

		m_Engine->SetWindowData(windowData);
	}
}
void Editor::OnTick()
{
	// Engine - tick
	m_Engine->Tick();
}