#pragma once
//= INCLUDES ==================
#include <array>
#include "Core/ISystem.h"
#include "Core/Engine.h"
#include <glm/glm.hpp>
//=============================

namespace Lemon
{
	enum KeyCode
	{
		// Keyboard
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,/*Function*/
		Alpha0, Alpha1, Alpha2, Alpha3, Alpha4, Alpha5, Alpha6, Alpha7, Alpha8, Alpha9,/*Numbers*/
		Keypad0, Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9,/*Numpad*/
		Q, W, E, R, T, Y, U, I, O, P,/*Letters*/
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		Esc,/*Controls*/
		Tab,
		Shift_Left, Shift_Right,
		Ctrl_Left, Ctrl_Right,
		Alt_Left, Alt_Right,
		Space,
		CapsLock,
		Backspace,
		Enter,
		Delete,
		Arrow_Left, Arrow_Right, Arrow_Up, Arrow_Down,
		Page_Up, Page_Down,
		Home,
		End,
		Insert,

		// Mouse
		Mouse_Left,
		Mouse_Middle,
		Mouse_Right,

		// Gamepad
		DPad_Up,
		DPad_Down,
		DPad_Left,
		DPad_Right,
		Button_A,
		Button_B,
		Button_X,
		Button_Y,
		Start,
		Back,
		Left_Thumb,
		Right_Thumb,
		Left_Shoulder,
		Right_Shoulder
	};

	class LEMON_API InputSystem : public ISystem
	{
	public:
		InputSystem(Engine* engine);
		~InputSystem() = default;

		void OnWindowData(WindowData windowData);
		//= ISubsystem ======================
		void Tick(float delta_time) override;
		
		//===================================

		void EndOneFrame() { m_bNewframe = true; }
		
		// Keys
		bool GetKey(const KeyCode key) const { return m_Keys[static_cast<uint32_t>(key)]; }							        // Returns true while the button identified by KeyCode is held down.
		bool GetKeyDown(const KeyCode key) const { return GetKey(key) && !m_KeysPreviousFrame[static_cast<uint32_t>(key)]; }	// Returns true during the frame the user pressed down the button identified by KeyCode.
		bool GetKeyUp(const KeyCode key) const { return !GetKey(key) && m_KeysPreviousFrame[static_cast<uint32_t>(key)]; }	// Returns true the first frame the user releases the button identified by KeyCode.

		// Mouse
		const glm::vec2& GetMousePosition() const { return m_MousePosition; }
		const glm::vec2& GetMouseDelta() const { return m_MouseDelta; }

	private:
		// Keys
		std::array<bool, 99> m_Keys;
		std::array<bool, 99> m_KeysPreviousFrame;
		uint32_t m_StartIndexMouse = 83;

		// Mouse
		glm::vec2 m_MousePosition = { 0, 0 };
		glm::vec2 m_MouseDelta = { 0, 0 };
		int m_MouseWheel = 0;
		float m_MouseWheelDelta = 0;

		// Misc
		bool m_bNewframe = false;

	};
}
