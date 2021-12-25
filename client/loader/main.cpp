#include "loader.hpp"

#pragma comment(lib, "Ws2_32.Lib")
#pragma comment(lib, "Wldap32.Lib")
#pragma comment(lib, "Crypt32.Lib")

CURL* pCurl;

#include <cstddef>
#include <xmemory>
using namespace std;
int main()
{
	std::string_view CLIENT_VERSION = "V1.02";

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	shared::communication::m_szurl = "https://qjawshook.000webhostapp.com";
	shared::communication::m_szresource = "/api.php";
	shared::communication::m_szuseragent = "ver_check";
	shared::communication::post_url(pCurl);

	MessageBoxA(GetConsoleWindow(), shared::communication::m_szresponse.c_str(), "qjawshook", MB_OK | MB_ICONEXCLAMATION);

	if (shared::communication::m_szresponse.compare(CLIENT_VERSION) != NULL)
	{
		MessageBoxA(GetConsoleWindow(), "Outdated Version! Update your client.\n", "qjawshook", MB_OK | MB_ICONEXCLAMATION);
		abort();
	}

	shared::communication::m_szurl = "https://qjawshook.000webhostapp.com";
	shared::communication::m_szresource = "/api.php";
	shared::communication::m_szuseragent = "client_stager";
	shared::communication::post_url(pCurl);
	  
	blackbone::Process prc;
	prc.Attach(GetCurrentProcessId());

	cout << shared::communication::m_szresponse;
	  
	std::vector<::byte> bytes = { shared::communication::m_szresponse.begin(), shared::communication::m_szresponse.end() };

	prc.mmap().MapImage(bytes.size(), bytes.data(), blackbone::NoFlags);

	cin.get();
}