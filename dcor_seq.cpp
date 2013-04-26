#include "postgresql/libpq-fe.h"
#include "database.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <limits>

std::vector< std::string > fill_params_array(std::ifstream* param_file) {
    std::string param;
    std::vector< std::string > params;
    
    if (param_file->is_open()) {
        std::cout << "Params received:" << std::endl;
        /*
         * asset2
         * end_date
         * asset1
         * per2
         * amount
         * per1
         * start_date */
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

void print_data(std::vector< std::vector< std::string > > data) {
    int n_rows = data[0].size();
    //std::cout << n_rows << std::endl;
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < data.size(); ++j) {
            std::cout << data[j][i] << "|";
        }
        std::cout << std::endl;
    }
} 


void get_least_loss(std::vector< std::string > params, std::vector< std::vector< std::string > > data) {
    float perc1 = ::atof(params[5].c_str()) / 100.0;
    float perc2 = ::atof(params[3].c_str()) / 100.0;
    int best_start_date = 0;
    int best_end_date   = 0;
    float best_return = -std::numeric_limits<float>::max();
    float value_asset1_start_date = 0.0; 
    float value_asset2_start_date = 0.0;
    float return_asset1 = 0.0;
    float return_asset2 = 0.0;
    float total_current_return = 0.0;
    
    int n_rows = data[0].size();
    //std::cout << n_rows << std::endl;
    for (int index_start_date = 0; index_start_date < n_rows - 1; ++index_start_date) {
        value_asset1_start_date = ::atof(data[1][index_start_date].c_str());
        value_asset2_start_date = ::atof(data[2][index_start_date].c_str());
        for (int current_date = index_start_date + 1; current_date < n_rows; ++current_date) {
            return_asset1 = (::atof(data[1][current_date].c_str()) - value_asset1_start_date) / value_asset1_start_date;
            return_asset2 = (::atof(data[2][current_date].c_str()) - value_asset2_start_date) / value_asset2_start_date;
            total_current_return =  (perc1 * return_asset1 + perc2 * return_asset2) * 100.0;
            
            if (total_current_return > best_return) {
                std::cout << "we have a better return: " << total_current_return << std::endl;
                best_return = total_current_return;
                best_start_date = index_start_date;
                best_end_date = current_date;
            }
        }
    }
    std::cout << "Best return is " << best_return;
    std::cout << " if you start on " << data[0][best_start_date];
    std::cout << " and stop on " << data[0][best_end_date];
    std::cout << std::endl;
}

void get_greatest_loss(std::vector< std::string > params, std::vector< std::vector< std::string > > data) {
    float perc1 = ::atof(params[5].c_str()) / 100.0;
    float perc2 = ::atof(params[3].c_str()) / 100.0;
    int worst_start_date = 0;
    int worst_end_date   = 0;
    float worst_return = std::numeric_limits<float>::max();
    float value_asset1_start_date = 0.0; 
    float value_asset2_start_date = 0.0;
    float return_asset1 = 0.0;
    float return_asset2 = 0.0;
    float total_current_return = 0.0;
    
    int n_rows = data[0].size();
    //std::cout << n_rows << std::endl;
    for (int index_start_date = 0; index_start_date < n_rows - 1; ++index_start_date) {
        value_asset1_start_date = ::atof(data[1][index_start_date].c_str());
        value_asset2_start_date = ::atof(data[2][index_start_date].c_str());
        for (int current_date = index_start_date + 1; current_date < n_rows; ++current_date) {
            return_asset1 = (::atof(data[1][current_date].c_str()) - value_asset1_start_date) / value_asset1_start_date;
            return_asset2 = (::atof(data[2][current_date].c_str()) - value_asset2_start_date) / value_asset2_start_date;
            total_current_return =  (perc1 * return_asset1 + perc2 * return_asset2) * 100.0;
            
            if (total_current_return < worst_return) {
                std::cout << "we have a worst return: " << total_current_return << std::endl;
                worst_return = total_current_return;
                worst_start_date = index_start_date;
                worst_end_date = current_date;
            }
        }
    }
    std::cout << "Worst return is " << worst_return;
    std::cout << " if you start on " << data[0][worst_start_date];
    std::cout << " and stop on " << data[0][worst_end_date];
    std::cout << std::endl;
}

int main(int argc, const char *argv[]) {
    std::ifstream param_file;
    std::vector< std::string > params;
    std::vector< std::vector< std::string > > data; 
    
    if (argc > 1) {
        param_file.open(argv[1]);
    } else {
        std::cout << "Usage: " << argv[0] << " filename" << std::endl;
        return -1;
    }
  
    params = fill_params_array(&param_file);
    PGconn *conn = connect_db(); 
    data = fetch_data(conn, params);
    print_data(data);
    get_least_loss(params, data);
    get_greatest_loss(params, data);
    
    return 0;
}
