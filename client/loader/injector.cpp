// injector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <string>
#include <TlHelp32.h>
#include "utils.h"
#include <filesystem>

#include <BlackBone/ManualMap/MMap.h>
#include <BlackBone/Process/Process.h>
#include <BlackBone/Misc/Utils.h>
#include <ios>
//#pragma comment(lib, "BlackBone.lib")

using namespace std;
using namespace blackbone;



std::string get_exe_path()
{
    char result[MAX_PATH];
    return std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
}

//void Backup()
//{
//    if (ntOpenFile) {
//
//        char originalBytes[5];
//        memcpy(originalBytes, ntOpenFile, 5);
//        WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 0, NULL);
//    }
//    else
//    {
//        cout << "Unable to backup\n";
//        Sleep(2000);
//        exit(-1);
//    }
//}

//int main()
//{
//    std::string dll_name{};
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
//}
//
