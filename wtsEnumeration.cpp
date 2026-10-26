// wtsEnumeration.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <WtsApi32.h>

#pragma comment (lib,"wtsapi32.lib")
std::wstring GetUserNameFromSid(PSID sid) //allows us to use strings in a safe way.
{
    WCHAR name[32], domain[32];
    DWORD lname = _countof(name);
    DWORD ldomain = _countof(domain);
    SID_NAME_USE use;
    if (!::LookupAccountSid(nullptr, sid, name, &lname, domain, &ldomain, &use))
    {
        return L"";
    }
    return std::wstring(domain) + L"\\"+ name;
}

int main()
{
    DWORD level = 1;
    DWORD count;
    PWTS_PROCESS_INFO_EX info;
    if (!WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &level, WTS_ANY_SESSION, (LPWSTR*)&info, &count))
    {
        return 1;
    }

    for(DWORD i = 0; i < count; i++)
    {
        PWTS_PROCESS_INFO_EX proc = info + i; 
        printf("PID: % u Thread % u Session : % u % ws(username : % ws \n", proc->ProcessId, proc->NumberOfThreads, proc->SessionId, proc->pProcessName, GetUserNameFromSid(proc->pUserSid).c_str()); //so long damnn
    }

    WTSFreeMemoryEx(WTSTypeSessionInfoLevel1, info, count);
    return 0;
}
