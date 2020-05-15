
#include <httplib.h>
#include <fstream>
#include <httplib.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <map>
#include "json.hpp"
#include "MachineRegistrationForLinux.h"

int main()
{
	using namespace httplib;
	using namespace std;
	using namespace std::chrono;
	using namespace nlohmann;

	
#ifdef _WIN32
	constexpr auto IP = "localhost";
	constexpr auto URL_MAP_PATH = "url-map.json";
	constexpr auto MACHINE_KEYMAP_PATH = "machine-key.json";
#else
	constexpr auto IP = "172.24.22.226";
	constexpr auto URL_MAP_PATH = "/data/machine-register/url-map.json";
	constexpr auto MACHINE_KEYMAP_PATH = "/data/machine-register/machine-key.json";
#endif
	/*
	@machine.key formt
	{
		"XXXX-XXXX-XXXX-XXXX":{
			"time_slamp":13131313131,
			"remote_addr":"192.168.1.137",

			"machine_id":"XXXXX-XXXXX-XXXXX-XXXXX",
			"environment":"XXXXX",
			"register_code":"XXXX-XXXX-XXXX-XXXX",
			"key_value":1234
		},
		"XXXX-XXXX-XXXX-XXXY":{
			"time_slamp":13131313131,
			"remote_addr":"192.168.1.137",

			"machine_id":"XXXXX-XXXXX-XXXXX-XXXXX",
			"environment":"XXXXX",
			"register_code":"XXXX-XXXX-XXXX-XXXX",
			"key_value":1234
		}
	}
	*/
	json machineKeyMap;
	/*
	@url.map formt
	[
		{
			"time_slamp":99D4DA31D7B0D,
			"remote_addr":"192.168.1.137",

			"target":"https://www.baidu.com",
			"url":"https://thatboy.info/jump/99D4DA31D7B0D.html"
		},
		{
			"time_slamp":99D4443DADC91,
			"remote_addr":"192.168.1.137",

			"target":"https://www.baidu.com",
			"url":"https://thatboy.info/jump/99D4443DADC91.html"
		}
	]
	*/
	json urlMap;

	httplib::Server regServer;
	regex urlRegex(R"((ht|f)tp(s?)\:\/\/[0-9a-zA-Z]([-.\w]*[0-9a-zA-Z])*(:(0-9)*)*(\/?)([a-zA-Z0-9\-\.\?\,\'\/\\\+&amp;%\$#_]*)?)");

	/* load from file */
	ifstream urlMapFileRead(URL_MAP_PATH);
	if (urlMapFileRead)
	{
		urlMapFileRead >> urlMap;
		urlMapFileRead.close();
	}

	ifstream machinKeyMapFileRead(MACHINE_KEYMAP_PATH);
	if (machinKeyMapFileRead)
	{
		machinKeyMapFileRead >> urlMap;
		machinKeyMapFileRead.close();
	}

	regServer.set_mount_point("/", ".");

	regServer.Get("/reg", [&](const Request& req, Response& res)
		{
			json body;
			if (!req.has_param("machine_id"))
			{
				body["error"] = "no machine_id given";
				res.set_content(body.dump(), "application/json");
				return;
			}

			auto machineId = req.get_param_value("machine_id");
			auto environment = req.get_param_value("environment");
			char clientCode[260] = { NULL };
			auto key = RegisterClient(machineId.c_str(), environment.c_str(), clientCode);

			stringstream timeSlampStream;
			string timeSlamp;
			timeSlampStream << setiosflags(ios::uppercase) << hex << steady_clock::now().time_since_epoch().count();
			timeSlamp = timeSlampStream.str();

			machineKeyMap[clientCode]["time_slamp"] = timeSlamp;
			machineKeyMap[clientCode]["remote_addr"] = req.remote_addr;

			machineKeyMap[clientCode]["register_code"] = clientCode;
			machineKeyMap[clientCode]["machine_id"] = machineId;
			machineKeyMap[clientCode]["environment"] = environment;
			machineKeyMap[clientCode]["key_value"] = key;

			body["register_code"] = clientCode;
			body["machine_id"] = machineId;
			body["environment"] = environment;;

			ofstream machineKeyMapFileWrite(MACHINE_KEYMAP_PATH);
			if (!machineKeyMapFileWrite)
				cerr << "Open " << MACHINE_KEYMAP_PATH << " error!" << endl;
			machineKeyMapFileWrite << machineKeyMap;
			machineKeyMapFileWrite.close();

			res.set_content(body.dump(), "application/json");
		});
	regServer.Get("/quit", [&regServer](const httplib::Request& req, httplib::Response& res) {regServer.stop(); });
	regServer.Get("/makejump", [&](const Request& req, Response& res)
		{
			json body;
			if (req.has_param("url"))
			{
				auto url = req.get_param_value("url");
				if (regex_match(url, urlRegex))
				{
					if (urlMap.find(url) == urlMap.end())
					{
						stringstream timeSlampStream;
						string timeSlamp;
						string redirectUrl;
						timeSlampStream << setiosflags(ios::uppercase) << hex << steady_clock::now().time_since_epoch().count();
						timeSlamp = timeSlampStream.str();

						urlMap[url]["time_slamp"] = timeSlamp;
						urlMap[url]["remote_addr"] = req.remote_addr;

						redirectUrl = "jump/" + timeSlamp + ".html";
						std::ofstream ifs(redirectUrl);
						ifs << R"(<html><script language="javascript" type="text/javascript">window.location.href=')" << url << R"(';</script></html>)" << endl;
						ifs.close();
						redirectUrl = "https://thatboy.info/" + redirectUrl;

						urlMap[url]["target"] = url;
						urlMap[url]["url"]= redirectUrl;

						ofstream urlMapFileWrite(URL_MAP_PATH);
						if (!urlMapFileWrite)
							cerr << "Open " << URL_MAP_PATH << " error!" << endl;
						urlMapFileWrite << urlMap;
						urlMapFileWrite.close();

					}
					body["target"] = url;
					body["url"] = urlMap[url]["url"];
				}
				else
					body["error"] = "url formt invalid";
			}
			else
				body["error"] = "no url given.";
			res.set_content(body.dump(), "application/json");
		}
	);

	cout << " Server starting..." << endl;
	regServer.set_error_handler([&](const Request& req, Response& res) {
		cout << "Error target:" << req.target << endl;
		res.set_redirect("https://thatboy.info/404.html");
		});
	if (regServer.listen(IP, 8080))
		cout << "Server start failed. Permission denied!" << endl;
	else
		cout << "Server started." << endl;
}
