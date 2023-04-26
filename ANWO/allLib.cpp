#include "allLib.h"
std::mt19937 gen(
    int(std::chrono::steady_clock::now().time_since_epoch().count()));
std::uniform_int_distribution<> dis;
std::ifstream fin("input.txt");
std::ofstream fout("output.txt");
std::string out = "";