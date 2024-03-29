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

/// <summary>
/// 注册客户端
/// </summary>
/// <param name="machineId">机器序列号</param>
/// <param name="envirment">用于结合MachineId生成注册码，以便校对</param>
/// <param name="registerCode"></param>
/// <returns>用于宿主程序的密钥【重要】</returns>
REGISTRATION_CODE_API int _STDCALL RegisterClient(const char* machineId, const char* envirment, char* registerCode);

/// <summary>
/// 验证客户端
/// </summary>
/// <param name="machineId">机器序列号</param>
/// <param name="envirment">用于结合MachineId生成注册码，以便校对</param>
/// <param name="registerCode">客户提供的注册码</param>
/// <returns>用于宿主程序的密钥【重要】，校验失败返回0，格式错误返回-1</returns>
REGISTRATION_CODE_API int _STDCALL VerifyClient(const char* machineId, const char* envirment, const char* registerCode);
#endif // MACHINEREGISTRATION_EXPORTS

/// <summary>
/// 验证本机
/// </summary>
/// <param name="envirment">用于结合MachineId生成注册码，以便校对</param>
/// <param name="registerCode">客户提供的注册码</param>
/// <returns>用于宿主程序的密钥【重要】，校验失败返回0，格式错误返回-1</returns>
REGISTRATION_CODE_API int _STDCALL VerifyMachine(const char* envirment, const char* registerCode);

/// <summary>
/// 查询本机序列号
/// </summary>
/// <returns>序列号</returns>
REGISTRATION_CODE_API const char* _STDCALL QueryMachineId();

#ifdef JNI_API
/// JNI
/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class MachineRegistrationClient */

#ifndef _Included_MachineRegistrationClient
#define _Included_MachineRegistrationClient
#ifdef __cplusplus
extern "C" {
#endif
	JNIEXPORT jint JNICALL Java_RegistrationClient_MachineRegistrationClient_VerifyMachine
	(JNIEnv*, jclass, jstring, jstring);

	JNIEXPORT jstring JNICALL Java_RegistrationClient_MachineRegistrationClient_QueryMachineId
	(JNIEnv*, jclass);
#ifdef __cplusplus
}
#endif
#endif
#endif


#endif // !_REGISTRATION_CODE_H_


