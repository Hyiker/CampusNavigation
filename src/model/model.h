#ifndef MODEL_H
#define MODEL_H
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
// id for each model (include path and buildings)
using Id = int32_t;
using StdTime = std::chrono::time_point<std::chrono::system_clock>;
// father class of both logical & physical models
// store the basic info of models
// all the models are stored in one "Hub" with a HashMap(unordered_map)
class Model {
   private:
    // a must-have key for models
    Id id = -1;
    // name of the model
    std::string name = "";

   protected:
    Model(std::string);
    Model() = default;

   public:
    void set_id(Id);
    Id get_id();
    void set_name(std::string);
    std::string get_name();
};

#endif  // !MODEL_H