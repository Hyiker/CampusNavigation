#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <memory>
#include <string>
#include "json.hpp"
#include "model/model_hub.h"

std::string get_all_models(std::shared_ptr<ModelHub> mh_ptr);
std::string get_navigation(std::shared_ptr<ModelHub> mh_ptr,int from,int to,int strategy);
std::string get_navigation(std::shared_ptr<ModelHub> mh_ptr, int from, int to, int strategy, nlohmann::json relay);
std::string get_player_initial_info();
std::string get_search_result(std::shared_ptr<ModelHub> mh_ptr, int from, int distance);

#endif