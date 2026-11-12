#pragma once

#include "config.h"
#include <vector>
#include <deque>
#include <string>

class TuringMachine {
    static constexpr int MAX_STEPS = 100000;

    Config config;
    std::vector<std::deque<char>> tapes;

    void printStep(int step, const TransitionKey& key, const std::vector<int>& head, int tapeStart);
    bool checkState(const std::string& state, const std::string& input);

public:
    TuringMachine(const Config& config);
    bool run(std::string input);
    void printConfig();
};