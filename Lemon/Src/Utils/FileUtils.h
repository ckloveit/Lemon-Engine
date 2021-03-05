#pragma once
#include "Core/Core.h"
#include <string>

namespace Lemon
{
	class LEMON_API FileUtils
	{
	public:
		static std::string ReadFile(const std::string& filepath);

		// String & WString Convert
		static std::wstring StringToWstring(const std::string& str);
		static std::string WstringToString(const std::wstring& str);
	};

}