#include "json_parser.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include "model/physical/building.h"
#include "model/physical/path.h"
#include "model/physical/physical_model.h"
#include "model/logical/course.h"
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
    R["bicycle_able"] = ptr->get_bicycle_able();
    double distance = ptr->get_distance();
    if(0 <= ptr->get_connections().first && ptr->get_connections().first <= 20 || 170 <= ptr->get_connections().first && ptr->get_connections().first <= 219)
    {
        distance -= 2000;
    }
    if(0 <= ptr->get_connections().second && ptr->get_connections().second <= 20 || 170 <= ptr->get_connections().second && ptr->get_connections().second <= 219)
    {
        distance -= 2000;
    }
    R["distance"] = distance;
    return R;
}

static json get_course_json(std::shared_ptr<Course> ptr)
{
    json R;
    R["type"] = "course";
    R["id"] = ptr->get_id();
    R["name"] = ptr->get_name();
    R["physical_id"] = ptr->get_physical_id();
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
        else if(id >= 170 && id <= 539)
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
        else if(id >= 360 && id < 400)
        {
            if (auto m = std::dynamic_pointer_cast<Course>(mh_ptr->get(id)))
            {
                R1.push_back(get_course_json(m));
            }
        }
        else
        {
            if (auto m = std::dynamic_pointer_cast<Course>(mh_ptr->get(id)))
            {
                R2.push_back(get_course_json(m));
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
        R["status"] = 1;
        R["distance"] = {};
        return R.dump();
    }
    else if (from < 170 && to < 170 || from >= 170 && to >= 170)
    {
        auto nav = mh_ptr->navigate(from, to, strategy);
        double distance = nav.second;
        if (nav.first.size() <= 1) 
        {
            R["status"] = 0;
        } 
        else 
        {
            R["status"] = 1;
            R["distance"] = distance - 4000;
            R["navigation"] = nav.first;
        }
        return R.dump();
    }else if(from < 170)
    {
        json R1;
        auto nav2 = mh_ptr->navigate(from, 21, strategy);
        auto nav3 = mh_ptr->navigate(326, to, strategy);
        json R2 = nav2.first;
        json R3 = nav3.first;
        R1.push_back(R2);
        R1.push_back(R3);
        R["status"] = 2;
        R["navigation"] = R1;
        R["distance"] = nav2.second + nav3.second + 20000 - 4000;
        return R.dump();

    }else
    {
        json R1;
        auto nav2 = mh_ptr->navigate(from, 326, strategy);
        auto nav3 = mh_ptr->navigate(21, to, strategy);
        json R2 = nav2.first;
        json R3 = nav3.first;
        R1.push_back(R2);
        R1.push_back(R3);
        R["status"] = 2;
        R["navigation"] = R1;
        R["distance"] = nav2.second + nav3.second + 20000 - 4000;
        return R.dump();
    }
    
}

std::string get_navigation(std::shared_ptr<ModelHub> mh_ptr, int from, int to, int strategy,json relay)
{
    int relay_size = relay.size();
    double distance = 0;
    if (relay_size == 0) {
        Logger::info("用户从 ["+ std::to_string(from) +"]\"" + mh_ptr->get(from)->get_name()  +"\" 导航到 [" + std::to_string(to)+"]\""+
        mh_ptr->get(to)->get_name()+"\".");
    }
    json R;
    R["status"] = 1;
    json tmp_path;
    json part_nav; 
    std::vector<int> original_points;
    for (auto i = 0; i < relay_size; i++) {
        original_points.push_back(stoi(relay[i].dump()));
    }
    if (from < to)
    {
        sort(original_points.begin(),original_points.end());
    }
    else
    {
        sort(original_points.rbegin(), original_points.rend());
    }

    std::vector<int> points;
    points.push_back(from);
    for (auto i = 0; i < original_points.size(); i++) {
        points.push_back(original_points[i]);
    }
    points.push_back(to);
    int idx = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        part_nav = json::parse(get_navigation(mh_ptr, points[i], points[i+1], strategy));
        if(part_nav["status"] == 0)
        {
            R["status"] = 0;
            return R.dump();
        }
        else if(part_nav["status"] == 1)
        {
            for (int j = 0; j < part_nav["navigation"].size()-1; j++)
            {
                tmp_path[idx].push_back(part_nav["navigation"][j]);
            }
        } 
        else if (part_nav["status"] == 2) {
            R["status"] = 2;
            for (int j = 0; j < part_nav["navigation"][0].size(); j++) {
                tmp_path[idx].push_back(part_nav["navigation"][0][j]);
            }
            idx++;
            for (int j = 0; j < part_nav["navigation"][1].size()-1; j++)
            {
                tmp_path[idx].push_back(part_nav["navigation"][1][j]);
            }
        }
        distance += stoi(part_nav["distance"].dump());
    }
    if (part_nav["status"] == 1)
    {
        tmp_path[idx].push_back(part_nav["navigation"][part_nav["navigation"].size() - 1]);
    } else {
        tmp_path[idx].push_back(part_nav["navigation"][1][part_nav["navigation"][1].size() - 1]);
    }

    if (idx == 0)
    {
        tmp_path = tmp_path[0];
    }

    R["navigation"] = tmp_path;
    R["distance"] = distance;
    Logger::info("用户从 ["+ std::to_string(from) +"]\"" + mh_ptr->get(from)->get_name()  +"\" 导航到 [" + std::to_string(to)+"]\""+
        mh_ptr->get(to)->get_name()+"\".");
    return R.dump();
}

std::string get_player_initial_info() {
    json R;
    R["position_id"] = 0;
    R["speed"] = 200;
    R["width"] = 40;
    return R.dump();
}

std::string get_search_result(std::shared_ptr<ModelHub> mh_ptr, int from, int distance)
{
    json R;
    json R1;
    int cnt = 0;
    int flag = 0;
    auto m = std::dynamic_pointer_cast<Building>(mh_ptr->get(from));
    if (m->get_shape() == "crossing" || m->get_shape() == "gate") {
        distance += 2000;
    }
    else
    {
        distance += 4000;
        flag = 1;
    }
    auto origin_res = mh_ptr->search_near_model(mh_ptr->get(from), distance);
    for (auto it = origin_res.begin(); it != origin_res.end(); it++) {
        if(std::dynamic_pointer_cast<Building>(mh_ptr->get(it->first))->get_shape() != "crossing")
        {
            if (std::dynamic_pointer_cast<Building>(mh_ptr->get(it->first))->get_shape() == "gate")
            {
                if(it->second > distance - 2000 * flag )
                {
                    continue;
                }
                else
                {
                   it->second+=2000;
                }
            }
            if (m->get_shape() != "crossing" && m->get_shape() != "gate")
            {
                it->second-=4000;
            }
            else
            {
                it->second-=2000;
            }
            json R2;
            R2["id"] = it->first;
            R2["distance"] = it->second;
            R1.push_back(R2);
            cnt++;
        }
    }
    if (cnt <= 0) {
        R["num"] = 0;
    } else {
        R["num"] = cnt;
        R["result"] = R1;
    }
    return R.dump();
}
