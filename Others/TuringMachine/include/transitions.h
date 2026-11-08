#pragma once

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <istream>

enum class Move {Left, Right, Stay};

struct TransitionKey {
    std::string currentState;
    std::vector<char> read;

    bool operator<(const TransitionKey& other) const {
        return std::tie(currentState, read) < std::tie(other.currentState, other.read);
    }
};

struct TransitionAction {
    std::string nextState;
    std::vector<char> write;
    std::vector<Move> moves;
};

struct Transitions {
    std::map<TransitionKey, TransitionAction> table;

    void print();
    static Transitions load(std::istream& input, int tapes);
};