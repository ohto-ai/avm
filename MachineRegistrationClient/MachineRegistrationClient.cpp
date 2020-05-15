#include "pch.h"
#include "MachineRegistrationClient.h"
#include "MD5.h"
#include "../TBCpuInfo.h"

constexpr char Salt[] = R"THIS-IS-SALT(SMARTION-XN1eO233,gCGtFc^z/0FkjYi9jzY)'E2K@:]'q3$]5&{dK!s2p$Wf14j/oIKKABR9?!R1yJW}{U;3DM/i9Z&L3|N3%N{M#Ml6~)4enw,*;.yD];vUBh8;D;)THIS-IS-SALT";
constexpr char SaltX[] = R"THIS-IS-SALT-X(W"4\)UJl)B$oDZF}?G6#b5Hb'h$LaX!M)THIS-IS-SALT-X";


int generateOriginalRegisterCode(const char* machineId, const char* envirment, std::string& registerCode)
{
	int xorValue = 0;
	registerCode = md5(md5(md5(md5(envirment) + Salt) + machineId));
	for (size_t i = 0; i < 32; i++)
		registerCode[i] ^= SaltX[i];
	for (size_t i = 0; i < 8; i++)
		xorValue ^= *reinterpret_cast<int*>(&registerCode[i * 4]);
	registerCode = md5(registerCode).substr(4, 20);
	xorValue &= 0XFFFF;
	return xorValue;
}

bool verifyClientCode(const char* machineId, const char* envirment, const char* registerCode, int& xorValue)
{
	std::string realCode;
	std::string clientCode(registerCode);
	xorValue = generateOriginalRegisterCode(machineId, envirment, realCode);
	clientCode.erase(std::remove(clientCode.begin(), clientCode.end(), '-'), clientCode.end());
	return clientCode.substr(0, 15) == realCode.substr(0, 15);
}

int generateFinalRegisterCode(int xorValue, std::string& registerCode)
{
	int keyValue;
	sscanf(registerCode.data() + 15, "%x", &keyValue);
	sprintf(const_cast<char*>(registerCode.data() + 15), "%05X", xorValue ^ keyValue);
	registerCode.insert(15, 1, '-');
	registerCode.insert(10, 1, '-');
	registerCode.insert(5, 1, '-');
	return keyValue;
}

int _STDCALL RegisterClient(const char* machineId, const char* envirment, char* clientCode)
{
	std::string registerCode;
	int key = generateFinalRegisterCode(generateOriginalRegisterCode(machineId, envirment, registerCode), registerCode);
	registerCode.copy(clientCode, registerCode.size() + 1);
	return key;
}

int _STDCALL VerifyClient(const char* machineId, const char* envirment, const char* registerCode)
{
	int key, xorCode;
	if (strlen(registerCode) < 23)
		return -1;
	sscanf(registerCode + 18, "%x", &key);
	if (verifyClientCode(machineId, envirment, registerCode, xorCode))
		key ^= xorCode;
	else
		key = 0;
	return key;
}

int _STDCALL VerifyMachine(const char* envirment, const char* registerCode)
{
	char machineId[MAX_PATH]{ NULL };
	QueryMachineId(machineId);
	if (thatboy::isInsideVirtualMachine())
		return 0;
	else
		return VerifyClient(machineId, envirment, registerCode);
}

void _STDCALL QueryMachineId(char* machineId)
{
	std::string serialId = thatboy::getCpuSerialId();
	serialId += thatboy::getCpuBrand();
	serialId = md5(serialId).substr(4, 20);
	serialId.insert(16, 1, '-');
	serialId.insert(12, 1, '-');
	serialId.insert(8, 1, '-');
	serialId.insert(4, 1, '-');
	serialId.copy(machineId, serialId.size() + 1);
}
