#pragma once
#ifndef _MACHINEREGISTRATION_H_
#define _MACHINEREGISTRATION_H_

#include "MD5.h"

#ifndef REGISTRATION_CODE_API

#ifdef __GNUC__
#define REGISTRATION_CODE_API extern "C"
#else
#ifdef _MSC_VER

#ifdef MACHINEREGISTRATION_EXPORTS
#define REGISTRATION_CODE_API extern "C" __declspec(dllexport)
#else
#define REGISTRATION_CODE_API extern "C" __declspec(dllimport)
#endif

#endif // _MSVC_VER_
#endif // __GNUC__
#endif // !REGISTRATIONCODEAPI

REGISTRATION_CODE_API int _stdcall RegisterMachine(const char* envirment, char* code);
REGISTRATION_CODE_API int _stdcall VerifyMachine(const char* envirment, const char* code);

#endif // !_REGISTRATION_CODE_H_
