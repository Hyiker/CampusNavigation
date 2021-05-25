#include "json_parser.h"
using json = nlohmann::json;

std::string s = R"(
{
    "name": "Judd Trump",
    "credits": 1754500,
    "ranking": 1
}
)";
auto j = json::parse(s);