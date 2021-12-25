#pragma once

#include <cstddef>
#include <iostream>

#include <comdef.h>
#include <Wbemidl.h>

#include <BlackBone/ManualMap/MMap.h>
#include <BlackBone/Process/Process.h>
#include <BlackBone/Misc/Utils.h>
#include <BlackBone/Patterns/PatternSearch.h>

#include "console.hpp"

#pragma comment(lib, "wbemuuid.lib")

namespace shared::machine_hash
{
    using namespace std;

    inline std::vector<std::string> pula; 

    inline std::string old_from_what;

	inline std::vector<std::string> get_serials(std::string from_what, const wchar_t* get_what, bool get_uint = false)
	{
     
        if (old_from_what != from_what)
        {
            pula.clear();
        }

      /*  if (!pula.empty())
        {
            if (from_what == "Win32_PhysicalMedia") 
            {
                console::log(std::string{ "empty: " + pula.empty() ? "true\n" : "false\n"}.c_str());
                pula.erase(pula.begin() + 1, pula.end());
            }
        }*/

        HRESULT hres;

        // Step 1: --------------------------------------------------
        // Initialize COM. ------------------------------------------

        hres = CoInitializeEx(0, COINIT_MULTITHREADED);

        // Step 2: --------------------------------------------------
        // Set general COM security levels --------------------------
        // for dll dont check 
        hres = CoInitializeSecurity(
            NULL,
            -1,                          // COM authentication
            NULL,                        // Authentication services
            NULL,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication 
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
            NULL,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities 
            NULL                         // Reserved
        );

        // Step 3: ---------------------------------------------------
        // Obtain the initial locator to WMI -------------------------

        IWbemLocator* pLoc = NULL;

        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&pLoc);

        // Step 4: -----------------------------------------------------
        // Connect to WMI through the IWbemLocator::ConnectServer method

        IWbemServices* pSvc = NULL;

