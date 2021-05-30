#ifndef _SERVER_H_
#define _SERVER_H_

#include "cinatra.hpp"
#include "json/json_parser.h"
#include "model/model_hub.h"

int server_init(std::shared_ptr<ModelHub> model_hub);

#endif