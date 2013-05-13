#ifndef _ALGO_H_
#define _ALGO_H_

#include "utils.h"
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
        std::string portfolio_distribution;

        PortReturn() : value(0.0), start_date(""), end_date(""), combination(""), percentages("") , portfolio_distribution("") {}

        PortReturn(float value) {
            this->value = value;
            this->start_date = "";
            this->end_date = "";
            this->combination = "";
            this->percentages = "";
            this->portfolio_distribution = "";
        }
        PortReturn(float value, std::string start_date, std::string end_date, std::string combination, std::string percentages, std::string portfolio_distribution) {
            this->value = value;
            this->start_date = start_date;
            this->end_date = end_date;
            this->combination = combination;
            this->percentages = percentages;
            this->portfolio_distribution = portfolio_distribution;
        }

        PortReturn(const PortReturn& pr) {
            this->value =  pr.value;
            this->start_date = pr.start_date;
            this->end_date = pr.end_date;
            this->combination = pr.combination;
            this->percentages = pr.percentages;
            this->portfolio_distribution = pr.portfolio_distribution;
        }

        PortReturn& operator=(const PortReturn& pr) {
            this->value =  pr.value;
            this->start_date = pr.start_date;
            this->end_date = pr.end_date;
            this->combination = pr.combination;
            this->percentages = pr.percentages;
            this->portfolio_distribution = pr.portfolio_distribution;
            return *this;
        } 

        ~PortReturn() {}
};

std::vector< std::string > fill_params_array(std::ifstream* param_file);
void print_data(std::map <std::string, std::vector<float> > data); 
void get_least_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data);
PortReturn get_greatest_loss(std::map<std::string, std::vector<float> > data, std::vector<std::string> all_assets, std::string asset_combination, int combi_size, std::string percentages); 
std::string get_portfolio_distribution(std::map<std::string, std::vector<float> > data, std::vector<std::string> all_assets, std::string combination, int combi_size); 
PortReturn get_right_combination(std::vector< std::string > params, std::map <std::string, std::vector<float> > data); 
std::string write_results(PortReturn port_return, std::string original_filename); 
void send_file_back(std::string result_filepath);


#endif