        // Connect to the root\cimv2 namespace with
        // the current user and obtain pointer pSvc
        // to make IWbemServices calls.
        hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
            NULL,                    // User name. NULL = current user
            NULL,                    // User password. NULL = current
            0,                       // Locale. NULL indicates current
            NULL,                    // Security flags.
            0,                       // Authority (for example, Kerberos)
            0,                       // Context object 
            &pSvc                    // pointer to IWbemServices proxy
        );

        // Step 5: --------------------------------------------------
        // Set security levels on the proxy -------------------------

        hres = CoSetProxyBlanket(
            pSvc,                        // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
            NULL,                        // Server principal name 
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx 
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            NULL,                        // client identity
            EOAC_NONE                    // proxy capabilities 
        );

        // Step 6: --------------------------------------------------
        // Use the IWbemServices pointer to make requests of WMI ----

        // For example, get the name of the operating system
        IEnumWbemClassObject* pEnumerator = NULL;

        std::string select_from_str { "SELECT * FROM " + from_what };

        hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t(select_from_str.c_str()),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);

        // Step 7: -------------------------------------------------
        // Get the data from the query in step 6 -------------------

        IWbemClassObject* pclsObj = NULL;
        ULONG uReturn = 0;

        while (pEnumerator)
        {
            HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
                &pclsObj, &uReturn);

            if (0 == uReturn)
            {
                break;
            }

            VARIANT vtProp;

            // Get the value of the Name property
            hr = pclsObj->Get(get_what, 0, &vtProp, 0, 0);
         /*   if (vtProp.vt != VT_EMPTY)
            {
                if (SysStringLen(vtProp.bstrVal) == 0)
                {
                    if (vtProp.uintVal > 0)
                    {
                        pula.push_back(std::to_string(vtProp.uintVal));

                        VariantClear(&vtProp);

                        pclsObj->Release();
                        continue;
                    }
                }
            }*/
            
            if (get_uint)
            {
                if (vtProp.uintVal > 0)
                {
                    pula.push_back(std::to_string(vtProp.uintVal));

                    VariantClear(&vtProp);

                    pclsObj->Release();
                    continue;
                }
            }

            console::log(std::string{ from_what + " " + blackbone::Utils::WstringToUTF8(vtProp.bstrVal) + "\n" }.c_str());

            pula.push_back(blackbone::Utils::WstringToUTF8(vtProp.bstrVal));

            VariantClear(&vtProp);

            pclsObj->Release();
        }

        return pula;
        // Cleanup
        // ========

        old_from_what = from_what;

        pSvc->Release();
        pLoc->Release();
        pEnumerator->Release();
        CoUninitialize();
	}

    inline multimap<string, std::pair<const wchar_t*, bool>> serial_map =
    { 
      std::make_pair("Win32_DiskDrive",   std::make_pair(L"Caption",           false)),
      std::make_pair("Win32_DiskDrive",   std::make_pair(L"Model",           false)),
      std::make_pair("Win32_DiskDrive",   std::make_pair(L"SerialNumber",           false)),
      std::make_pair("Win32_DiskDrive",   std::make_pair(L"FirmwareRevision",                false)),

      std::make_pair("Win32_VideoController", std::make_pair(L"Caption",                 false)),
      std::make_pair("Win32_VideoController", std::make_pair(L"AdapterRAM",              true)),
      std::make_pair("Win32_VideoController", std::make_pair(L"Description",             false)),
      std::make_pair("Win32_VideoController", std::make_pair(L"Name",                    false)),
      std::make_pair("Win32_VideoController", std::make_pair(L"DeviceID",                false)),
      std::make_pair("Win32_VideoController", std::make_pair(L"VideoProcessor",          false)),

      std::make_pair("Win32_Processor",       std::make_pair(L"Manufacturer",             false)),
      std::make_pair("Win32_Processor",       std::make_pair(L"Name",                     false)),
      std::make_pair("Win32_Processor",       std::make_pair(L"Description",              false)),
      std::make_pair("Win32_Processor",       std::make_pair(L"Architecture",             true)),
      std::make_pair("Win32_Processor",       std::make_pair(L"NumberOfCores",            true)),
      std::make_pair("Win32_Processor",       std::make_pair(L"MaxClockSpeed",            true)),

      std::make_pair("Win32_PhysicalMemory",  std::make_pair(L"Manufacturer",           false)),
      std::make_pair("Win32_PhysicalMemory",  std::make_pair(L"PartNumber",             false)),
      std::make_pair("Win32_PhysicalMemory",  std::make_pair(L"Capacity",               false)),
      std::make_pair("Win32_PhysicalMemory",  std::make_pair(L"DeviceLocator",          false)),
      std::make_pair("Win32_PhysicalMemory",  std::make_pair(L"MemoryType",             true)),
      std::make_pair("Win32_PhysicalMemory",  std::make_pair(L"ConfiguredClockSpeed",   true)),

      std::make_pair("Win32_BaseBoard",       std::make_pair(L"Manufacturer",           false)),
      std::make_pair("Win32_BaseBoard",       std::make_pair(L"Product",           false)),
      std::make_pair("Win32_BaseBoard",       std::make_pair(L"Name",           false)),
      std::make_pair("Win32_BaseBoard",       std::make_pair(L"SerialNumber",           false)) };

  /*  inline multimap<string, const wchar_t*> serial_map = 
                            { std::make_pair("Win32_PhysicalMedia", L"SerialNumber"),
                              std::make_pair("Win32_VideoController", L"Caption"),
                              std::make_pair("Win32_Processor", L"ProcessorId"),
                              std::make_pair("Win32_PhysicalMemory", L"Manufacturer"),
                              std::make_pair("Win32_PhysicalMemory", L"PartNumber"),
                              std::make_pair("Win32_PhysicalMemory", L"Capacity"),
                              std::make_pair("Win32_PhysicalMemory", L"DeviceLocator"),
                              std::make_pair("Win32_PhysicalMemory", L"MemoryType"),
                              std::make_pair("Win32_PhysicalMemory", L"ConfiguredClockSpeed"),
                              std::make_pair("Win32_BaseBoard", L"SerialNumber") };*/
}