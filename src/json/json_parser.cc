#include "json_parser.h"
#include "model/physical/building.h"
#include "model/physical/path.h"
#include "model/physical/physical_model.h"
#include <iostream>
#include <vector>
using json = nlohmann::json;

static json get_building_json(std::shared_ptr<Building> ptr)
{
    json R;
    R["type"] = "building";
    R["id"] = ptr->get_id();
    R["name"] = ptr->get_name();
    R["x"] = ptr->get_coordinate()[0];
    R["y"] = ptr->get_coordinate()[1];
    R["width"] = ptr->get_width();
    R["height"] = ptr->get_height();
    R["paths"] = ptr->get_connections();
    R["image"] = "square";
    return R;
}

static json get_path_json(std::shared_ptr<PhysicalPath> ptr)
{
    json R;
    R["type"] = "path";
    R["id"] = ptr->get_id();
    R["name"] = ptr->get_name();
    R["anchors"] =  ptr->get_anchors();
    R["endpoints"] =  ptr->get_connections();
    R["crowdness"] = ptr->get_congestion_rate();
    return R;
}

std::string get_all_models(std::shared_ptr<ModelHub> mh_ptr) {
    json R;
    Id id = 0;
    while(mh_ptr->have(id) == true)
    {
        if (auto m = std::dynamic_pointer_cast<Building>(mh_ptr->get(id))) {
            R.push_back(get_building_json(m));
        }
        else if (auto m = std::dynamic_pointer_cast<PhysicalPath>(mh_ptr->get(id)))
        {
            R.push_back(get_path_json(m));
        }     
        id++;
    }
    return R.dump();
}

std::string get_navigation(std::shared_ptr<ModelHub> mh_ptr,int from,int to,int strategy)
{
    json R;
    auto nav = mh_ptr->navigate(from, to, strategy);
    if (nav.size() <= 1) {
        R["status"] = 0;
    } else {
        R["status"] = 1;
        R["navigation"] = nav;
    }
    return R.dump();
}

std::string get_player_initial_info()
{
    json R;
    R["position_id"] = 0;
    R["speed"] = 2;
    return R.dump();
}
