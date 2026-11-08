#include <string>

using namespace std;

string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r");
    return s.substr(start, end - start + 1);
}