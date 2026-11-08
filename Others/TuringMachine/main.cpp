#include "config.h"
#include <iostream>
#include <string_view>

using namespace std;

constexpr string_view DEFAULT_CONFIG_PATH = "./config";

int main() {
    Config config;

    config = Config::load(string(DEFAULT_CONFIG_PATH));
}