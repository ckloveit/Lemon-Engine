#pragma once
#include <Windows.h>
#include <functional>

#include <Core/Engine.h>
#include <Utils/FileUtils.h>


namespace Window
{
	static HINSTANCE g_Instance;
	static HWND g_Handle;
	static std::function<void(Lemon::WindowData& windowData)> g_MessageCallback;

	inline void GetWindowSize(float* width, float* height)
	{
		RECT rect;
		::GetClientRect(g_Handle, &rect);
		*width = static_cast<float>(rect.right - rect.left);
		*height = static_cast<float>(rect.bottom - rect.top);
	}

	// Window Procedure
	inline LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT result = 0;

		Lemon::WindowData windowData = {};
		windowData.Handle = static_cast<void*>(g_Handle);
		windowData.hInstance = static_cast<void*>(g_Instance);
		windowData.Message = static_cast<uint32_t>(msg);
		windowData.Wparam = static_cast<uint64_t>(wParam);
		windowData.Lparam = static_cast<int64_t>(lParam);
		GetWindowSize(&windowData.Width, &windowData.Height);

		if (msg == WM_DISPLAYCHANGE || msg == WM_SIZE)
		{
			windowData.Width = static_cast<float>(lParam & 0xffff);
			windowData.Height = static_cast<float>((lParam >> 16) & 0xffff);
		}
		else if (msg == WM_CLOSE)
		{
			PostQuitMessage(0);
		}
		else
		{
			result = DefWindowProc(hwnd, msg, wParam, lParam);
		}

		// process g_MessageCallback
		if (g_MessageCallback)
		{
			g_MessageCallback(windowData);
		}
		return result;
	}

	inline bool Create(HINSTANCE instance, const std::string& title)
	{
		g_Instance = instance;
		const std::wstring windowTitle = Lemon::FileUtils::StringToWstring(title);
		const int windowWidth = 1600;// GetSystemMetrics(SM_CXSCREEN);
		const int windowHeight = 900;// GetSystemMetrics(SM_CYSCREEN);
		const LPCWSTR className = L"MyLemonWindowClass";

		// Register the Window Class
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = g_Instance;
		wc.hIcon = LoadIcon(instance, IDI_APPLICATION);
		wc.hCursor = LoadCursor(instance, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = className;
		wc.hIconSm = LoadIcon(instance, IDI_APPLICATION);

		if (!RegisterClassEx(&wc))
		{
			MessageBox(nullptr, L"Window registration failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		// Create the Window
		g_Handle = CreateWindowEx
		(
			WS_EX_CLIENTEDGE,
			className,
			windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight,
			nullptr, nullptr, g_Instance, nullptr
		);

		if (!g_Handle)
		{
			MessageBox(nullptr, L"Window creation failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		return true;
	}

	inline void Show()
	{
		ShowWindow(g_Handle, SW_SHOWNORMAL);
		UpdateWindow(g_Handle);
		SetFocus(g_Handle);
	}

	inline bool Tick()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				return false;
		}

		return true;
	}

	inline void Destroy()
	{
		DestroyWindow(g_Handle);
	}

}