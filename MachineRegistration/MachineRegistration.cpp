#include "pch.h"
#include "MachineRegistration.h"
#include "RegistrationCore.hpp"

using namespace thatboy::registration;

int _STDCALL RegisterClient(const char* machineId, const char* envirment, char* clientCode)
{
	std::string registerCode{ admin::generateRegCode(machineId, envirment) };
	registerCode.copy(clientCode, registerCode.size() + 1);
	return admin::getKey(envirment);
}

int _STDCALL VerifyClient(const char* machineId, const char* envirment, const char* registerCode)
{
	if (strlen(registerCode) < 23)
		return -1;
	if (client::verifyRegCode(machineId, envirment, registerCode))
		return static_cast<int>(client::getKeyOfRegCode(machineId, envirment, registerCode));
	else
		return 0;
}

int _STDCALL VerifyMachine(const char* envirment, const char* registerCode)
{
#ifndef VMWARE_NOCHECK	// ²»¼ì²éÐéÄâ»ú
	if (thatboy::isInsideVirtualMachine())
		return -2;
	else
#endif
		return VerifyClient(QueryMachineId(), envirment, registerCode);
}

const char* _STDCALL QueryMachineId()
{
	static char machineId[MAX_PATH]{ NULL };
	std::string serialId{ client::queryMachineId() };
	serialId.copy(machineId, serialId.size() + 1);
	return machineId;
}

#ifdef JNI_API
JNIEXPORT jint JNICALL Java_RegistrationClient_MachineRegistrationClient_VerifyMachine(JNIEnv* jniEnv, jclass, jstring env, jstring regcode)
{
	auto nativeEnv = jniEnv->GetStringUTFChars(env, nullptr);
	auto nativeRegCode = jniEnv->GetStringUTFChars(regcode, nullptr);
	jint ret = VerifyMachine(nativeEnv, nativeRegCode);
	jniEnv->ReleaseStringUTFChars(env, nativeEnv);
	jniEnv->ReleaseStringUTFChars(regcode, nativeEnv);
	return ret;
}

JNIEXPORT jstring JNICALL Java_RegistrationClient_MachineRegistrationClient_QueryMachineId(JNIEnv* jniEnv, jclass)
{
	return jniEnv->NewStringUTF(QueryMachineId());
}
#endif