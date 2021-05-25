#ifndef CLI_H
#define CLI_H
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include "boot/bootloader.h"
#include "model/logical/course.h"
#include "model/model_hub.h"
#include "toki/toki.hpp"
enum class CliState {
    ASK_FOR_START,
    ASK_FOR_NEXT,
    ASK_FOR_SEARCH,
    ASK_FOR_TARGET,
    ASK_FOR_STRATEGY,
    NAVIGATING_ON_ROAD,
    NAVIGATION_FINISH
};
// command line interface class
class Cli {
   private:
    Toki toki;
    std::shared_ptr<ModelHub> mh_ptr = nullptr;
    std::string hub_path;
    // use a finite state machine
    CliState state;
    std::shared_ptr<Model> _dest, _position;
    // distance from start endpoint on a path
    Distance _on_road_distance;
    uint32_t route_index;
    std::vector<std::shared_ptr<Model>> route;

    // user's speed, how far he/she can goes in a toki second
    Distance speed;
    const std::vector<std::string> _strategies{"步行", "避开拥挤", "自行车", "返回"};

   public:
    void show_dashboard() {
        std::cout << boost::format("当前系统时间: %1%\n") % toki.get_format_string()
                  << boost::format("当前位置: [%1%]%2%\n") % (_position == nullptr ? -1 : _position->get_id()) %
                         (_position == nullptr ? "无" : _position->get_name());
    }
    void clear() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
    // request user input by output the question first
    // accepting multitype return types
    int ask_for_int(const std::string& question) {
        std::cout << question;
        std::string buf;
        std::cin >> buf;
        return stoi(buf);
    }
    void show_list(const std::vector<std::string>& items, const std::string& prefix = "") {
        for (size_t i = 0; i < items.size(); i++) {
            std::cout << i << '.' << prefix << items[i] << '\n';
        }
    }
    void show_warning(const std::string& message) {
        std::cout << message << '\n';
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    void show_info(const std::string& message) {
        std::cout << message << '\n';
    }
    std::string ask_for(const std::string& question) {
        std::cout << question;
        std::string buf;
        std::cin >> buf;
        return buf;
    }

    // init cli from a external model hub
    Cli(const std::string& hub_path)
        : hub_path{hub_path},
          state{CliState::ASK_FOR_START},
          _position{nullptr},
          _dest{nullptr},
          _on_road_distance{0.0},
          speed{1} {
    }
    // or, let the cli ask for it
    Cli() : Cli{ask_for("输入模型仓库路径:")} {
    }
    void init() {
        Logger::init("stderr", FileMode::OVERRIDE, LogLevel::DEBUG);
        mh_ptr = BootLoader::load_model_hub(hub_path);
        // default use Id=0 as start position
        toki.run();
    }
    void loop() {
        while (true) {
            clear();
            show_dashboard();
            switch (state) {
                case CliState::ASK_FOR_START: {
                    std::string start_str = ask_for("输入你的起始地以进行查询:");
                    auto start_options = mh_ptr->search_name(start_str);
                    if (start_options.size() == 0) {
                        show_warning("没有查找到指定的起始地");
                        break;
                    }
                    std::vector<std::string> _start_opts_str;
                    std::vector<std::shared_ptr<PhysicalModel>> _start_opts;
                    for (auto& opt : start_options) {
                        if (auto _ = std::dynamic_pointer_cast<PhysicalModel>(opt.second)) {
                            _start_opts_str.push_back(_->get_name());
                            _start_opts.push_back(_);
                        }
                    }
                    show_list(_start_opts_str);
                    int sel = ask_for_int("输入当前位置的序号(-1表示没有对应的起始地点):");
                    if (sel < 0) {
                        break;
                    }
                    if (sel >= _start_opts_str.size()) {
                        show_warning("无效的序号");
                        break;
                    }
                    _position = _start_opts[sel];

                    state = CliState::ASK_FOR_NEXT;
                    break;
                }
                case CliState::ASK_FOR_NEXT: {
                    int todo = ask_for_int("请选择你要做什么:\n1.导航  2.查找附近的建筑\n");
                    switch (todo) {
                        case 1:
                            state = CliState::ASK_FOR_TARGET;
                            break;
                        case 2:
                            state = CliState::ASK_FOR_SEARCH;
                            break;
                        default:
                            show_warning("无效的输入序号");
                            break;
                    }

                    break;
                }

                case CliState::ASK_FOR_SEARCH:{
                    int distance = ask_for_int("请输入查找的距离范围:");
                    auto _model_list = mh_ptr->search_near_model(_position, distance);
                    if(_model_list.size() == 0){
                        show_warning("该距离内没有其他建筑");
                        break;
                    }
                    std::vector<std::string> _near_model_str;
                    for(auto& _near_model: _model_list){
                       _near_model_str.push_back(_near_model->get_name()); 
                    }
                    show_list(_near_model_str);
                    int sel = ask_for_int("输入你想要到达的目的地序号(-1表示没有希望到达的目的地):");
                    if(sel<0){
                        break;
                    }
                    _dest = _model_list[sel];
                    state = CliState::ASK_FOR_STRATEGY;
                    break;
                }
                case CliState::ASK_FOR_TARGET: {
                    std::string dest = ask_for("输入你的目的地以进行查询:");
                    auto dest_options = mh_ptr->search_name(dest);
                    if (dest_options.size() == 0) {
                        show_warning("没有查找到指定的目的地");
                        break;
                    }
                    std::vector<std::string> _dest_opts_str;
                    for (auto& opt : dest_options) {
                        _dest_opts_str.push_back(opt.second->get_name());
                    }
                    show_list(_dest_opts_str);
                    int sel = ask_for_int("输入你想要到达的目的地序号(-1表示没有希望到达的目的地):");
                    if (sel < 0) {
                        break;
                    }
                    if (sel >= dest_options.size()) {
                        show_warning("无效的序号");
                        break;
                    }
                    _dest = dest_options[sel].second;
                    if (auto lm = std::dynamic_pointer_cast<LogicalModel>(_dest)) {
                        show_info(boost::str(boost::format("逻辑位置%1%对应的物理位置为%2%") % _dest->get_name() %
                                             (_dest = mh_ptr->get(lm->get_physical_id()))->get_name()));
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }

                    state = CliState::ASK_FOR_STRATEGY;

                    break;
                }
                case CliState::ASK_FOR_STRATEGY: {
                    show_list(_strategies);

                    int sel = ask_for_int("输入你的目的策略:");

                    if (sel < 0 || sel >= _strategies.size()) {
                        show_warning("无效的序号");
                        break;
                    }
                    if (sel == _strategies.size() - 1) {
                        state = CliState::ASK_FOR_TARGET;
                        break;
                    }

                    auto route = mh_ptr->navigate(_position->get_id(), _dest->get_id(), sel + 1);
                    if (route.size() <= 1) {
                        show_warning("该策略下无可用路线");
                        break;
                    }
                    show_info("找到有效路线，如下: ");
                    size_t i = 0;
                    for (auto& mp : route) {
                        if (auto path = std::dynamic_pointer_cast<PhysicalPath>(mp)) {
                            if (sel == 2 && path->get_bicycle_able() == 1) {
                                show_info(boost::str(boost::format("骑自行车经过路径[%1%],路径长度为[%2%]") %
                                                     path->get_id() % path->get_distance()));
                            } else {
                                show_info(boost::str(boost::format("步行经过路径[%1%],路径长度为[%2%]") %
                                                     path->get_id() % path->get_distance()));
                            }

                        } else if (auto pm = std::dynamic_pointer_cast<PhysicalModel>(mp)) {
                            show_info(boost::str(boost::format("到达地点:%1%") % pm->get_name()));
                        } else {
                            Logger::error("未识别的模型类型");
                        }
                    }
                    std::string ans = boost::algorithm::to_lower_copy(ask_for("是否开始导航(y/n):"));
                    if (ans.find("y") != std::string::npos) {
                        state = CliState::NAVIGATING_ON_ROAD;
                        this->route = route;
                        this->_on_road_distance = 0.0;
                        this->route_index = 0;
                        this->toki.clear_delta_time();
                    } else {
                        state = CliState::ASK_FOR_TARGET;
                    }

                    break;
                }
                case CliState::NAVIGATING_ON_ROAD: {
                    if (route_index >= route.size() - 1) {
                        show_info(boost::str(boost::format("到达目的地: %1%") % _dest->get_name()));
                        state = CliState::NAVIGATION_FINISH;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        break;
                    }

                    uint64_t dt = toki.delta_time();
                    _on_road_distance += (Distance)(dt)*speed;
                    while (route_index < route.size()) {
                        _position = route[route_index];
                        if (auto path = std::dynamic_pointer_cast<PhysicalPath>(_position)) {
                            if (_on_road_distance >= path->get_distance()) {
                                _on_road_distance -= path->get_distance();
                                route_index++;
                                show_info(boost::str(boost::format("离开路径[%1%],") % path->get_id()));
                            } else {
                                show_info(
                                    boost::str(boost::format("正在经过路径[%1%],路径总长为[%2%],剩余长度为[%3%]") %
                                               path->get_id() % path->get_distance() %
                                               (path->get_distance() - _on_road_distance)));
                                break;
                            }

                        } else if (auto pm = std::dynamic_pointer_cast<PhysicalModel>(_position)) {
                            show_info(boost::str(boost::format("经过地点: %1%") % pm->get_name()));
                            route_index++;
                        } else {
                            Logger::error("未识别的模型类型");
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(1));

                    break;
                }
                case CliState::NAVIGATION_FINISH: {
                    show_info("导航结束");
                    state = CliState::ASK_FOR_TARGET;
                    break;
                }
                default:
                    return;
                    break;
            }
        }
    }
};

#endif