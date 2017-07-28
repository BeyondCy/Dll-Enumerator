# DllEnum
NOTE: This application was compiled using the DEV-C++ IDE (formally known as Bloodshed)

This application attempts to enumerate all processes currently running. If it can successfully get a handle on the process in question it will attempt to traverse further and harvest all loaded DLLs loaded from the process.

0. On load the application creates a log path. This log path will be placed in the current directory that this executable is running from. It will get the directory then append ProcessLog.txt to it. This results in a string like: C:\Users\...\Documents\...\ProcessUsers\ProcessLog.txt

1. The application has a static DWORD array (DWORD dwProcesses[1024]). If the log path function is successful, the function calls EnumProcesses. If the call is successful the dwProcesses array is an array of process ID's.

2. If dwProcesses is not empty, Processf function iterates through all of the process IDs. On each process it will attempt to get a handle on the process via OpenProcess. If OpenProcess is successful the function calls EnumProcessModules. EnumProcessModules stores its data into a static array of module handles (HMOD hMod[1024]). 

3. If the application successfully loads all of the associated modules from the process ID, we get the module string from GetModuleBaseName and write the name to the log file.

4. Following the call to WriteFile for the module name we call GetModuleFileNameEx to get the full path to the module name. This is written to the log file.

5. Finally, we iterate through the hMod array and write their base names to the log file. Once the function call to Processf is complete, all associated handles are closed using CloseHandle.