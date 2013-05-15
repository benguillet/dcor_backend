/* author: Benjamin Guillet <benjamin.guillet@gmail.com>
Lost in the code? Any questions? Shout me an email. */

#include "database.h"
#include "utils.h"
#include "algo.h"
#include <iostream>
#include <fstream>

int main(int argc, const char *argv[]) {
    std::ifstream param_file;
    std::string name_param_file;
    std::string result_filepath;
    std::vector< std::string > params;
    std::map <std::string, std::vector<float> > data;

    if (argc > 1) {
        name_param_file = argv[1];
        param_file.open(argv[1]);
    }
    else {
        std::cout << "Usage: " << argv[0] << " filename" << std::endl;
        return -1;
    }
    params = fill_params_array(&param_file);
    PGconn *conn = connect_db(); 
    //fill_tables(conn, "data/a_lot_of_assets_yeah2.csv");
    data = fetch_data(conn, params);
    //print_data(data);
    PortReturn port_return = get_right_combination(params, data);
    result_filepath = write_results(port_return, name_param_file); 
    send_file_back(result_filepath);
    return 0;
}
