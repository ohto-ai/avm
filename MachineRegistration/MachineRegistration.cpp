#include "pch.h"
#include "MachineRegistration.h"

constexpr char Salt[] = R"THIS-IS-SALT(SMARTION-XN1eO233,gCGtFc^z/0FkjYi9jzY)'E2K@:]'q3$]5&{dK!s2p$Wf14j/oIKKABR9?!R1yJW}{U;3DM/i9Z&L3|N3%N{M#Ml6~)4enw,*;.yD];vUBh8;D;)THIS-IS-SALT";
constexpr char SaltX[] = R"THIS-IS-SALT-X(W"4\)UJl)B$oDZF}?G6#b5Hb'h$LaX!M)THIS-IS-SALT-X";

int encodeXor(const char* machineCode, std::string& registerCode)
{
	int xorValue = 0;
	registerCode = md5(md5(md5(md5(machineCode)) + Salt));
	for (size_t i = 0; i < 32; i++)
		registerCode[i] ^= SaltX[i];
	for (size_t i = 0; i < 8; i++)
		xorValue ^= *reinterpret_cast<int*>(&registerCode[i * 4]);
	registerCode = md5(registerCode).substr(4, 20);
	xorValue &= 0XFFFF;

	return xorValue;
}

bool verifyCode(const char* machineCode, const char* registerCode, int& xorValue)
{
	std::string realCode;
	std::string clientCode(registerCode);
	xorValue = encodeXor(machineCode, realCode);
	clientCode.erase(std::remove(clientCode.begin(), clientCode.end(), '-'), clientCode.end());
	return realCode == clientCode;
}

int replaceLastPart(int xorValue, std::string& registerCode) 
{
	int keyValue;
	sscanf_s(registerCode.data() + 15, "%x", &keyValue);
	sprintf_s(const_cast<char*>(registerCode.data() + 15), 6, "%05X", xorValue ^ keyValue);
	registerCode.insert(15, 1, '-');
	registerCode.insert(10, 1, '-');
	registerCode.insert(5, 1, '-');
	return keyValue;
}

int getRegisterCode(const char* machineCode, std::string& registerCode)
{
	return replaceLastPart(encodeXor(machineCode, registerCode), registerCode);
}


REGISTRATION_CODE_API int _stdcall RegisterMachine(const char* machineCode, char* code)
{
	std::string registerCode;
	int key = getRegisterCode(machineCode, registerCode);
	registerCode.copy(code, registerCode.size() + 1);
	return key;
}

REGISTRATION_CODE_API int _stdcall VerifyMachine(const char* machineCode, const char* code)
{
	char registerCode[40];
	int key, xorCode;

	if (strlen(code) < 23)
		return -1;
	sscanf_s(code + 18, "%x", &key);
	if (verifyCode(machineCode, code, xorCode))
	{
		key ^= xorCode;
	}
	else
	{
		key ^= xorCode;
		key = -key;
	}
	return key;
}
