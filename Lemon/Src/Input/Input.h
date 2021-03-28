#pragma once

//= INCLUDES ==================
#include "Core/Core.h"
//=============================

#ifdef API_INPUT_WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#pragma comment(lib, "XInput.lib")
	#include <windows.h>
	#include <xinput.h>
#endif
