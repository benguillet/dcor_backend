#include "database.h"
#include "csv.h"
#include "algo.h"
#include <iostream>
#include <fstream>

int main(int argc, const char *argv[]) {
    std::ifstream param_file;
    std::vector< std::string > params;
    std::map <std::string, std::vector<float> > data;

    if (argc > 1) {
        param_file.open(argv[1]);
    }
    else {
        std::cout << "Usage: " << argv[0] << " filename" << std::endl;
        return -1;
    }
  
    params = fill_params_array(&param_file);
    PGconn *conn = connect_db(); 
    //fill_tables(conn, "data/vym_prices.csv");
    data = fetch_data(conn, params);
    print_data(data);
    //get_least_loss(params, data);
    //get_greatest_loss(params, data);
    get_right_combination(params, data);
        
    return 0;
}
