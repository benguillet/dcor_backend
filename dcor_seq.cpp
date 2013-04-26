#include "postgresql/libpq-fe.h"
#include "database.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

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
    float portfolio_return = 0.0;
    float perc1 = ::atof(params[5].c_str()) / 100.0;
    float perc2 = ::atof(params[3].c_str()) / 100.0;

    int n_rows = data[0].size();
    for (int i = 0; i < n_rows; ++i) {
        portfolio_return =  perc1 * ::atof(data[1][i].c_str()) + perc2 * ::atof(data[2][i].c_str());
    }
    std::cout << portfolio_return << std::endl;
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
    //print_data(data);
    get_least_loss(params, data);
    
    return 0;
}
