#ifndef BOOTLOADER_H
#define BOOTLOADER_H
#include "model/model_hub.h"
#include "model/physical/path.h"

namespace BootLoader {
// load a model hub from the formatted file
std::shared_ptr<ModelHub> load_model_hub(const std::string& filename);
};  // namespace BootLoader

#endif