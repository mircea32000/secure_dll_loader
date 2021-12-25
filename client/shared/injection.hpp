#pragma once

#include <BlackBone/ManualMap/MMap.h>
#include <BlackBone/Process/Process.h>
#include <BlackBone/Misc/Utils.h>
#include <BlackBone/Patterns/PatternSearch.h>
#include <cstddef>
#include <TlHelp32.h>
#include <ProcessSnapshot.h>

#pragma comment(lib, "BlackBone.lib")

//std::string dll_name{};
//
//    std::cout << "Enter dll name" << endl;
//    std::cin >> dll_name;
//
//    auto pids = Process::EnumByName(L"csgo.exe");
//    std::string path = get_exe_path().substr(0, get_exe_path().find_last_of("\\")) + ("\\" + dll_name + ".dll");
//    Process prc;
//    if (!pids.empty())
//    {
//        prc.Attach(pids.front());
//    }
//    else
//    {
//        cout << "wtf goin on in miami bruh\n";
//        return -1;
//    }
//    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pids.front());
//
//    if (ntOpenFile) {
//        char originalBytes[5];
//        memcpy(originalBytes, ntOpenFile, 5);
//        WriteProcessMemory(hProcess, ntOpenFile, originalBytes, 5, NULL);
//    }
//    else
//    {
//        cout << "Unable to bypass :(\n";
//    }
//
//    prc.mmap().MapImage(blackbone::Utils::UTF8ToWstring(path), ManualImports);
//
//
//    CloseHandle(hProcess);
//
//    return -1;

namespace shared::injection
{
	inline DWORD find_pid(const std::wstring& processName)
	{

		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof(processInfo);

		HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (processesSnapshot == INVALID_HANDLE_VALUE)
			return 0;



		Process32First(processesSnapshot, &processInfo);

		if (!processName.compare(processInfo.szExeFile))
		{

			CloseHandle(processesSnapshot);


			return processInfo.th32ProcessID;

		}

		while (Process32Next(processesSnapshot, &processInfo))
		{

			if (!processName.compare(processInfo.szExeFile))
			{


				CloseHandle(processesSnapshot);

				return processInfo.th32ProcessID;

			}
		}

		CloseHandle(processesSnapshot);

		return 0;
	}

	inline void inject(std::string_view procName, std::string_view response, bool bypassntopenfile = false, DWORD pId = 0)
	{
		using namespace blackbone;

		auto pids = Process::EnumByName(Utils::UTF8ToWstring(procName.data()));
		LPVOID nt_open_file = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");

		Process prc;
		if (pId == 0)
		{
			if (!pids.empty())
				prc.Attach(pids.front());
			else
				abort();
		}
		else
			prc.Attach(pId);

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId == 0 ? pids.front() : pId);

		while (!prc.modules().GetModule(L"serverbrowser.dll"))
		{
			std::this_thread::sleep_for(std::chrono::seconds(10));
		}

		if (bypassntopenfile)
		{
			if (nt_open_file) {
				char originalBytes[5];
				memcpy(originalBytes, nt_open_file, 5);
				WriteProcessMemory(hProcess, nt_open_file, originalBytes, 5, NULL);
			}
			else
			{
				MessageBoxA(NULL, "Couldn't inject: injection bypass failure\n", "qjawshook", MB_OK | MB_ICONEXCLAMATION);
				abort();
			}

			const auto patch_jump_condition = [&](blackbone::ModuleDataPtr&& module_data, blackbone::PatternSearch&& pattern)
			{
				if (!module_data.get())
				{
					MessageBoxA(NULL, "Obtaining necessary module failed.\n", "qjawshook", MB_OK | MB_ICONEXCLAMATION);
					abort();
				}

				std::vector<blackbone::ptr_t> results{};

				pattern.SearchRemote(prc, 0xCC, module_data->baseAddress, module_data->size, results);

				if (results.empty() || results.size() > 1ul)
				{
					MessageBoxA(NULL, "Byte sequence matching failed.\n", "qjawshook", MB_OK | MB_ICONEXCLAMATION);
					abort();
				}

				const blackbone::ptr_t patch_address{ results.front() };

				const uint8_t original_byte{ prc.memory().Read<uint8_t>(patch_address) };
				constexpr uint8_t patch_byte{ 0xEB };

				prc.memory().Write(patch_address, patch_byte);

				return patch_address;
			};

			patch_jump_condition(prc.modules().GetModule(L"client.dll"), { 0x74, 0xCC, 0x68, 0xCC, 0xCC, 0xCC, 0xCC, 0x50, 0xFF, 0x15, 0xCC, 0xCC, 0xCC, 0xCC, 0x8B, 0xF0 });
		}

		std::vector<byte> bytes = { response.begin(), response.end() };

		const auto backup = [&]() -> void
		{
			if (nt_open_file) {
				char originalBytes[5];
				memcpy(originalBytes, nt_open_file, 5);
				WriteProcessMemory(hProcess, nt_open_file, originalBytes, 0, NULL);
			}
			else
			{
				MessageBoxA(NULL, "Couldn't inject: injection bypass failure\n", "qjawshook", MB_OK | MB_ICONEXCLAMATION);
				abort();
			}
		};
		//investigate manualimports
		if (prc.mmap().MapImage(bytes.size(), bytes.data(), NoFlags))
		{
			if (bypassntopenfile)
				backup();
		}
		else if (bypassntopenfile)
			backup();


		CloseHandle(hProcess);
	}

	inline bool is_process_running(std::string procName)
	{
		using namespace blackbone;
		auto pids = Process::EnumByName(Utils::UTF8ToWstring(procName));

		if (pids.empty())
			return false;

		HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

		PROCESSENTRY32W entry{};
		entry.dwSize = sizeof(entry);

		if (Process32FirstW(snapshot, &entry))
		{
			do
			{
				if (entry.th32ProcessID == pids.front())
				{
					return true;
				}
			} while (Process32NextW(snapshot, &entry));
		}

		return false;
	}

	inline DWORD wait_for_process(const char* process_name_hash, const std::chrono::milliseconds& time_out)
	{
		DWORD pid{};

		const std::chrono::milliseconds time_out_target
		{
			std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch())
			+
			time_out
		};

		do
		{
			HANDLE snapshot{ CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) };

			PROCESSENTRY32W entry{};
			entry.dwSize = sizeof(entry);

			if (Process32FirstW(snapshot, &entry))
			{
				do
				{
					if (strcmp(blackbone::Utils::WstringToUTF8(entry.szExeFile).c_str(), process_name_hash))
					{
						pid = entry.th32ProcessID;
						break;
					}
				} while (Process32NextW(snapshot, &entry));
			}
		} while (!pid && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()) < time_out_target);

		return pid;
	}

}