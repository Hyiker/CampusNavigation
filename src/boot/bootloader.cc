#include "boot/bootloader.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <string>
using namespace std;
static void parse_model(shared_ptr<ModelHub> hub, string& line) {
    vector<string> result;
    boost::split(result, line, boost::is_any_of(","));
    Id id = stoi(result[0]);
    auto model_ptr = hub->construct_with_list(result[1], result);
}
shared_ptr<ModelHub> BootLoader::load_model_hub(const string& filename) {
    auto hub = make_shared<ModelHub>();
    ifstream ifs(filename);
    string line;
    while (!ifs.eof()) {
        getline(ifs, line);
        parse_model(hub, line);
    }
    return hub;
}