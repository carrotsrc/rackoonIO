#include <fstream>
#include "Rack.h"

using namespace RackoonIO;
Rack::Rack() {
	rackState = RACK_OFF;
}

void Rack::init() {
	initialConfig();
	std::string json = loadConfig();
	picojson::value v;
	std::string err = picojson::parse(v, json);
	if(err.empty())
		parseConfig(v, ROOT);
	else
		cout << err << endl;
	initRackQueue();
	uSleep = std::chrono::microseconds(rackConfig.system.threads.cycle);
}

void Rack::initialConfig() {
	rackConfig.system.threads = { 120, 6, 120 };
}

std::string Rack::loadConfig() {
	std::ifstream fcfg;
	std::string config = "", line = "";
	fcfg.open("system/rackoonio.cfg");
	while(fcfg) {
		std::getline(fcfg, line);
		config += line;
	}
	fcfg.close();
	return config;
}

void Rack::parseConfig(picojson::value v, RConfigArea area) {
	picojson::value cv;
	if(v.is<picojson::object>()) {
		const picojson::object& o = v.get<picojson::object>();
			for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i) {
				if(i->first == "system" && area == ROOT)
					parseConfig(i->second, SYSTEM);
				else
				if(i->first == "rack" && area == ROOT)
					parseRack(i->second);
				else
				if(i->first == "threads" && area == SYSTEM) {
					cv = i->second.get("workers");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.workers = (int)(cv.get<double>());

					cv = i->second.get("worker_us");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.worker_us = (int)(cv.get<double>());

					cv = i->second.get("cycle");
					if(!cv.is<picojson::null>())
						rackConfig.system.threads.cycle = (int)(cv.get<double>());
					return;
				}
		}
	}
}

void Rack::parseRack(picojson::value v) {
	const picojson::object& o = v.get<picojson::object>();
	vector<ConfigConnection> connections;

	picojson::value cv;
	int nplugs = o.size();

	Plug *plug = NULL;
	RackUnit *unit = NULL;

	for(int i = 1; i <= nplugs; i++) {
		plug = new Plug(NULL);
		plug->name = "ac"+std::to_string(i);
		plugArray.push_back(plug);
	}

	for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i) {
		plug = getPlug(i->first);

		cv = i->second.get("connections");
		const picojson::array& carray = cv.get<picojson::array>();
		connections = parseConnections(carray);
		for(int i = 0; i < connections.size(); i++) {
			unit = rackChain.getUnit(connections[i].name);
			if(!unit) {

				// I'M HERE
			}
		}

	}

}

std::vector<ConfigConnection> Rack::parseConnections(picojson::array a) {
	vector<ConfigConnection> connections;
	std::string s;
	for (picojson::array::const_iterator i = a.begin(); i != a.end(); ++i) { 
		const picojson::object& o = (*i).get<picojson::object>();
		ConfigConnection c;

		for (picojson::object::const_iterator i = o.begin(); i != o.end(); ++i) {
			if(i->first == "jack")
				c.jack = i->second.get<std::string>();
			else
			if(i->first == "unit")
				c.unit = i->second.get<std::string>();
			else
			if(i->first == "name")
				c.name = i->second.get<std::string>();
			else
			if(i->first == "plug")
				c.plug = i->second.get<std::string>();
		}
		connections.push_back(c);
	}
	return connections;
}

void Rack::parseChain(picojson::value v) {

}

void Rack::initRackQueue() {
	rackQueue = new RackQueue(rackConfig.system.threads.workers);
	rackQueue->init();
	rackQueue->setSleep(
		std::chrono::microseconds(rackConfig.system.threads.worker_us)
		);
}

void Rack::start() {
	rackState = RACK_AC;
	//cycleThread = new std::thread(&Rack::cycle, this);
}

void Rack::cycle() {
	cout << "Rack is cycling" << endl;

	while(rackState == RACK_AC) {

		std::this_thread::sleep_for(uSleep);
	}
}

Plug *Rack::getPlug(string name) const {
	int sz = plugArray.size();
	for(int i = 0; i < sz; i++)
		if(plugArray[i]->name == name)
			return (plugArray[i]);

	return NULL;
}
