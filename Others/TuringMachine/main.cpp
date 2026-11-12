#include "turingMachine.h"
#include "config.h"
#include <iostream>
#include <string_view>
#include <string>

using namespace std;

constexpr int EXIT = 3;
constexpr string_view DEFAULT_CONFIG_PATH = "./config";

int main() {
    Config config = Config::load(string(DEFAULT_CONFIG_PATH));
    TuringMachine tm(config);

    int menu;
    do {
        cout << "\n===== MENU =====" << endl;
        cout << "1. PRINT CONFIG" << endl;
        cout << "2. RUN" << endl;
        cout << "3. EXIT" << endl;

        cout << "Option: ";
        cin >> menu;
        while (getchar() != '\n');
        cout << "================" << endl;

        switch (menu) {
            case 1:
                tm.printConfig();
                cout << endl;
                break;
            case 2: {
                string input;
                cout << "\nInput to run: ";
                getline(cin, input);
                tm.run(input);
                cout << endl;
                break;}
            case 3: break;
        }
    } while (menu != EXIT);

    cout << "\nTuring Machine Simulator ending...\n" << endl;

    return 0;
}