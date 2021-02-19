#include "LemonPCH.h"
#include "FileUtils.h"

namespace Lemon
{
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