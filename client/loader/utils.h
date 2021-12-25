#pragma once
#include <string>
#include <locale>
#include <codecvt>
namespace utils
{
	inline bool compare_strings(std::string_view proc_name, WCHAR* proc_info)
	{
		char doodoo[128];

		WideCharToMultiByte(CP_UTF8, 0, proc_info, -1, doodoo, std::wstring_view(proc_info).length() + 1, NULL, NULL);

        return (proc_name.compare(std::string_view(doodoo)));
	}

	namespace details
	{
		inline HWND g_hwnd;
	}
}