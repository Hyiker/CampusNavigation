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
enum class CliState { ASK_FOR_TARGET, ASK_FOR_STRATEGY, NAVIGATING_ON_ROAD };
// command line interface class
class Cli {
   private:
    std::shared_ptr<ModelHub> mh_ptr = nullptr;
    std::string hub_path;
    // use a finite state machine
    CliState state;
    std::shared_ptr<Model> _dest, _position;
    const std::vector<std::string> _strategies{"步行", "避开拥挤", "自行车"};

   public:
    void show_dashboard() {
        std::cout << boost::format("当前系统时间: %u\n") % 0
                  << boost::format("当前位置: %1%\n") % (_position == nullptr ? "NULL" : _position->get_name());
    }
    void clear() {
#ifdef _WIN32
        system("clr");
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
    Cli(const std::string& hub_path) : hub_path{hub_path}, state{CliState::ASK_FOR_TARGET} {
    }
    // or, let the cli ask for it
    Cli() : Cli{ask_for("输入模型仓库路径:")} {
    }
    void init() {
        Logger::init("stderr", FileMode::OVERRIDE, LogLevel::DEBUG);
        mh_ptr = BootLoader::load_model_hub(hub_path);
        // default use Id=0 as start position
        _position = mh_ptr->get(0);
    }
    void loop() {
        while (true) {
            clear();
            show_dashboard();
            switch (state) {
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
                    }

                    state = CliState::ASK_FOR_STRATEGY;

                    // TODO: ask for strategy
                    break;
                }
                case CliState::ASK_FOR_STRATEGY: {
                    show_list(_strategies);

                    int sel = ask_for_int("输入你的目的策略:");

                    if (sel < 0 || sel >= _strategies.size()) {
                        show_warning("无效的序号");
                        break;
                    }

                    auto route = mh_ptr->navigate(_position->get_id(), _dest->get_id(), sel + 1);
                    if (route.size() == 0) {
                        show_warning("该策略下无可用路线");
                        break;
                    }
                    show_info("找到有效路线，如下: ");
                    size_t i = 0;
                    for (auto& mp : route) {
                        if (auto path = std::dynamic_pointer_cast<PhysicalPath>(mp)) {
                            show_info(boost::str(boost::format("经过路径[%1%],") % path->get_id()));
                        } else if (auto pm = std::dynamic_pointer_cast<PhysicalModel>(mp)) {
                            show_info(boost::str(boost::format("到达地点\"%1%\".") % pm->get_name()));
                        } else {
                            Logger::error("未识别的模型类型");
                        }
                    }
                    std::string ans = boost::algorithm::to_lower_copy(ask_for("是否开始导航(y/n):"));
                    if (ans.find("y") != std::string::npos) {
                        state = CliState::NAVIGATING_ON_ROAD;
                    } else {
                        state = CliState::ASK_FOR_TARGET;
                    }

                    break;
                }
                case CliState::NAVIGATING_ON_ROAD: {
                    return;
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