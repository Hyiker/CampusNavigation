#include "server.h"
#include <string>
#include <iostream>
#include "json/json_parser.h"
using namespace cinatra;

int server_init(std::shared_ptr<ModelHub> model_hub) {
    int max_thread_num = std::thread::hardware_concurrency();
    http_server server(max_thread_num);
    server.listen("0.0.0.0", "8080");
    server.set_http_handler<GET,POST>("/v1/models", [=](request& req, response& res) {
        res.set_status_and_content(status_type::ok, std::move(get_all_models(model_hub)));
        Logger::debug("<GET> 200: 客户端成功获得地图信息");
    });

    server.set_http_handler<GET,POST>("/v1/player", [=](request& req, response& res) {
        res.set_status_and_content(status_type::ok, std::move(get_player_initial_info()));
        Logger::debug("<GET> 200: 客户端成功获得用户初始信息");
    });

    server.set_http_handler<GET,POST>("/v1/search", [=](request& req, response& res) {
        int distance = 1000;
        if(req.get_query_value("from").empty())
        {
            Logger::error("<GET> 400: 参数 ${from} 缺失，请检查请求");
            res.set_status_and_content(status_type::bad_request);
        }
        else
        {           
            int from = std::stoi(req.get_query_value("from").data());
            if(!req.get_query_value("distance").empty())
            {
                distance = std::stoi(req.get_query_value("distance").data());
            }
            res.set_status_and_content(status_type::ok, std::move(get_search_result(model_hub,from,distance)));

        }
    });

    server.set_http_handler<GET,POST>("/v1/navigate", [=](request& req, response& res) {
            int strategy = 0;
            if(req.get_query_value("from").empty() || req.get_query_value("to").empty())
            {
                Logger::error("<GET> 400: 参数 ${from} 或 ${to} 缺失，请检查请求");
                res.set_status_and_content(status_type::bad_request);
            }
            else
            {   
                int from = std::stoi(req.get_query_value("from").data());
                int to = std::stoi(req.get_query_value("to").data());
                if(!req.get_query_value("strategy").empty())
                {
                    strategy = std::stoi(req.get_query_value("strategy").data());
                }
                if(req.get_query_value("relay").empty())
                {
                    res.set_status_and_content(status_type::ok, std::move(get_navigation(model_hub,from,to,strategy,nlohmann::json{})));      
                }
                else
                {
                    auto relay = nlohmann::json::parse(req.get_query_value("relay").data());
                    res.set_status_and_content(status_type::ok,std::move(get_navigation(model_hub, from, to, strategy, relay)));
                }

            }

    });

    server.run();
    return 0;
}