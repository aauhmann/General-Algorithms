#pragma once

#include "transitions.h"
#include <string>

struct Config {
    int tapes = 1;
    bool bidirectional = false;
    char blank = '_';
    std::string initialState = "q0";
    std::string acceptState = "qA";
    std::string rejectState = "qR";
    Transitions transitions;

    void print();
    static Config load(const std::string& path);
};