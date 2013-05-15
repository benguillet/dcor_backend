#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>

// Change this value to false to disable verbose mode
const bool DEBUG = true;

std::string intToString(int number);
std::string floatToString(float number);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif
