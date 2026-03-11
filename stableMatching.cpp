#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

class Person {
public:
    int match;

    Person(int id, int amount) : id(id), match(0) {
        GeneratePreferences(amount);
    }

    int GetId() {
        return id;
    }

    std::vector<int> GetPreferences() {
        return preferences;
    }

protected:
    int id;
    std::vector<int> preferences;

    void GeneratePreferences(int n) {
        std::vector<int> available;

        for (int i = 0; i < n; i++) {
            available.push_back(i + 1);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::shuffle(available.begin(), available.end(), gen);

        int aux;
        for (int i = 0; i < n; i++) {
            aux = available.back();
            available.pop_back();
            preferences.push_back(aux);
        }
    }
};

class Man : public Person {
public:
    std::vector<int> yetToPropose;

    Man(int id, int amount) : Person(id, amount) {
        yetToPropose = preferences;
    }

    // Proposes to a woman and removes her from the yetToPropose list
    int Propose() {
        if (!yetToPropose.empty()) {
            int aux = yetToPropose[0];
            yetToPropose.erase(yetToPropose.begin());

            return aux;
        }

        return 0;
    }

    // Prints preferences and match (both by the ID's)
    void Print() {
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
};

class Woman : public Person {
public:
    Woman(int id, int amount) : Person(id, amount) {}

    // Decides rather to accept or refuse a proposal
    bool Accept(int proposerId) {
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

    // Prints preferences and match (both by the ID's)
    void Print() {
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
};

int main() {
    using namespace std;

    int amount = 0;

    while (amount != -1) {
        do {
            cout << "\nEnter amount of pairs (type -1 to exit): ";
            cin >> amount;
        } while (amount < -1);

        if (amount == -1) {
            cout << "\n\nExiting..." << endl;
        }

        else {
            vector<Man> male;
            vector<Woman> female;
            vector<Man*> singleMen;
            vector<Man*> matchedMen;

            // Creates two separate groups
            for (int i = 0; i < amount; i++) {
                male.push_back(Man(i + 1, amount));
                female.push_back(Woman(i + 1, amount));
            }

            // Sets all men as single
            for (int i = 0; i < amount; i++) {
                singleMen.push_back(&male[i]);
            }

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
                    int oldMatch = desired->match;
                    desired->match = proposer->GetId();
                    proposer->match = desired->GetId();

                    matchedMen.push_back(proposer);

                    if (oldMatch != 0) { // Case she had already accepted another proposal
                        for (int j = 0; j < matchedMen.size(); j++) {
                            if (matchedMen[j]->GetId() == oldMatch) {
                                matchedMen[j]->match = 0;
                                singleMen.push_back(matchedMen[j]);
                                matchedMen.erase(matchedMen.begin() + j);
                                break;
                            }
                        }
                    }
                }
                else { // Case desired woman refuses proposal
                    singleMen.push_back(proposer); // Puts the man back in the singles list at the end of it
                }
            }

            // Prints both groups
            for (int i = 0; i < amount; i++) {
                male[i].Print();
            }

            for (int i = 0; i < amount; i++) {
                female[i].Print();
            }
        }
    }

    return 0;
}