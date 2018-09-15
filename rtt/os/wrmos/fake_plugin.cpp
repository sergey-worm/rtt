#include "../../plugin/PluginLoader.hpp"
using namespace RTT;
using namespace plugin;
using namespace std;
boost::shared_ptr<PluginLoader> PluginLoader::Instance() { return 0; }
bool PluginLoader::loadService(string const& servicename, TaskContext* tc) { return false; }
