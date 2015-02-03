#include "MidiRouter.h"

using namespace RackoonIO;

void MidiRouter::addModule(string port, string name) {
	modules.push_back(new MidiModule(port, name));
}

void MidiRouter::init() {
	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it) {
		if(!(*it)->init())
			it = modules.erase(it);

		if(it == modules.end())
			break;
	}
}

void MidiRouter::cycle() {
	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
		(*it)->cycle();
}
MidiModule* MidiRouter::operator[] (std::string name) {

	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
		if((*it)->getAlias() == name)
			return *it;

	return nullptr;
}

void MidiRouter::addBinding(std::string module, double code, std::function<void(int)> func) {
	for(std::vector<MidiModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
		if((*it)->getAlias() == module)
			(*it)->addBinding(code, func);
}
