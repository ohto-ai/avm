#include <cstdio>
#include <string>
#include <windows.h>
#include <tlhelp32.h>

//查询注册表，获取SystemProductName
std::string getSystemProductName() {
    HKEY hkey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, R"(HARDWARE\DESCRIPTION\System\BIOS)", 0, KEY_QUERY_VALUE, &hkey) != ERROR_SUCCESS)
        return "";
    char strValue[MAX_PATH] = { NULL };
    DWORD dwSize = sizeof(strValue);
    RegQueryValueEx(hkey, "SystemProductName", NULL, NULL, (LPBYTE)strValue, &dwSize);
    RegCloseKey(hkey);
    return strValue;
}

bool checkSystemProductName() {
    char VmName[][10] = { "Virtual", "KVM", "VMware", "HVM", "RHEV", "VMLite", "VBox" };
    std::string productName = getSystemProductName();
    for (auto& vmName : VmName)
        if (productName.find(vmName) != std::string::npos)
            return true;
    return false;
}

bool checkVMtoolsd()
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//拍摄快照
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }
    BOOL bMore = Process32First(hProcessSnap, &pe32);					  //遍历第一个进程
    while (bMore)
    {
        if (std::string("vmtoolsd").find(pe32.szExeFile)!=std::string::npos)
            return true;
        bMore = Process32Next(hProcessSnap, &pe32);					      //遍历下一个进程
    }
    CloseHandle(hProcessSnap);
    return false;
}

bool checkVMwareHostOpen()
{
    HKEY hkey;
    bool hasVMware = RegOpenKey(HKEY_CLASSES_ROOT, "Applications\\VMwareHostOpen.exe", &hkey) == ERROR_SUCCESS;
    if (hasVMware) RegCloseKey(hkey);
    return hasVMware;
}

bool isInsideVirtualMachine()
{
    return checkSystemProductName() || checkVMtoolsd() || checkVMtoolsd();
}

int main(void)
{
    puts(isInsideVirtualMachine() ? "Inside Virtual Machine" : "Not Inside Virtual Machine");
    return 0;
}