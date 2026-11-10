#include "turingMachine.h"
#include <stdexcept>
#include <iostream>

using namespace std;

TuringMachine::TuringMachine(const Config& config) : config(config) {
    if (config.tapes < 1)
        throw invalid_argument("config must have at least one tape");
    if (config.transitions.table.empty())
        throw invalid_argument("config has no transitions");
    if (config.acceptState == config.rejectState)
        throw invalid_argument("accept and reject states must be different");

    tapes.assign(config.tapes, deque<char>{config.blankSymbol});
}

bool TuringMachine::run(string input) {
    vector<int> head(config.tapes, 0); // Creates all heads filled with zeroes
    int steps = MAX_STEPS;
    string currentState = config.initialState;
    vector<char> read(config.tapes);
    
    tapes[0].assign(input.begin(), input.end()); // Copies input into the first tape
    for (int i = 1; i < config.tapes; i++) { // Creates the rest of the tapes with the same size as the first one
        tapes[i].assign(tapes[0].size(), config.blankSymbol);
    }

    for (deque<char>& tape : tapes) {
        if (tape.empty()) {
            tape.push_back(config.blankSymbol);
        }
    }

    printStep(0, head);

    while (steps > 0) {
        if (currentState == config.acceptState || currentState == config.rejectState) {
            return checkState(currentState, input);
        }

        for (int i = 0; i < config.tapes; i++) {
            read[i] = tapes[i][head[i]];
        }

        TransitionKey key(currentState, read);
        auto iterator = config.transitions.table.find(key);
        if (iterator == config.transitions.table.end()) {
            return checkState(currentState, input);
        }

        const TransitionAction& action = iterator->second;

        currentState = action.nextState;

        bool pushBack = false, pushFront = false;
        for (int i = 0; i < config.tapes; i++) {
            tapes[i][head[i]] = action.write[i];

            switch(action.moves[i]) {
                case Move::Right: head[i]++; break;
                case Move::Left: head[i]--; break;
                case Move::Stay: break;
            }

            if (head[i] == tapes[i].size()) {
                pushBack = true;
            }
            else if (head[i] < 0) {
                if (!config.bidirectional) {
                    cout << "Cancelled (" << input << "): Invalid head move to the left" << endl;
                    return false;
                }
                else {
                    pushFront = true;
                }
            }
        }

        if (pushBack) {
            for (deque<char>& tape : tapes) {
                tape.push_back(config.blankSymbol);
            }
        }

        if (pushFront) {
            for (int i = 0; i < config.tapes; i++) {
                tapes[i].push_front(config.blankSymbol);
                head[i]++;
            }
        }

        steps--;

        printStep(MAX_STEPS - steps, head);
    }

    cout << "Cancelled (" << input << "): Step limit (" << MAX_STEPS << ") reached" << endl;
    return false;
}

void TuringMachine::printConfig() {
    config.print();
}

void TuringMachine::printStep(int step, const vector<int>& head) {
    cout << ">" << endl;
    cout << "Step " << (step) << ":" << endl;
    for (int i = 0; i < config.tapes; i++) {
        for (int j = 0; j < tapes[i].size(); j++) {
            if (j == head[i]) {
                cout << '[' << tapes[i][j] << ']';
            }
            else {
                cout << ' ' << tapes[i][j] << ' ';
            }
        }
        cout << endl;
    }
}

bool TuringMachine::checkState(const string& state, const string& input) {
    if (state == config.acceptState) {
        cout << "Accepted (" << input << ")" << endl;
        return true;
    }
    else {
        cout << "Rejected (" << input << ")" << endl;
        return false;
    }
}