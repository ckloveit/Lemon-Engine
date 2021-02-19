//= INCLUDES ======
#include "Window.h"
#include "Editor.h"
//=================

int main(int argc, char** argv)
{
	// Create editor
	Editor editor;

	// Create window
	HINSTANCE hInstance = GetModuleHandle(0);
	Window::Create(hInstance, "Lemon " + std::string("V01"));
	Window::Show();

	// Hook it up with the editor
	Window::g_MessageCallback = [&editor](Lemon::WindowData& windowData) { editor.OnWindowMessage(windowData); };

	// Tick
	while (Window::Tick())
	{
		editor.OnTick();
	}

	// Exit
	Window::Destroy();
	return 0;
}
