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

class PortReturn {
    public:
        float value;
        std::string start_date;
        std::string end_date;
        std::string combination;
        std::string percentages;

        PortReturn() : value(0.0), start_date(""), end_date(""), combination(""), percentages("") {}

        PortReturn(float value) {
            this->value = value;
            this->start_date = "";
            this->end_date = "";
            this->combination = "";
            this->percentages = "";
        }
        PortReturn(float value, std::string start_date, std::string end_date, std::string combination, std::string percentages) {
            this->value = value;
            this->start_date = start_date;
            this->end_date = end_date;
            this->combination = combination;
            this->percentages = percentages;
        }

        PortReturn(const PortReturn& pr) {
            this->value =  pr.value;
            this->start_date = pr.start_date;
            this->end_date = pr.end_date;
            this->combination = pr.combination;
            this->percentages = pr.percentages;
        }

        PortReturn& operator=(const PortReturn& pr) {
            this->value =  pr.value;
            this->start_date = pr.start_date;
            this->end_date = pr.end_date;
            this->combination = pr.combination;
            this->percentages = pr.percentages;
            return *this;
        } 

        ~PortReturn() {}
};

std::vector< std::string > fill_params_array(std::ifstream* param_file);
void print_data(std::map <std::string, std::vector<float> > data); 
void get_least_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data);
PortReturn get_greatest_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data, std::string asset_combination); 
void get_right_combination(std::vector< std::string > params, std::map <std::string, std::vector<float> > data); 
#endif
