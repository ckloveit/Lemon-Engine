#pragma once
#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//Engine Common
#include "Log/Log.h"
#include "Utils/FileUtils.h"

// Profiler

#ifdef LEMON_PLATFORM_WINDOW
	#include <Windows.h>
#endif