#include "algo.h"

std::vector< std::string > fill_params_array(std::ifstream* param_file) {
    std::string param;
    std::vector< std::string > params;
    
    if (param_file->is_open()) {
        std::cout << "Params received:" << std::endl;
        /*
         * start_date
         * end_date
         * assets
         * pers if two assets mode
         * portofolio_size
         * */
        while (getline(*param_file, param)) {
            std::cout << param << std::endl;
            params.push_back(param);
        }
        param_file->close();
    } else {
        std::cout << "Unable to open file" << std::endl;
    }
 
    return params; 
}

void print_data(std::map< std::string, std::vector<float> > data) {
    for (std::map<std::string, std::vector<float> >::iterator it = data.begin(); it != data.end(); ++it) {
        std::cout << it->first << " => ";
        for (int i = 0; i < it->second.size(); ++i) {
            std::cout << it->second[i] << "|"; 
        }
        std::cout << std::endl;
    }
} 

void get_least_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data) {
    std::vector<std::string> pers = split(params[3].c_str(), ',');
    float perc1 = ::atof(pers[0].c_str()) / 100.0;
    float perc2 = ::atof(pers[1].c_str()) / 100.0;
    float best_return = -std::numeric_limits<float>::max();
    float value_asset1_start_date = 0.0; 
    float value_asset2_start_date = 0.0;
    float return_asset1 = 0.0;
    float return_asset2 = 0.0;
    float total_current_return = 0.0;
    std::string best_start_date = "";
    std::string best_end_date   = "";
     
    std::map<std::string, std::vector<float> >::iterator data_end = data.end();
    for (std::map<std::string, std::vector<float> >::iterator index_start_date = data.begin(); index_start_date != data_end; ++index_start_date) {
        value_asset1_start_date = index_start_date->second[0];
        value_asset2_start_date = index_start_date->second[1];
        std::map<std::string, std::vector<float> >::iterator index_start_date_copy = index_start_date;
        for (std::map<std::string, std::vector<float> >::iterator current_date = ++index_start_date_copy; current_date != data_end; ++current_date) {
            return_asset1 = (current_date->second[0] - value_asset1_start_date) / value_asset1_start_date;
            return_asset2 = (current_date->second[1] - value_asset2_start_date) / value_asset2_start_date;
            total_current_return =  (perc1 * return_asset1 + perc2 * return_asset2) * 100.0;
            
            if (total_current_return > best_return) {
                std::cout << "we have a better return: " << total_current_return << std::endl;
                best_return = total_current_return;
                best_start_date = index_start_date->first;
                best_end_date = current_date->first;
            }
        }
     
    } 
    std::cout << "Best return is " << best_return;
    std::cout << " if you start on " << best_start_date;
    std::cout << " and stop on " << best_end_date;
    std::cout << std::endl;

}

void get_greatest_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data) {
    std::vector<std::string> pers = split(params[3].c_str(), ',');
    float perc1 = ::atof(pers[0].c_str()) / 100.0;
    float perc2 = ::atof(pers[1].c_str()) / 100.0;
    float worst_return = std::numeric_limits<float>::max();
    float value_asset1_start_date = 0.0; 
    float value_asset2_start_date = 0.0;
    float return_asset1 = 0.0;
    float return_asset2 = 0.0;
    float total_current_return = 0.0;
    std::string worst_start_date = "";
    std::string worst_end_date   = "";
     
    std::map<std::string, std::vector<float> >::iterator data_end = data.end();
    for (std::map<std::string, std::vector<float> >::iterator index_start_date = data.begin(); index_start_date != data_end; ++index_start_date) {
        value_asset1_start_date = index_start_date->second[0];
        value_asset2_start_date = index_start_date->second[1];
        std::map<std::string, std::vector<float> >::iterator index_start_date_copy = index_start_date;
        for (std::map<std::string, std::vector<float> >::iterator current_date = ++index_start_date_copy; current_date != data_end; ++current_date) {
            return_asset1 = (current_date->second[0] - value_asset1_start_date) / value_asset1_start_date;
            return_asset2 = (current_date->second[1] - value_asset2_start_date) / value_asset2_start_date;
            total_current_return =  (perc1 * return_asset1 + perc2 * return_asset2) * 100.0;
            
            if (total_current_return < worst_return) {
                std::cout << "we have a worst return: " << total_current_return << std::endl;
                worst_return = total_current_return;
                worst_start_date = index_start_date->first;
                worst_end_date = current_date->first;
            }
        }
     
    } 
    std::cout << "Worst return is " << worst_return;
    std::cout << " if you start on " << worst_start_date;
    std::cout << " and stop on " << worst_end_date;
    std::cout << std::endl;
}

void get_right_combinaison(std::vector< std::string > params, std::map <std::string, std::vector<float> > data) {

}


