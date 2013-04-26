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
    float current_return = 0.0;
    float value_start_date = 0.0;
    float value_current_date = 0.0;
    
    int n_rows = data[0].size();
    //std::cout << n_rows << std::endl;
    // optimization: right now some return of rows are computed multiple times,
    // return should be computed once and stored and reused
    for (int index_start_date = 0; index_start_date < n_rows - 1; ++index_start_date) {
        value_start_date =  perc1 * ::atof(data[1][index_start_date].c_str()) + perc2 * ::atof(data[2][index_start_date].c_str());
        //std::cout << value_start_date << std::endl;
        for (int current_date = index_start_date + 1; current_date < n_rows; ++current_date) {
            value_current_date =  perc1 * ::atof(data[1][current_date].c_str()) + perc2 * ::atof(data[2][current_date].c_str());
            //std::cout << value_current_date << std::endl;
            current_return = value_current_date - value_start_date;
            //std::cout << current_return << std::endl; 
            if (current_return > best_return) {
                std::cout << "we have a better return: " << current_return << std::endl;
                best_return = current_return;
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
    
    return 0;
}
