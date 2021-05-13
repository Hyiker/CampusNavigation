#ifndef CLI_H
#define CLI_H
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include "boot/bootloader.h"
#include "model/model_hub.h"
enum class CliState { ASK_FOR_TARGET, ASK_FOR_STRATEGY };
// command line interface class
class Cli {
   private:
    std::shared_ptr<ModelHub> mh_ptr = nullptr;
    std::string hub_path;
    // use a finite state machine
    CliState state;
    std::shared_ptr<Model> _dest;

   public:
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
    }
    void loop() {
        while (true) {
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
                    state = CliState::ASK_FOR_STRATEGY;

                    // TODO: ask for strategy
                    break;
                }

                default:
                    break;
            }
        }
    }
};

#endif