#ifndef _ALGO_H_
#define _ALGO_H_

#include "csv.h"
#include <iostream>
#include <fstream>
#include <map>
#include <limits>
#include <cstdlib>
#include <algorithm>
#include <sstream>

std::vector< std::string > fill_params_array(std::ifstream* param_file);
void print_data(std::map <std::string, std::vector<float> > data); 
void get_least_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data);
float get_greatest_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data, std::string asset_combination); 
void get_right_combination(std::vector< std::string > params, std::map <std::string, std::vector<float> > data); 
#endif
