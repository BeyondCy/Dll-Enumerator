#include <stdio.h>
#include <windows.h>
#include <psapi.h>

#define WIN32_LEAN_AND_MEAN

#define DEBUG printf("DEBUG - %ld\r\n", GetLastError());

DWORD dwError;
HANDLE hFile;

#define SW SysWait();
VOID SysWait(VOID)
{
     printf("Press any key to exit.\r\n");
     dwError = getch();
}

BOOL Processf(DWORD dwProcessId);
BOOL InitializeLogPath(VOID);

VOID ProcessWrite(TCHAR *lpString);

INT __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
     //
     DWORD dwProcesses[1024] = {0};
     DWORD dwX = 0;
     DWORD dwY;
     
     if(!InitializeLogPath())
          goto FAILURE;
     
     if(!EnumProcesses(dwProcesses, sizeof(dwProcesses), &dwY))
          goto FAILURE;
     
     dwError = dwY / sizeof(DWORD);
     
     for(;dwX < dwError; dwX++)
     {
          if(dwProcesses[dwX] != 0)
               if(!Processf(dwProcesses[dwX]))
                    goto FAILURE;
     }
    
     SW
     CloseHandle(hFile);
     return ERROR_SUCCESS;
    
    
FAILURE:

     DEBUG
     CloseHandle(hFile);
     SW
     return GetLastError();
}

BOOL Processf(DWORD dwProcessId)
{
     TCHAR szProcessName[MAX_PATH] = TEXT("<UNKNOWN>");
     TCHAR szModuleName[MAX_PATH] = {0};
     HMODULE hMod[1024];
     DWORD dwX = 0;
     HANDLE hProcess;
     
     DWORD dwY;
     TCHAR szWrite[1024] = {0};

     
     hProcess = OpenProcess(PROCESS_ALL_ACCESS,
                            FALSE, dwProcessId);
                            
     if(hProcess != NULL)
     {
          if(!EnumProcessModules(hProcess, hMod, sizeof(hMod), &dwY))
          {
               sprintf(szWrite, "\r\nUnable to OPEN process %s (PID: %u) - Error %ld\r\n\r\n", szProcessName, dwProcessId, GetLastError());
               ProcessWrite(szWrite);                                                          
          }
               
          else{
               GetModuleBaseName(hProcess, hMod[dwX], szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
               sprintf(szWrite, "%s (PID: %u)\r\n", szProcessName, dwProcessId);
               ProcessWrite(szWrite);
               
               
               if(GetModuleFileNameEx(hProcess, NULL, szModuleName, sizeof(szModuleName)/sizeof(TCHAR)))
               sprintf(szWrite, "%s\r\n", szModuleName);
               ProcessWrite(szWrite);
               
               for(;dwX < (dwY / sizeof(HMODULE)); dwX++)
               {
                    if(GetModuleFileNameEx(hProcess, hMod[dwX], szModuleName, sizeof(szModuleName)/sizeof(TCHAR)))
                    {
                         sprintf(szWrite, "\t%s (0x%08X)\r\n", szModuleName, hMod[dwX]);
                         ProcessWrite(szWrite);
                    }
               } 
          }
     }
     CloseHandle(hProcess);
     return TRUE;  
}

BOOL InitializeLogPath(VOID)
{
     //
     TCHAR szCurrentPath[MAX_PATH] = {0};
     
     dwError = GetCurrentDirectory(MAX_PATH, szCurrentPath);
     if(dwError == 0)
          return FALSE;
     else
          sprintf(szCurrentPath, "%s\\ProcessLog.txt", szCurrentPath);
          
     hFile = CreateFile(szCurrentPath, GENERIC_WRITE | GENERIC_READ , 
                        0, NULL, OPEN_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
                        
     if(hFile == INVALID_HANDLE_VALUE)
          return FALSE;
     else{
          dwError = SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
          if(dwError == INVALID_SET_FILE_POINTER)
               return FALSE;
     }
     
     return TRUE;
}

VOID ProcessWrite(TCHAR *lpString)
{
     DWORD dwWritten = 0;
     
     if(WriteFile(hFile, lpString, strlen(lpString), &dwWritten, NULL))
          puts("Write successful.");
     else{
          puts("Write unsuccessful.");
     }
          
     ZeroMemory(lpString, sizeof(lpString));  
}
