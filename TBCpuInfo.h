#pragma once

#ifndef _CPU_INFO_H_
#define _CPU_INFO_H_


#include <string>
#include <sstream>

// intrinsics
#if defined(__GNUC__)    // GCC
#include <cpuid.h>
#include <cstring>
#elif defined(_MSC_VER)    // MSVC
#if _MSC_VER >=1400    // VC2005
#include <Windows.h>
#include <intrin.h>
#include <tlhelp32.h>
#endif    // #if _MSC_VER >=1400
#else
#error Only supports MSVC or GCC.
#endif    // #if defined(__GNUC__)

namespace thatboy
{
	void getCpuIdEx(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue)
	{
#if defined(__GNUC__)    // GCC
		__cpuid_count(InfoType, ECXValue, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#elif defined(_MSC_VER)    // MSVC
#if defined(_WIN64) || _MSC_VER>=1600    // 64位下不支持内联汇编. 1600: VS2010, 据说VC2008 SP1之后才支持__cpuidex.
		__cpuidex(reinterpret_cast<int*>(CPUInfo), InfoType, ECXValue);
#else
		if (NULL == CPUInfo)    return;
		_asm {
			// load. 读取参数到寄存器.
			mov edi, CPUInfo;    // 准备用edi寻址CPUInfo
			mov eax, InfoType;
			mov ecx, ECXValue;
			// CPUID
			cpuid;
			// save. 将寄存器保存到CPUInfo
			mov[edi], eax;
			mov[edi + 4], ebx;
			mov[edi + 8], ecx;
			mov[edi + 12], edx;
		}
#endif
#endif    // #if defined(__GNUC__)
	}

	void getCpuId(unsigned int CPUInfo[4], unsigned int InfoType)
	{
#if defined(__GNUC__)    // GCC
		__cpuid(InfoType, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
#elif defined(_MSC_VER)    // MSVC
#if _MSC_VER>=1400    // VC2005才支持__cpuid
		__cpuid(reinterpret_cast<int*>(CPUInfo), InfoType);
#else
		getcpuidex(CPUInfo, InfoType, 0);
#endif
#endif    // #if defined(__GNUC__)
	}

	std::string getCpuVendor()
	{
		std::string vendor(13, '\0');
		unsigned int dwBuf[4];
		// Function 0: Vendor-ID and Largest Standard Function
		getCpuId(dwBuf, 0);
		// save. 保存到pvendor
		*reinterpret_cast<unsigned int*>(&vendor[0]) = dwBuf[1];    // ebx: 前四个字符.
		*reinterpret_cast<unsigned int*>(&vendor[4]) = dwBuf[3];    // edx: 中间四个字符.
		*reinterpret_cast<unsigned int*>(&vendor[8]) = dwBuf[2];    // ecx: 最后四个字符.
		vendor[12] = '\0';
		return vendor;
	}

	std::string getCpuBrand()
	{
		std::string vendor(49, '\0');
		unsigned int dwBuf[4];
		// Function 0x80000000: Largest Extended Function Number
		getCpuId(dwBuf, 0x80000000U);
		if (dwBuf[0] < 0x80000004U)    return 0;
		// Function 80000002h,80000003h,80000004h: Processor Brand String
		getCpuId(reinterpret_cast<unsigned int*>(&vendor[0]), 0x80000002U);    // 前16个字符.
		getCpuId(reinterpret_cast<unsigned int*>(&vendor[16]), 0x80000003U);    // 中间16个字符.
		getCpuId(reinterpret_cast<unsigned int*>(&vendor[32]), 0x80000004U);    // 最后16个字符.
		vendor[48] = '\0';
		return vendor;
	}

	std::string getCpuSerialId()
	{
		char serialId[33];
		unsigned int dwBuf[4];
		getCpuId(dwBuf, 1);
		dwBuf[1] &= 0X00FFFFFF;
		sprintf(serialId, "%08X%08X%08X%08X", dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3]);
		serialId[32] = '\0';
		return serialId;
	}

#ifdef _WIN32
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
			if (std::string("vmtoolsd").find(pe32.szExeFile) != std::string::npos)
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
#endif // _WIN32

	bool isInsideVirtualMachine()
	{
#ifdef _WIN32
		return checkSystemProductName() || checkVMtoolsd() || checkVMtoolsd();
#else
		FILE* pp = popen(R"(lspci | grep -i -c -G '\(\(Hyper-V\)\|\(VMware\)\)\|\(VirtualBox\)')", "r"); // build pipe
		if (!pp)
			return false;
		else
		{
			int num;
			fscanf(pp, "%d", &num);
			pclose(pp);
			return num > 0;
		}
#endif
		return true;
	}
}
#endif // !_CPU_INFO_H