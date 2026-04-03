#include <iostream>
#include <random>
#include <algorithm>
#include "stableMatching.h"

Person::Person(int id, int amount) : id(id), match(0) {
    GeneratePreferences(amount);
}

int Person::GetId() { return id; }

int Person::GetMatch() { return match; }

void Person::SetMatch(int id) {
    using namespace std;

    if (id < 0 || id > preferences.size()) {
        cout << "ID " << id << " is invalid" << endl;
    }
    else {
        match = id;
    }
}

std::vector<int> Person::GetPreferences() { return preferences; }

void Person::GeneratePreferences(int amount) {
    std::vector<int> available;

    for (int i = 0; i < amount; i++) {
        available.push_back(i + 1);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::shuffle(available.begin(), available.end(), gen);

    int aux;
    for (int i = 0; i < amount; i++) {
        aux = available.back();
        available.pop_back();
        preferences.push_back(aux);
    }
}

Man::Man(int id, int amount) : Person(id, amount) {
    yetToPropose = preferences;
}

int Man::Propose() {
    if (!yetToPropose.empty()) {
        int aux = yetToPropose[0];
        yetToPropose.erase(yetToPropose.begin());
        return aux;
    }

    return 0;
}

void Man::Print() {
    using namespace std;

    cout << "\nMan " << id << "\nPrefs: " << preferences[0];
    for (int i = 1; i < preferences.size(); i++) {
        cout << ", " << preferences[i];
    }

    cout << "\nMatch: ";
    if (match == 0) {
        cout << "None" << endl;
    }
    else {
        cout << match << endl;
    }
}

Woman::Woman(int id, int amount) : Person(id, amount) {}

bool Woman::Accept(int proposerId) {
    if (match == 0) {
        return true;
    }

    else {
        int amount = preferences.size();
        for (int i = 0; i < amount; i++) {
            if (preferences[i] == proposerId) {
                return true;
            }
            else if (preferences[i] == match) {
                return false;
            }
        }
        return true;
    }
}

void Woman::Print() {
    using namespace std;

    cout << "\nWoman " << id << "\nPrefs: " << preferences[0];
    for (int i = 1; i < preferences.size(); i++) {
        cout << ", " << preferences[i];
    }

    cout << "\nMatch: ";
    if (match == 0) {
        cout << "None" << endl;
    }
    else {
        cout << match << endl;
    }
}

StableMatch::StableMatch() : amount(0) {}

StableMatch::StableMatch(int pairAmount) {
    GenerateMatch(pairAmount);
}

void StableMatch::Print() {
    using namespace std;

    cout << "\n===== PRINTING STABLE MATCH (" << amount << " PAIRS) =====";

    for (int i = 0; i < amount; i++) {
        male[i].Print();
    }

    cout << "\n----------------------" << endl;
    
    for (int i = 0; i < amount; i++) {
        female[i].Print();
    }

    cout << "===== END PRINTING =====\n" << endl;
}

void StableMatch::GenerateMatch(int pairAmount) {
    using namespace std;

    if (pairAmount <= 0) {
        cout << "\nPair amount must be a positive number" << endl;
        return;
    }

    Clear();

    amount = pairAmount;

    vector<Man*> singleMen;
    vector<Man*> matchedMen;

    GenerateGroups(singleMen);

    while (!singleMen.empty()) {
        Man* proposer = singleMen[0];
        singleMen.erase(singleMen.begin()); // Removes current man from the singles list

        int desiredId = proposer->Propose();
        Woman* desired = nullptr;

        int i = 0;
        while (desired == nullptr) { // Searches for the current desired woman by it's ID
            if (female[i].GetId() == desiredId) {
                desired = &female[i];
            }
            i++;
        }

        if (desired->Accept(proposer->GetId())) { // Case desired woman accepts proposal
            int oldMatch = desired->GetMatch();
            desired->SetMatch(proposer->GetId());
            proposer->SetMatch(desired->GetId());
            matchedMen.push_back(proposer);

            if (oldMatch != 0) { // Case she had already accepted another proposal
                for (int j = 0; j < matchedMen.size(); j++) {
                    if (matchedMen[j]->GetId() == oldMatch) {
                        matchedMen[j]->SetMatch(0);
                        singleMen.push_back(matchedMen[j]);
                        matchedMen.erase(matchedMen.begin() + j);
                        break;
                    }
                }
            }
        }

        else { // Case desired woman refuses proposal
            singleMen.push_back(proposer); // Puts the man back in the singles list (at the end of it)
        }
    }
}

void StableMatch::Clear() {
    male.clear();
    female.clear();
    amount = 0;
}

void StableMatch::GenerateGroups(std::vector<Man*>& singleMen) {
    // Creates two separate groups
    for (int i = 0; i < amount; i++) {
        male.push_back(Man(i + 1, amount));
        female.push_back(Woman(i + 1, amount));
    }

    male.shrink_to_fit();
    female.shrink_to_fit();

    // Sets all men as single
    for (int i = 0; i < amount; i++) {
        singleMen.push_back(&male[i]);
    }
}