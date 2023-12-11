#pragma once

#include "wrapper_classes.h"
#include <unordered_map>

class Engine {
private:
    std::unordered_map<std::string, Wrapper *> commands;
public:
    void register_command(std::string name, Wrapper *wrapper);

    int execute(std::string name, const std::unordered_map<std::string, int> &args);
};