#pragma once
#include <unordered_map>
#include <string>
#include "wrapper_classes.h"

class Engine {
private:
    std::unordered_map<std::string, Wrapper *> commands;

public:
    void register_command(std::string name, Wrapper *wrapper);

    int execute(std::string name, const std::unordered_map<std::string, int> &args);
};

