#pragma once

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <istream>
#include <optional>

enum class Move {Left, Right, Stay};

struct TransitionKey {
    std::string currentState;
    std::vector<char> read;

    TransitionKey();
    TransitionKey(std::string state, std::vector<char> read);

    bool operator<(const TransitionKey& other) const {
        return std::tie(currentState, read) < std::tie(other.currentState, other.read);
    }
};

struct TransitionAction {
    std::string nextState;
    std::vector<char> write;
    std::vector<Move> moves;
};

class Transitions {
    std::map<TransitionKey, TransitionAction> table;

public:
    static Transitions load(std::istream& input, int tapes);
    std::optional<TransitionAction> find(const TransitionKey& key) const;
    void print() const;
    void print(const TransitionKey& key) const;
    bool empty() const;
};