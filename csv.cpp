#include "csv.h"
#include <sstream>

unsigned int iter_factorial(unsigned int n) {
    unsigned int ret = 1;
    for (unsigned int i = 1; i <= n; ++i) {
        ret *= i;
    }
    return ret;
}


std::string intToString(int number) {
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}



