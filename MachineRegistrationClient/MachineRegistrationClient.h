#pragma once
#ifndef _MACHINEREGISTRATION_H_
#define _MACHINEREGISTRATION_H_

#ifndef REGISTRATION_CODE_API

#ifdef __GNUC__
#define REGISTRATION_CODE_API extern "C"
#define _STDCALL
#else
#ifdef _MSC_VER
#define _STDCALL _stdcall
#if defined MACHINEREGISTRATION_EXPORTS || defined MACHINEREGISTRATIONCLIENT_EXPORTS
#define REGISTRATION_CODE_API extern "C" __declspec(dllexport)
#else
#define REGISTRATION_CODE_API extern "C" __declspec(dllimport)
#endif
#endif // _MSVC_VER_
#endif // __GNUC__
#endif // !REGISTRATIONCODEAPI

#if defined MACHINEREGISTRATION_EXPORTS || defined MACHINE_REG_ADMINISTRATORS_CALL	// for admin call

/**
	注册
	param machineId, 机器序列号
	param envirment, 用于结合MachineId生成注册码 XXXXX-XXXXX-XXXXX-XXXXX
	param registerCode, 存储注册码
	return keyValue, 用于宿主程序的密钥【重要】
*/
REGISTRATION_CODE_API int _STDCALL RegisterClient(const char* machineId, const char* envirment, char* registerCode);
/**
	验证
	注册
	param machineId, 机器序列号
	param envirment, 用于结合MachineId生成注册码 XXXXX-XXXXX-XXXXX-XXXXX
	param registerCode, 客户提供的注册码
	return keyValue, 用于宿主程序的密钥【重要】
*/
REGISTRATION_CODE_API int _STDCALL VerifyClient(const char* machineId, const char* envirment, const char* registerCode);
#endif // MACHINEREGISTRATION_EXPORTS

/**
	验证
	param envirment, 用于结合MachineId生成注册码，以便校对
	param registerCode, 客户提供的注册码
	return keyValue, 用于宿主程序的密钥【重要】，校验失败返回0
*/
REGISTRATION_CODE_API int _STDCALL VerifyMachine(const char* envirment, const char* registerCode);

/**
	序列号
	param machineId, 存储机器序列号 XXXX-XXXX-XXXX-XXXX-XXXX
	return keyValue, 用于宿主程序的密钥【重要】，校验失败返回0
*/
REGISTRATION_CODE_API void _STDCALL QueryMachineId(char* machineId);




#endif // !_REGISTRATION_CODE_H_
