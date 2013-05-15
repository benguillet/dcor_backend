/* author: Benjamin Guillet <benjamin.guillet@gmail.com>
Lost in the code? Any questions? Shout me an email. */

#include "algo.h"

std::vector< std::string > fill_params_array(std::ifstream* param_file) {
    std::string param;
    std::vector< std::string > params;
    
    if (param_file->is_open()) {
        if (DEBUG) {
            std::cout << "Params received:" << std::endl;
        }
        /*
         * start_date
         * end_date
         * assets
         * pers if two assets mode
         * portofolio_size
         * */
        while (getline(*param_file, param)) {
            if (DEBUG) {
                std::cout << param << std::endl;
            }
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

int find_index(std::string asset_name, std::vector<std::string> all_assets) {
    for (int i = 0; i < all_assets.size(); ++i) {
        if (asset_name.compare(all_assets[i]) == 0) {
            return i;
        }
    }
    return -1;
}

PortReturn get_greatest_loss(std::map<std::string, std::vector<float> > data, std::vector<std::string> all_assets, std::string asset_combination, int portofolio_size, std::string percentages_string) {
    PortReturn worst_return(std::numeric_limits<float>::max());     
    worst_return.combination = asset_combination;
    worst_return.percentages = percentages_string;
    
    std::vector<std::string> combi = split(asset_combination.c_str(), ','); 
    std::vector<std::string> percentages = split(percentages_string.c_str(), ',');
    std::vector<int> index_assets;
    std::vector<float> pers;
    
    for (int i = 0; i < portofolio_size; ++i) {
        pers.push_back(::atof(percentages[i].c_str()));
        //std::cout << "pers" << i << ": " << pers[i] << std::endl;
        index_assets.push_back(find_index(combi[i], all_assets));
    }

    std::map<std::string, std::vector<float> >::iterator data_end = data.end();
    for (std::map<std::string, std::vector<float> >::iterator index_start_date = data.begin(); index_start_date != data_end; ++index_start_date) {
        //std::cout << "values start_date:" << std::endl;
        std::vector<float> values_asset_start_date;
        for (int i = 0; i < portofolio_size; ++i) {
            values_asset_start_date.push_back(index_start_date->second[index_assets[i]]);
            //std::cout << values_asset_start_date.back() << std::endl;
        }
        //std::cout << std::endl;
        
        std::map<std::string, std::vector<float> >::iterator index_start_date_copy = index_start_date;
        for (std::map<std::string, std::vector<float> >::iterator current_date = ++index_start_date_copy; current_date != data_end; ++current_date) {
            std::vector<float> returns_asset;
            //std::cout << "indiv returns:" << std::endl;
            for (int i = 0; i < portofolio_size; ++i) {
                //std::cout << current_date->second[index_assets[i]] << std::endl;
                //std::cout << values_asset_start_date[i] << std::endl;
                returns_asset.push_back((current_date->second[index_assets[i]] - values_asset_start_date[i]) / values_asset_start_date[i]);
                //std::cout << returns_asset.back() << std::endl;
            }
            //std::cout << std::endl;

            float total_current_return = 0.0;
            for (int i = 0; i < portofolio_size; ++i) {
                float return_asset = pers[i] * returns_asset[i];
                total_current_return += return_asset;
            }
            //std::cout << total_current_return << std::endl;

            if (total_current_return < worst_return.value) {
                //std::cout << "we have a worst return: " << total_current_return << std::endl;
                worst_return.value = total_current_return;
                worst_return.start_date  = index_start_date->first;
                worst_return.end_date = current_date->first;
            }
        }

    } 
    std::cout << "Worst return is " << worst_return.value;
    std::cout << "% if you start on " << worst_return.start_date;
    std::cout << " and stop on " << worst_return.end_date;
    std::cout << " with this combination of assets: " << worst_return.combination;
    std::cout << " and this percentages (respectively): " << worst_return.percentages;
    std::cout << std::endl;
    return worst_return;
}

std::vector<std::string> combinations;
std::vector<std::string> temp_combinations;

void join_combinations(const std::vector<std::string>& v) {
    std::string combi = "";
    for (int i = 0; i < v.size(); ++i) { 
        combi += v[i];
        if (i < v.size() - 1) {
            combi += ',';
        }
    }
    combinations.push_back(combi);
}

void make_combinations(const std::vector<std::string>& assets, int offset, int k) {
    if (k == 0) {
        join_combinations(temp_combinations);
        return;
    }

    for (int i = offset; i <= assets.size() - k; ++i) {
        temp_combinations.push_back(assets[i]);
        make_combinations(assets, i + 1, k - 1);
        temp_combinations.pop_back();
    }
}

void print_combinations() {
    for (int i = 0; i < combinations.size(); ++i) {
        std::cout << combinations[i] << std::endl;
    }
}

std::string get_portfolio_distribution(std::map<std::string, std::vector<float> > data, std::vector<std::string> all_assets, std::string combination, int portfolio_size) {
    std::string portfolio_distribution = "";
    PortReturn current_return;
    
    for (int perc1 = 0, perc2 = 100; perc1 <= 100; perc1 += 5, perc2 = (100 - perc1)) {
        std::string percentages = intToString(perc1) + "," + intToString(perc2);
        current_return = get_greatest_loss(data, all_assets, combination, portfolio_size, percentages);
        portfolio_distribution += floatToString(0 - current_return.value);
        if (perc1 < 100) {
            portfolio_distribution += ",";
        }
    }

    return portfolio_distribution;
}

PortReturn get_right_combination(std::vector< std::string > params, std::map <std::string, std::vector<float> > data) {
    std::vector<std::string> all_assets = split(params[2].c_str(), ',');
    std::string percentages = "";

    // default value for portofolio size
    int portfolio_size = 2;
    std::stringstream portfolio_stream(params[4]); 
    portfolio_stream >> portfolio_size;
   
    if (!portfolio_stream) {
        std::cout << "Impossible to get value for portfolio_size, default value: " + intToString(portfolio_size) << std::endl;
    }
    make_combinations(all_assets, 0, portfolio_size);
    if (DEBUG) {
        print_combinations();
    }

    int combi_size = combinations.size();
    if (params[3].compare("void") != 0)  { 
        percentages = params[3];
    }
    else {
        float per_each = 100.0 / (float) portfolio_size;
        for (int i = 0; i < portfolio_size; ++i) {
            percentages += floatToString(per_each);
            if (i != portfolio_size - 1) {
                percentages += ",";
            }
        }
    }

    std::cout << percentages << std::endl;
    PortReturn best_return(-std::numeric_limits<float>::max());
    for (int i = 0; i < combi_size; ++i) {
        //std::cout << "combi no " << i << ": " << combinations[i] << std::endl; 
        PortReturn current_return = get_greatest_loss(data, all_assets, combinations[i], portfolio_size, percentages);
        if (current_return.value > best_return.value) {
            best_return = current_return;
        }
    }
    
    if (combi_size == 1) {
        std::cout << "two_assets mode, let's build that chart!" << std::endl; 
        best_return.portfolio_distribution = get_portfolio_distribution(data, all_assets, combinations[0], portfolio_size);
        std::cout << "Portfolio distribution is: " << best_return.portfolio_distribution << std::endl;
    }

    std::cout << "Best return is: " << best_return.value;
    std::cout << "% if you start on " << best_return.start_date;
    std::cout << " and stop on " << best_return.end_date;
    std::cout << " with this combination of assets: " << best_return.combination;
    std::cout << " and this percentages (respectively): " << best_return.percentages;
    std::cout << std::endl;
    return best_return;
}

std::string write_results(PortReturn port_return, std::string original_filename) {
    std::string result_filepath = original_filename.substr(0, original_filename.size() - 4) + "_results" + ".txt";
    std::ofstream output_file(result_filepath.c_str());
    output_file << port_return.value << "\n";
    output_file << port_return.start_date << "\n";
    output_file << port_return.end_date << "\n";
    output_file << port_return.combination << "\n";
    output_file << port_return.percentages << "\n";
    output_file << port_return.portfolio_distribution; 
    output_file.close();
    return result_filepath;
} 

void send_file_back(std::string result_filepath) {
    std::string scp = "scp " + result_filepath + " guillet@hipercic-devel.public.stolaf.edu:/home/guillet/hipercic/apps/dcor/jobs";
    system(scp.c_str());
    if (DEBUG) {
        std::cout << "File " << result_filepath << " sent!" << std::endl; 
    }
}
