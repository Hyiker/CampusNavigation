#include "boot/bootloader.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <string>
#include "logger/logger.h"
#include "model/physical/path.h"
#include "model/physical/building.h"
using namespace std;
static void parse_model(shared_ptr<ModelHub> hub, string& line) {
    vector<string> result;
    boost::split(result, line, boost::is_any_of(","));
    Id id = stoi(result[0]);
    auto model_ptr = hub->construct_with_list(result[1], result);
    if (auto path_ptr = static_pointer_cast<PhysicalPath>(model_ptr)) {
        shared_ptr<PhysicalModel> m1, m2;
        auto conn = path_ptr->get_connections();
        if (!hub->have(conn.first) && !hub->have(conn.second)) {
            // TODO: throw error here
            return;
        }

        if (m1 = static_pointer_cast<Building>(hub->get(conn.first))) {
            hub->connect(path_ptr, m1);
        } else {
            // TODO: throw error here
            return;
        }
        if (m2 = static_pointer_cast<Building>(hub->get(conn.second))) {
            hub->connect(path_ptr, m2);
        } else {
            // TODO: throw error here
            return;
        }
    }
}
shared_ptr<ModelHub> BootLoader::load_model_hub(const string& filename) {
    auto hub = make_shared<ModelHub>();
    ifstream ifs(filename);
    if(ifs.fail())
    {
        Logger::error("打开地图文件失败，请检查文件路径是否正确");
        exit(0);
    }
    string line;
    while (!ifs.eof()) {
        //Logger::debug("reading a line");
        getline(ifs, line);
        if (line.length() > 0) {
            parse_model(hub, line);
        }
    }
    Logger::info("成功从地图文件中加载了模型数据");
    Logger::info("请在浏览器中打开: http://dir.bj1.byr.cool:11209/ 查看导览效果");
    return hub;
}