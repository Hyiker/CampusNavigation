#include "server.h"
using namespace cinatra;

int server_init() {
    int max_thread_num = std::thread::hardware_concurrency();
    http_server server(max_thread_num);
    server.listen("127.0.0.1", "8080");
    server.set_http_handler<GET, POST>(
        "/", [](request& req, response& res) { res.set_status_and_content(status_type::ok, "hello world"); });
    server.run();
    return 0;
}