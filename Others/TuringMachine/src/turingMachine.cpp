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
    int tapeStart = 0; // Keeps track of where all tapes started
    int steps = MAX_STEPS;
    string currentState = config.initialState;
    vector<char> read(config.tapes);
    
    tapes[0].assign(input.begin(), input.end()); // Copies input into the first tape
    for (int i = 1; i < config.tapes; i++) { // Creates the rest of the tapes with the same size as the first one
        tapes[i].assign(tapes[0].size(), config.blankSymbol);
    }

    for (deque<char>& tape : tapes) { // If the input is blank, all tapes begin with a blank symbol
        if (tape.empty()) {
            tape.push_back(config.blankSymbol);
        }
    }

    cout << "========== RUNNING (" << input << ") ==========" << endl;

    cout << "---" << endl;

    while (steps > 0) { // Guarantees that if a loop happens the program ends
        if (currentState == config.acceptState || currentState == config.rejectState) {
            bool result = checkState(currentState, input);
            cout << "========== RUN END ==========" << endl;
            return result;
        }

        for (int i = 0; i < config.tapes; i++) { // Reads the character pointed by the head of each tape
            read[i] = tapes[i][head[i]];
        }

        TransitionKey key(currentState, read);

        printStep(MAX_STEPS - steps, key, head, tapeStart);

        auto result = config.transitions.find(key);
        if (!result) { // If there's no result for the current key checks if it stopped on an accept state or not
            bool result = checkState(currentState, input);
            cout << "========== RUN END ==========" << endl;
            return result;
        }
        const TransitionAction& action = *result;

        currentState = action.nextState;

        bool pushBack = false, pushFront = false;
        for (int i = 0; i < config.tapes; i++) { // Handles writing and movement of every tape
            tapes[i][head[i]] = action.write[i]; // Writes to where the head points

            switch(action.moves[i]) { // Moves the head
                case Move::Right: head[i]++; break;
                case Move::Left: head[i]--; break;
                case Move::Stay: break;
            }

            // Controls the "out of bounds" head movement
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

        if (pushBack) { // Grows all tapes by one on the right if any head tried to access a element after where the deque ends
            for (deque<char>& tape : tapes) {
                tape.push_back(config.blankSymbol);
            }
        }

        if (pushFront) { // Grows all tapes by one on the left if any head tried to go negative
            for (int i = 0; i < config.tapes; i++) {
                tapes[i].push_front(config.blankSymbol);
                head[i]++; // Head increments since a new blank symbol was added the beginning of the deque
            }
            tapeStart++; // Since all elements were shifted by one to the right, so is the start of all tapes
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