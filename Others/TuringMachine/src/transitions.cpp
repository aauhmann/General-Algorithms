#include "transitions.h"
#include "utils.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

static Move charToMove(char c);
static char moveToChar(Move m);
static void condPrint(char c, bool& cond);

TransitionKey::TransitionKey() {}
TransitionKey::TransitionKey(std::string state, std::vector<char> read) : currentState(state), read(read) {}

Transitions Transitions::load(istream& input, int tapes) {
    Transitions transitions;
    string line;

    while (getline(input, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        istringstream iss(line);
        TransitionKey key;
        TransitionAction action;
        string arrow;

        iss >> key.currentState;

        key.read.resize(tapes);
        for (char& c : key.read) iss >> c;

        iss >> arrow >> action.nextState;

        action.write.resize(tapes);
        for (char& c : action.write) iss >> c;

        action.moves.resize(tapes);
        for (Move& m : action.moves) {
            char c = '?';
            iss >> c;
            m = charToMove(c);
        }

        string extra;
        if (!iss || arrow != "->" || (iss >> extra))
            throw runtime_error("malformed transition: \"" + line + "\"");

        if (transitions.table.count(key))
            throw runtime_error("duplicate transition: \"" + line + "\"");

        transitions.table[key] = action;
    }

    return transitions;
}

optional<TransitionAction> Transitions::find(const TransitionKey& key) const {
    auto iterator = table.find(key);
    if (iterator == table.end()) {
        return nullopt;
    }
    return iterator->second;
}

void Transitions::print() const {
    cout << "Transitions (" << table.size() << "):" << endl;

    for (const auto& [key, action] : table) {
        cout << "   ";
        print(key);
    }
}

void Transitions::print(const TransitionKey& key) const {
    auto result = find(key);
    if (!result) {
        cout << "There's no match for this key";
    }
    const TransitionAction& action = *result;

    cout << "{ " << key.currentState << " (";

    bool spacePrint = false;
    for (char c : key.read) {
        condPrint(c, spacePrint);
    }

    cout << ")  ->  " << action.nextState << " (";

    spacePrint = false;
    for (char c : action.write) {
        condPrint(c, spacePrint);
    }

    cout << ") (";

    spacePrint = false;
    for (Move m : action.moves) {
        condPrint(moveToChar(m), spacePrint);
    }
    
    cout << ") }" << endl;
}

bool Transitions::empty() const {
    return table.empty();
}

static Move charToMove(char c) {
    switch (c) {
        case 'L': return Move::Left;
        case 'R': return Move::Right;
        case 'S': return Move::Stay;
        default: throw runtime_error(string("invalid move '") + c + "' (use L, R or S)");
    }
}

static char moveToChar(Move m) {
    switch (m) {
        case Move::Left:  return 'L';
        case Move::Right: return 'R';
        case Move::Stay:  return 'S';
    }
    return '?';
}

static void condPrint(char c, bool& cond) {
    if (cond) {
        cout << ',' << c;
    }
    else {
        cond = true;
        cout << c;
    }
}