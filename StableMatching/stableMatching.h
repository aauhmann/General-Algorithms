/*Stable Matching program based on Gale-Shapley algorithm*/
#include <vector>

class Person {
public:
    int match;

    Person(int id, int amount);

    int GetId();

    std::vector<int> GetPreferences();

protected:
    int id;
    std::vector<int> preferences;

    void GeneratePreferences(int amount);
};

class Man : public Person {
public:
    std::vector<int> yetToPropose;

    Man(int id, int amount);

    // Proposes to a woman and removes her from the yetToPropose list
    int Propose();

    // Prints preferences and match (both by the ID's)
    void Print();
};

class Woman : public Person {
public:
    Woman(int id, int amount);

    // Decides rather to accept or refuse a proposal
    bool Accept(int proposerId);

    // Prints preferences and match (both by the ID's)
    void Print();
};

struct StableMatch {
public:
    StableMatch(std::vector<Man> male, std::vector<Woman> female, int amount);

    void Print();

private:
    std::vector<Man> male;
    std::vector<Woman> female;
    int amount;
};

// Generates a stable match based on random preferences of a given amount of pairs
StableMatch stableMatching(int amount);