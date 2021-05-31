#include "json_parser.h"
#include <iostream>
#include <vector>
#include "model/physical/building.h"
#include "model/physical/path.h"
#include "model/physical/physical_model.h"
using json = nlohmann::json;

static json get_building_json(std::shared_ptr<Building> ptr) {
    json R;
    R["type"] = "building";
    R["id"] = ptr->get_id();
    R["name"] = ptr->get_name();
    R["x"] = ptr->get_coordinate()[0];
    R["y"] = ptr->get_coordinate()[1];
    R["width"] = ptr->get_width();
    R["height"] = ptr->get_height();
    R["paths"] = ptr->get_connections();
    R["image"] = ptr->get_shape();
    return R;
}

static json get_path_json(std::shared_ptr<PhysicalPath> ptr) {
    json R;
    R["type"] = "path";
    R["id"] = ptr->get_id();
    R["name"] = ptr->get_name();
    R["anchors"] = ptr->get_anchors();
    R["endpoints"] = ptr->get_connections();
    R["crowdness"] = ptr->get_congestion_rate();
    return R;
}

std::string get_all_models(std::shared_ptr<ModelHub> mh_ptr) {
    json R;
    json R1;
    json R2;
    Id id = 0;
    while (mh_ptr->have(id) == true) {
        if(id < 170)
        {
            if (auto m = std::dynamic_pointer_cast<Building>(mh_ptr->get(id))) 
            {
            R1.push_back(get_building_json(m));
            } 
            else if (auto m = std::dynamic_pointer_cast<PhysicalPath>(mh_ptr->get(id))) 
            {
            R1.push_back(get_path_json(m));
            }
        }
        else
        {
           if (auto m = std::dynamic_pointer_cast<Building>(mh_ptr->get(id))) 
            {
            R2.push_back(get_building_json(m));
            } 
            else if (auto m = std::dynamic_pointer_cast<PhysicalPath>(mh_ptr->get(id))) 
            {
            R2.push_back(get_path_json(m));
            } 
        }
        id++;
    }
    R.push_back(R1);
    R.push_back(R2);
    return R.dump();
}

std::string get_navigation(std::shared_ptr<ModelHub> mh_ptr, int from, int to, int strategy) {
    json R;
    if(from == to)
    {
        R["status"] = -1;
        return R.dump();
    }
    else if (from < 170 && to < 170 || from >= 170 && to >= 170)
    {
        auto nav = mh_ptr->navigate(from, to, strategy);
        if (nav.size() <= 1) 
        {
            R["status"] = 0;
        } 
        else 
        {
            R["status"] = 1;
            R["navigation"] = nav;
        }
        return R.dump();
    }else if(from < 170)
    {
        json R1;
        json R2 = mh_ptr->navigate(from, 21, strategy);
        json R3 = mh_ptr->navigate(326, to, strategy);
        R1.push_back(R2);
        R1.push_back(R3);
        R["status"] = 2;
        R["navigation"] = R1;
        return R.dump();

    }else
    {
        json R1;
        json R2 = mh_ptr->navigate(from, 326, strategy);
        json R3 = mh_ptr->navigate(21, to, strategy);
        R1.push_back(R2);
        R1.push_back(R3);
        R["status"] = 2;
        R["navigation"] = R1;
        return R.dump();
    }
    
}

std::string get_player_initial_info() {
    json R;
    R["position_id"] = 0;
    R["speed"] = 200;
    R["width"] = 40;
    return R.dump();
}
