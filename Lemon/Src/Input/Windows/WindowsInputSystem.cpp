//= INCLUDES =======================
#include "LemonPCH.h"
#include "Input/Input.h"
#ifdef API_INPUT_WINDOWS
#include "Input/InputSystem.h"
#include "Core/Engine.h"
#include "Core/Core.h"
#include "Core/Delegate.h"
#include <sstream>
#include "Log/Log.h"
//==================================

//= NAMESPACES ===============
using namespace std;
//============================

namespace Lemon
{
	InputSystem::InputSystem(Engine* engine) : ISystem(engine)
	{
		const WindowData& windowData = m_Engine->GetWindowData();
		const auto windowHandle = static_cast<HWND>(windowData.Handle);

		if (!windowHandle)
		{
			LEMON_CORE_ERROR("Invalid window handle");
			return;
		}

		// Register mouse
		{
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif
			RAWINPUTDEVICE Rid[1];
			Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
			Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
			Rid[0].dwFlags = RIDEV_INPUTSINK;
			Rid[0].hwndTarget = windowHandle;
			RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
		}

		// Bind delegate
		m_Engine->OnWindowMessageEvent.AddDynamic(this, &InputSystem::OnWindowData);
	
	}
	
	void InputSystem::OnWindowData(WindowData windowData)
	{
		// OnWindowData can run multiple times per frame (for each window message)
		// So only code within the if statement scope will run once per frame
		if (m_bNewframe)
		{
			m_MouseDelta = { 0, 0 };
			m_KeysPreviousFrame = m_Keys;
		}

		HWND windowHandle = static_cast<HWND>(windowData.Handle);

		// Mouse
		{
			// Keys
			m_Keys[m_StartIndexMouse] = (::GetKeyState(VK_LBUTTON) & 0x8000) != 0; // Left button pressed
			m_Keys[(uint32_t)(m_StartIndexMouse + 1)] = (::GetKeyState(VK_MBUTTON) & 0x8000) != 0; // Middle button pressed
			m_Keys[(uint32_t)(m_StartIndexMouse + 2)] = (::GetKeyState(VK_RBUTTON) & 0x8000) != 0; // Right button pressed

			// Delta
			if (windowData.Message == WM_INPUT)
			{
				UINT dwSize = 48;
				static BYTE lpb[48];
				GetRawInputData((HRAWINPUT)windowData.Lparam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
				RAWINPUT* raw = (RAWINPUT*)lpb;
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					m_MouseDelta.x = static_cast<float>(raw->data.mouse.lLastX);
					m_MouseDelta.y = static_cast<float>(raw->data.mouse.lLastY);
					//LEMON_CORE_INFO("inputSystem->GetMouseDelta() = {0} , {1}", m_MouseDelta.x, m_MouseDelta.y);
				}
			}

			// Position
			if (windowHandle == ::GetActiveWindow())
			{
				POINT mouseScreenPos;
				if (::GetCursorPos(&mouseScreenPos))
				{
					::ScreenToClient(windowHandle, &mouseScreenPos);
					m_MousePosition = glm::vec2(static_cast<float>(mouseScreenPos.x), static_cast<float>(mouseScreenPos.y));
				}
			}

			// Wheel
			{
				int curWheel = ::GetScrollPos(windowHandle, SB_VERT);
				m_MouseWheelDelta = static_cast<float>(curWheel - m_MouseWheel);
				m_MouseWheel = curWheel;
			}
		}
		// KEYBOARD
		{
#define is_pressed(keyCode) (::GetKeyState(keyCode) & 0x8000) != 0

			// FUNCTION
			m_Keys[0] = is_pressed(VK_F1);
			m_Keys[1] = is_pressed(VK_F2);
			m_Keys[2] = is_pressed(VK_F3);
			m_Keys[3] = is_pressed(VK_F4);
			m_Keys[4] = is_pressed(VK_F5);
			m_Keys[5] = is_pressed(VK_F6);
			m_Keys[6] = is_pressed(VK_F7);
			m_Keys[7] = is_pressed(VK_F8);
			m_Keys[8] = is_pressed(VK_F9);
			m_Keys[9] = is_pressed(VK_F10);
			m_Keys[10] = is_pressed(VK_F11);
			m_Keys[11] = is_pressed(VK_F12);
			m_Keys[12] = is_pressed(VK_F13);
			m_Keys[13] = is_pressed(VK_F14);
			m_Keys[14] = is_pressed(VK_F15);
			// NUMBERS
			m_Keys[15] = is_pressed('0');
			m_Keys[16] = is_pressed('1');
			m_Keys[17] = is_pressed('2');
			m_Keys[18] = is_pressed('3');
			m_Keys[19] = is_pressed('4');
			m_Keys[20] = is_pressed('5');
			m_Keys[21] = is_pressed('6');
			m_Keys[22] = is_pressed('7');
			m_Keys[23] = is_pressed('8');
			m_Keys[24] = is_pressed('9');
			// KEYPAD
			m_Keys[25] = is_pressed(VK_NUMPAD0);
			m_Keys[26] = is_pressed(VK_NUMPAD1);
			m_Keys[27] = is_pressed(VK_NUMPAD2);
			m_Keys[28] = is_pressed(VK_NUMPAD3);
			m_Keys[29] = is_pressed(VK_NUMPAD4);
			m_Keys[30] = is_pressed(VK_NUMPAD5);
			m_Keys[31] = is_pressed(VK_NUMPAD6);
			m_Keys[32] = is_pressed(VK_NUMPAD7);
			m_Keys[33] = is_pressed(VK_NUMPAD8);
			m_Keys[34] = is_pressed(VK_NUMPAD9);
			// LETTERS
			m_Keys[35] = is_pressed('Q');
			m_Keys[36] = is_pressed('W');
			m_Keys[37] = is_pressed('E');
			m_Keys[38] = is_pressed('R');
			m_Keys[39] = is_pressed('T');
			m_Keys[40] = is_pressed('Y');
			m_Keys[41] = is_pressed('U');
			m_Keys[42] = is_pressed('I');
			m_Keys[43] = is_pressed('O');
			m_Keys[44] = is_pressed('P');
			m_Keys[45] = is_pressed('A');
			m_Keys[46] = is_pressed('S');
			m_Keys[47] = is_pressed('D');
			m_Keys[48] = is_pressed('F');
			m_Keys[49] = is_pressed('G');
			m_Keys[50] = is_pressed('H');
			m_Keys[51] = is_pressed('J');
			m_Keys[52] = is_pressed('K');
			m_Keys[53] = is_pressed('L');
			m_Keys[54] = is_pressed('Z');
			m_Keys[55] = is_pressed('X');
			m_Keys[56] = is_pressed('C');
			m_Keys[57] = is_pressed('V');
			m_Keys[58] = is_pressed('B');
			m_Keys[59] = is_pressed('N');
			m_Keys[60] = is_pressed('M');
			// CONTROLS
			m_Keys[61] = is_pressed(VK_ESCAPE);
			m_Keys[62] = is_pressed(VK_TAB);
			m_Keys[63] = is_pressed(VK_LSHIFT);
			m_Keys[64] = is_pressed(VK_RSHIFT);
			m_Keys[65] = is_pressed(VK_LCONTROL);
			m_Keys[66] = is_pressed(VK_RCONTROL);
			m_Keys[67] = is_pressed(VK_LMENU);
			m_Keys[68] = is_pressed(VK_RMENU);
			m_Keys[69] = is_pressed(VK_SPACE);
			m_Keys[70] = is_pressed(VK_CAPITAL);
			m_Keys[71] = is_pressed(VK_BACK);
			m_Keys[72] = is_pressed(VK_RETURN);
			m_Keys[73] = is_pressed(VK_DELETE);
			m_Keys[74] = is_pressed(VK_LEFT);
			m_Keys[75] = is_pressed(VK_RIGHT);
			m_Keys[76] = is_pressed(VK_UP);
			m_Keys[77] = is_pressed(VK_DOWN);
			m_Keys[78] = is_pressed(VK_PRIOR);
			m_Keys[79] = is_pressed(VK_NEXT);
			m_Keys[80] = is_pressed(VK_HOME);
			m_Keys[81] = is_pressed(VK_END);
			m_Keys[82] = is_pressed(VK_INSERT);
		}

		// Check for any device changes (e.g. gamepad connected)
		if (windowData.Message == WM_DEVICECHANGE)
		{
			//m_check_for_new_device = true;
		}

		m_bNewframe = false;
	}

	void InputSystem::Tick(float deltaTime)
	{
		//m_bNewframe = true;
	}

}

#endif