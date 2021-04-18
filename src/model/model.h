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

const std::string BUILDING_STR = "building", CAMPUS_STR = "campus", PATH_STR = "path", SERVICE_STR = "service",
                  TRANSPORT_STR = "transport", COURSE_STR = "course";

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
    // init from a vector
    // available information truncates from 2:-1
    virtual void init(Id, std::vector<std::string>&) = 0;
    void set_id(Id);
    Id get_id();
    void set_name(std::string);
    std::string get_name();
};

#endif  // !MODEL_H