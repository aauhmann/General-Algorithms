#include <iostream>
#include "stableMatching.h"

#define EXIT -1

int main() {
    using namespace std;

    int amount = 0;
    StableMatch match;

    while (amount != EXIT) {
        do {
            cout << "\nEnter amount of pairs (type -1 to exit): ";
            cin >> amount;
        } while (amount < EXIT);

        if (amount == EXIT) {
            cout << "\nExiting..." << endl;
        }
        else {
            match.GenerateMatch(amount);
            match.Print();
        }
    }

    return 0;
}