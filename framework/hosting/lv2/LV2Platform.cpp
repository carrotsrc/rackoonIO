#include "LV2Platform.h"

using namespace RackoonIO::Hosting;

LV2Platform::LV2Platform() {
	world = lilv_world_new();
	if(world == NULL) {
		throw new std::bad_alloc();
	}

	lilv_world_load_all(world);
}


LV2Platform::~LV2Platform() {
	lilv_world_free(world);
}


const LilvPlugins* LV2Platform::getPlugins() {
	return lilv_world_get_all_plugins(world);
}


const LilvPlugin *LV2Platform::getPlugin(std::string uri) {
	auto plugins = getPlugins();
	const LilvPlugin *plugin;
	auto uriNode = lilv_new_uri(world, uri.c_str());

	if((plugin = lilv_plugins_get_by_uri(plugins, uriNode)) == NULL) {
		return nullptr;
	}

	return plugin;
}