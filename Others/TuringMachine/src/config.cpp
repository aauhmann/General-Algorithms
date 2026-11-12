#include "config.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

void Config::print() {
    cout << "\n========== TURING MACHINE ==========" << endl;
    cout << "Tapes: " << tapes << endl;
    cout << "Bidirectional: " << (bidirectional ? "Yes" : "No") << endl;
    cout << "Blank symbol: " << blankSymbol << endl;
    cout << "Initial state: " << initialState << endl;
    cout << "Accept state: " << acceptState << endl;
    cout << "Reject state: " << rejectState << endl;
    transitions.print();
    cout << "============= CONFIG END =============" << endl;
}

Config Config::load(const string& path) {
    Config config;
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error: Could not open config file: " << path << endl;
        exit(1);
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        // Everything after [transitions] is the transition table
        if (line == "[transitions]") {
            try {
                config.transitions = Transitions::load(file, config.tapes);
            }
            catch (const exception& e) {
                cerr << "Error (transitions): " << e.what() << endl;
                exit(1);
            }
            break;
        }

        size_t pos = line.find(':');
        if (pos == string::npos) {
            cerr << "Warning (line " << lineNumber << "): missing ':'" << endl;
            continue;
        }

        string key = trim(line.substr(0, pos));
        string value = trim(line.substr(pos + 1));

        if (key == "tapes") {
            try {
                config.tapes = stoi(value);
            }
            catch (const exception&) {
                cerr << "Error (line " << lineNumber << "): tapes must be a number" << endl;
                exit(1);
            }
        } 
        else if (key == "bidirectional") {
            config.bidirectional = (value == "true");
        } 
        else if (key == "blank_symbol") {
            config.blankSymbol = value.empty() ? '_' : value[0];
        } 
        else if (key == "initial_state") {
            config.initialState = value;
        } 
        else if (key == "accept_state") {
            config.acceptState = value;
        } 
        else if (key == "reject_state") {
            config.rejectState = value;
        } 
        else {
            cerr << "Warning (line " << lineNumber << "): unknown key '" << key << "'" << endl;
        }
    }

    return config;
}