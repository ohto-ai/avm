
#include <httplib.h>
#include <fstream>
#include "json.hpp"
#include "MachineRegistrationForLinux.h"


std::ofstream keyFile("machine.key",std::ios::app);

void Register(const httplib::Request&req, httplib::Response&res)
{
	using namespace nlohmann;
	using namespace std;
	if (!req.has_param("machine_id"))
	{
		res.set_content(R"(Illegal parameter)"_json.dump(), "application/json");
		return;
	}

	auto machineId = req.get_param_value("machine_id");
	auto environment = req.get_param_value("environment");
	char clientCode[260] = { NULL };
	auto key = RegisterClient(machineId.c_str(), environment.c_str(), clientCode);


	json param, file;
	param["register_code"] = clientCode;
	file["machine_id"] = machineId;
	file["environment"] = environment;
	file["register_code"] = clientCode;
	file["key_value"] = key;
	file["remote_addr"] = req.remote_addr;

	keyFile << file.dump(4) << endl << endl;
	keyFile.flush();

	res.set_content(param.dump(), "application/json");
}

int main()
{
	constexpr auto IP =
#ifdef _WIN32
		"localhost";
#else
		"172.24.22.226";
#endif
	httplib::Server regServer;
	regServer.set_mount_point("/", ".");

	regServer.Get("/reg", Register);
	regServer.Get("/quit", [&regServer](const httplib::Request& req, httplib::Response& res) {regServer.stop(); });
	regServer.listen(IP, 2048);
	keyFile.close();
}