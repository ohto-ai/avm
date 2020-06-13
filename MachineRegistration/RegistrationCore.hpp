#pragma once
#ifndef _REGISTRATIONCORE_HPP_
#define _REGISTRATIONCORE_HPP_

#include <vector>
#include "MD5.h"
#include "../TBCpuInfo.h"

namespace thatboy
{
	namespace __hide_space__
	{
		constexpr auto MachineIdSalt = R"THIS-IS-MAC-SALT('GF:lG4V=xu9*#.7yF7f6tGICKUP/b\z\~i/$<0avtGGN2h/!$X1OCCgX"ZIU752)THIS-IS-MAC-SALT";
		constexpr auto EnironmentSalt_1 = R"THIS-IS-ENV-SALT(4Pzg\=sXc?;$fE7\9i3T<E0&eZec%\>%;0P%lh'kmwI&8fxGwxkC+&rYAA*-&M7r)THIS-IS-ENV-SALT";
		constexpr auto EnironmentSalt_2 = R"THIS-IS-ENV-SALT(ROpJ|kFp`4_8Stb.'u#3OZgt[@_TK:wF'wB_X=sT\ZI!F]k.0]$Put9/2iU,dmq-)THIS-IS-ENV-SALT";
		constexpr auto EnironmentSalt_3 = R"THIS-IS-ENV-SALT(.y1:]YH;LmFxu|D8aaT1^AH",M5w%PUz~}q6.FAV"p7hk_}VeTPm?0du/{G(tx^P)THIS-IS-ENV-SALT";
		constexpr auto EnironmentSalt_4 = R"THIS-IS-ENV-SALT(xe+TX.#B/(\-GWE>0LE3n:a(3\?*+k07SjII+?$OG~nJgXX!'is>dMKK3Lhep+OO)THIS-IS-ENV-SALT";
		constexpr auto EnironmentSalt_5 = R"THIS-IS-ENV-SALT(8TL2fvNo>IyoY>*4XOj;hZ@C/^fqFj2l3heicT<;Qfaq.*Jw72w=1hLi:-/*/b^4)THIS-IS-ENV-SALT";
		constexpr auto XOR_MASK_1 = 0XA99A5665u;
		constexpr auto XOR_MASK_2 = 0XF82635CBu;
		constexpr auto XOR_MASK_3 = 0X65AD45CBu;
		constexpr auto XOR_MASK_4 = 0X84218421u;

		/// <summary>
		/// 生成密钥，只针对环境变量，不依赖机器
		/// </summary>
		/// <param name="envirment">环境变量</param>
		/// <returns></returns>
		DWORD getKey(std::string envirment)
		{
			DWORD code = 0;
			auto ciphertext{ md5(EnironmentSalt_1 + md5(md5(envirment) + EnironmentSalt_3) + EnironmentSalt_2) };
			for (size_t i = 0; i < 8; i++)
			{
				int num;
				sscanf(ciphertext.substr(i * 4, 4).c_str(), "%x", &num);
				code ^= num + i;
			}
			code ^= XOR_MASK_1;
			code &= 0XFFFFF;
			return code;
		}

		/// <summary>
		/// 根据环境参数生成env
		/// </summary>
		/// <param name="envirment">环境参数</param>
		/// <returns>env</returns>
		DWORD getEnv(std::string envirment)
		{
			DWORD code = 0;
			auto ciphertext{ md5(EnironmentSalt_4 + md5(md5(envirment) + EnironmentSalt_5)) };
			for (size_t i = 0; i < 8; i++)
			{
				int num;
				sscanf(ciphertext.substr(i * 4, 4).c_str(), "%x", &num);
				code ^= num;
				code += i;
			}
			return code;
		}


		/// <summary>
		/// 查询设备序列号
		/// </summary>
		/// <returns>MachineId</returns>
		std::string queryMachineId()
		{
			std::string serialId{ md5(thatboy::getCpuSerialId() + thatboy::getCpuBrand()).substr(4, 20) };
			serialId.insert(16, 1, '-');
			serialId.insert(12, 1, '-');
			serialId.insert(8, 1, '-');
			serialId.insert(4, 1, '-');
			return serialId;
		}


		/// <summary>
		/// 生成Code
		/// </summary>
		/// <param name="machineId">设备序列号</param>
		/// <param name="environment">环境参数</param>
		/// <returns>Code</returns>
		std::vector<DWORD> generateBaseCode(std::string machineId, std::string environment)
		{
			auto baseCode{ md5(md5(md5(md5(environment) + MachineIdSalt) + machineId)).substr(4, 20) };
			std::vector<DWORD> code(4, 0);

			baseCode.insert(16, 1, '-');
			baseCode.insert(12, 1, '-');
			baseCode.insert(8, 1, '-');
			baseCode.insert(4, 1, '-');
			sscanf(baseCode.c_str(), "%x-%x-%x-%x", &code[0], &code[1], &code[2], &code[3]);
			return code;
		}

		/// <summary>
		/// 添加env
		/// </summary>
		/// <param name="baseCode">由generateBaseCode生成的Code</param>
		/// <param name="environment">环境参数</param>
		void attachEnvCodePart(std::vector<DWORD>& baseCode, std::string environment)
		{
			auto env{ getEnv(environment) };
			baseCode[0] ^= (env >> 0) & 0XFFFF;
			baseCode[1] ^= (env >> 8) & 0XFFFF;
			baseCode[2] ^= (env >> 16) & 0XFFFF;
			baseCode[0] ^= XOR_MASK_1 & 0XFFFFF;
			baseCode[1] ^= XOR_MASK_2 & 0XFFFFF;
			baseCode[2] ^= XOR_MASK_3 & 0XFFFFF;
		}

		/// <summary>
		/// 添加key
		/// </summary>
		/// <param name="baseCode">由generateBaseCode生成的Code</param>
		/// <param name="environment">环境参数</param>
		void attachKeyCodePart(std::vector<DWORD>& baseCode, std::string environment)
		{
			auto key{ getKey(environment) };
			baseCode[3] ^= key;
			baseCode[3] ^= XOR_MASK_3 & 0XFFFFF;
		}

		/// <summary>
		/// 混淆Code
		/// </summary>
		/// <param name="baseCode">经过两次attach的Code，混淆后即为注册码</param>
		void mergeCodePart(std::vector<DWORD>& baseCode)
		{
			baseCode[3] ^= baseCode[0];
			baseCode[3] ^= baseCode[1];
			baseCode[3] ^= baseCode[2];
			baseCode[0] ^= XOR_MASK_2 & 0XFFFFF;
			baseCode[1] ^= XOR_MASK_3 & 0XFFFFF;
			baseCode[2] ^= XOR_MASK_4 & 0XFFFFF;
			baseCode[0] ^= baseCode[3];
			baseCode[1] ^= baseCode[3];
			baseCode[2] ^= baseCode[3];
		}

		/// <summary>
		/// 反混淆Code
		/// </summary>
		/// <param name="baseCode">从注册码读来的Code，反混淆后，会还原到attach后状态</param>
		void splitCodePart(std::vector<DWORD>& baseCode)
		{
			baseCode[0] ^= baseCode[3];
			baseCode[1] ^= baseCode[3];
			baseCode[2] ^= baseCode[3];
			baseCode[0] ^= XOR_MASK_2 & 0XFFFFF;
			baseCode[1] ^= XOR_MASK_3 & 0XFFFFF;
			baseCode[2] ^= XOR_MASK_4 & 0XFFFFF;
			baseCode[3] ^= baseCode[0];
			baseCode[3] ^= baseCode[1];
			baseCode[3] ^= baseCode[2];
		}

		/// <summary>
		/// 根据序列号、环境参数、注册码 得到key
		/// </summary>
		/// <param name="machineId">设备序列号</param>
		/// <param name="environment">环境参数</param>
		/// <param name="regCode">注册码</param>
		/// <returns></returns>
		DWORD getKeyOfRegCode(std::string machineId, std::string environment, std::string regCode)
		{
			auto baseCodeOri{ generateBaseCode(machineId,environment) };
			std::vector<DWORD> baseCode(4, 0);
			sscanf(regCode.c_str(), "%x-%x-%x-%x", &baseCode[0], &baseCode[1], &baseCode[2], &baseCode[3]);
			splitCodePart(baseCode);
			return baseCode[3] ^ (XOR_MASK_3 & 0XFFFFF) ^ baseCodeOri[3];
		}


		/// <summary>
		/// 获取注册码
		/// </summary>
		/// <param name="machineId">设备序列号</param>
		/// <param name="environment">参数</param>
		/// <returns>注册码</returns>
		std::string generateRegCode(std::string machineId, std::string environment)
		{
			char buff[128] = { 0 };
			auto baseCode{ generateBaseCode(machineId,environment) };
			attachEnvCodePart(baseCode, environment);
			attachKeyCodePart(baseCode, environment);
			mergeCodePart(baseCode);
			sprintf(buff, "%05X-%05X-%05X-%05X", baseCode[0], baseCode[1], baseCode[2], baseCode[3]);
			return buff;
		}

		/// <summary>
		/// 验证注册码正确
		/// </summary>
		/// <param name="machineId">设备序列号</param>
		/// <param name="environment">参数</param>
		/// <param name="regCode">注册码</param>
		/// <returns></returns>
		bool verifyRegCode(std::string machineId, std::string environment, std::string regCode)
		{
			auto baseCodeOri{ generateBaseCode(machineId,environment) };
			std::vector<DWORD> baseCode(4, 0);
			sscanf(regCode.c_str(), "%x-%x-%x-%x", &baseCode[0], &baseCode[1], &baseCode[2], &baseCode[3]);
			splitCodePart(baseCode);
			attachEnvCodePart(baseCodeOri, environment);
			baseCode[3] = 0;
			baseCodeOri[3] = 0;
			return baseCode == baseCodeOri;
		}
	}
	/// <summary>
	/// 注册机函数
	/// </summary>
	namespace registration
	{
		namespace client
		{
			using __hide_space__::verifyRegCode;
			using __hide_space__::queryMachineId;
			using __hide_space__::getKeyOfRegCode;
		}
		namespace admin
		{
			using __hide_space__::getKey;
			using __hide_space__::generateRegCode;
			using namespace client;
		}
	}
}

#endif