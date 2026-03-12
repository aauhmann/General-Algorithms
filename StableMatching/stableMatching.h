/*Stable Matching program based on Gale-Shapley algorithm*/
#include <vector>

class Person {
public:
    Person(int id, int amount);

    int GetId();

    int GetMatch();

    void SetMatch(int id);

    std::vector<int> GetPreferences();

protected:
    int id;
    int match;
    std::vector<int> preferences;

    void GeneratePreferences(int amount);
};

class Man : public Person {
public:
    Man(int id, int amount);

    // Proposes to a woman and removes her from the yetToPropose list
    int Propose();

    // Prints preferences and match (both by the ID's)
    void Print();

private:
    std::vector<int> yetToPropose;
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
    // Default constructor
    StableMatch();

    // Constructor with automatic stable match generation
    StableMatch(int pairAmount);

    // Prints both groups preferences and matches
    void Print();

    // Generates a stable match based on random preferences of a given amount of pairs
    void GenerateMatch(int pairAmount);

private:
    std::vector<Man> male;
    std::vector<Woman> female;
    int amount;
};