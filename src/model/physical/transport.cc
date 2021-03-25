#include "model/physical/transport.h"
using std::string;
Transport::Transport(string name, StdTime from, StdTime to, float speed)
    : Path{name}, from{from}, to{to}, speed{speed} {
}