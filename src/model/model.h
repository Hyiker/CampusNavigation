#ifndef MODEL_H
#define MODEL_H
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
// id for each model (include path and buildings)
using Id = int32_t;
static const Id DEFAULT_ID_BEGIN = 0;
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

class ModelHub {
   private:
    std::unordered_map<Id, std::shared_ptr<Model>> model_map;
    Id gen;
    Id generate_id();

   public:
    ModelHub(Id = DEFAULT_ID_BEGIN);
    Id add(std::shared_ptr<Model>);
    // return the removed id if exists
    // or -1 if failed to find or remove the model
    Id remove(Id);
    // true if hub has the corresponding model
    bool have(Id);
    std::shared_ptr<Model> get(Id);
    // the sequence is proved to be the same
    std::vector<std::shared_ptr<Model>> get(std::vector<Id>&);
};
#endif  // !MODEL_H