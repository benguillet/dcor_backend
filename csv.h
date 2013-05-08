#ifndef CSV_H
#define CSV_H

#include <vector>
#include <string>

unsigned int iter_factorial(unsigned int n);
std::string intToString(int number);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

#endif
