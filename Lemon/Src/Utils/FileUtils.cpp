#include "LemonPCH.h"
#include <fstream>
#include "FileUtils.h"

namespace Lemon
{
	std::string FileUtils::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			LEMON_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::wstring FileUtils::StringToWstring(const std::string& str /*= CP_ACP*/)
	{
		std::wstring wstr;
		int len = MultiByteToWideChar(0, 0, str.c_str(), (int)strlen(str.c_str()), NULL, 0);
		wchar_t* m_wchar = new wchar_t[len + 1];
		MultiByteToWideChar(0, 0, str.c_str(), (int)strlen(str.c_str()), m_wchar, len);
		m_wchar[len] = '\0';
		wstr = m_wchar;
		delete m_wchar;
		return wstr;
	}

	std::string FileUtils::WstringToString(const std::wstring& wstr /*= CP_ACP*/)
	{
		std::string str;
		int len = WideCharToMultiByte(0, 0, wstr.c_str(), (int)wcslen(wstr.c_str()), NULL, 0, NULL, NULL);
		char* m_char = new char[len + 1];
		WideCharToMultiByte(0, 0, wstr.c_str(), (int)wcslen(wstr.c_str()), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		str = m_char;
		delete m_char;
		return str;
	}
}