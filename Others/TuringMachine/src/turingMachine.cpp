#include "turingMachine.h"
#include <stdexcept>
#include <iostream>

using namespace std;

TuringMachine::TuringMachine(const Config& config) : config(config) {
    if (config.tapes < 1)
        throw invalid_argument("config must have at least one tape");
    if (config.transitions.empty())
        throw invalid_argument("config has no transitions");
    if (config.acceptState == config.rejectState)
        throw invalid_argument("accept and reject states must be different");

    tapes.assign(config.tapes, deque<char>{config.blankSymbol});
}

bool TuringMachine::run(string input) {
    vector<int> head(config.tapes, 0); // Creates all heads filled with zeroes
    int tapeStart = 0;
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

    cout << "========== RUNNING (" << input << ") ==========" << endl;

    cout << "---" << endl;

    while (steps > 0) {
        if (currentState == config.acceptState || currentState == config.rejectState) {
            bool result = checkState(currentState, input);
            cout << "========== RUN END ==========" << endl;
            return result;
        }

        for (int i = 0; i < config.tapes; i++) {
            read[i] = tapes[i][head[i]];
        }

        TransitionKey key(currentState, read);

        printStep(MAX_STEPS - steps, key, head, tapeStart);

        auto result = config.transitions.find(key);
        if (!result) {
            bool result = checkState(currentState, input);
            cout << "========== RUN END ==========" << endl;
            return result;
        }
        const TransitionAction& action = *result;

        currentState = action.nextState;

        bool pushBack = false, pushFront = false;
        for (int i = 0; i < config.tapes; i++) {
            tapes[i][head[i]] = action.write[i];

            switch(action.moves[i]) {
                case Move::Right: head[i]++; break;
                case Move::Left: head[i]--; break;
                case Move::Stay: break;
            }

            if (head[i] == (int)tapes[i].size()) {
                pushBack = true;
            }
            else if (head[i] < 0) {
                if (!config.bidirectional) {
                    cout << "\nCANCELLED (" << input << "): Invalid head move to the left" << endl;
                    cout << "========== RUN END ==========" << endl;
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
            tapeStart++;
        }

        steps--;
    }

    cout << "\nCANCELLED (" << input << "): Step limit (" << MAX_STEPS << ") reached" << endl;
    cout << "========== RUN END ==========" << endl;
    return false;
}

void TuringMachine::printConfig() {
    config.print();
}

void TuringMachine::printStep(int step, const TransitionKey& key, const vector<int>& head, int tapeStart) {
    cout << "Step " << (step) << ":" << endl;

    for (int i = 0; i < config.tapes; i++) {
        cout << "   >  ";
        for (int j = 0; j < (int)tapes[i].size(); j++) {
            if (j == head[i]) {
                cout << '[' << tapes[i][j] << (j == tapeStart ? "'" : "") << ']';
            }
            else {
                cout << ' ' << tapes[i][j] << (j == tapeStart ? "'" : " ");
            }
        }
        cout << "  <" << endl;
    }

    cout << "   ";
    config.transitions.print(key);

    cout << "---" << endl;
}

bool TuringMachine::checkState(const string& state, const string& input) {
    if (state == config.acceptState) {
        cout << "\nACCEPTED (" << input << ")" << endl;
        return true;
    }
    else {
        cout << "\nREJECTED (" << input << ")" << endl;
        return false;
    }
}