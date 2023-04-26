#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>
#include <algorithm>

//#define fout std::cout

extern std::mt19937 gen;
extern std::uniform_int_distribution<> dis;
extern std::ofstream fout;
extern std::ifstream fin;
extern std::string out;

const int INF = int(1e5);




