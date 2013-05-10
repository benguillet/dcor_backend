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

int find_index(std::string asset_name, std::vector<std::string> all_assets) {
    for (int i = 0; i < all_assets.size(); ++i) {
        if (asset_name.compare(all_assets[i]) == 0) {
            return i;
        }
    }
    return -1;
}

float get_greatest_loss(std::vector< std::string > params, std::map<std::string, std::vector<float> > data, std::string asset_combination) {
    std::vector<std::string> combi = split(asset_combination.c_str(), ',');
    std::vector<std::string> pers_string;
    std::vector<std::string> all_assets = split(params[2].c_str(), ',');
    std::vector<float> pers;
    std::vector<int> index_assets;
    int combi_size = combi.size();

    if (params[3].compare("void") != 0)  { 
        pers_string = split(params[3].c_str(), ',');
        for (int i = 0; i < combi_size; ++i) {
            pers.push_back(::atof(pers_string[i].c_str()));
            std::cout << "pers" << i << ": " << pers[i] << std::endl;
            index_assets.push_back(find_index(combi[i], all_assets));
        }
    }
    else {
        for (int i = 0; i < combi_size; ++i) {
           pers.push_back(100.0 / (float) combi_size);
           index_assets.push_back(find_index(combi[i], all_assets));
           //std::cout << pers.back() << std::endl;
           //std::cout << index_assets.back() << std::endl;
        }
    }


    float worst_return = std::numeric_limits<float>::max();
    std::string worst_start_date = "";
    std::string worst_end_date   = "";
     
    std::map<std::string, std::vector<float> >::iterator data_end = data.end();
    
    for (std::map<std::string, std::vector<float> >::iterator index_start_date = data.begin(); index_start_date != data_end; ++index_start_date) {
        //std::cout << "values start_date:" << std::endl;
        std::vector<float> values_asset_start_date;
        for (int i = 0; i < combi_size; ++i) {
            values_asset_start_date.push_back(index_start_date->second[index_assets[i]]);
            //std::cout << values_asset_start_date.back() << std::endl;
        }
        //std::cout << std::endl;
        
        std::map<std::string, std::vector<float> >::iterator index_start_date_copy = index_start_date;
        for (std::map<std::string, std::vector<float> >::iterator current_date = ++index_start_date_copy; current_date != data_end; ++current_date) {
            std::vector<float> returns_asset;
            //std::cout << "indiv returns:" << std::endl;
            for (int i = 0; i < combi_size; ++i) {
                //std::cout << current_date->second[index_assets[i]] << std::endl;
                //std::cout << values_asset_start_date[i] << std::endl;
                returns_asset.push_back((current_date->second[index_assets[i]] - values_asset_start_date[i]) / values_asset_start_date[i]);
                //std::cout << returns_asset.back() << std::endl;
            }
            //std::cout << std::endl;

            float total_current_return = 0.0;
            for (int i = 0; i < combi_size; ++i) {
                float return_asset = pers[i] * returns_asset[i];
                total_current_return += return_asset;
            }
            //std::cout << total_current_return << std::endl;

            if (total_current_return < worst_return) {
                std::cout << "we have a worst return: " << total_current_return << std::endl;
                worst_return = total_current_return;
                worst_start_date = index_start_date->first;
                worst_end_date = current_date->first;
            }
        }

    } 
    std::cout << "Worst return is " << worst_return;
    std::cout << "% if you start on " << worst_start_date;
    std::cout << " and stop on " << worst_end_date;
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

void get_right_combination(std::vector< std::string > params, std::map <std::string, std::vector<float> > data) {
    std::vector<std::string> assets = split(params[2].c_str(), ',');
    
    // default value for portofolio size
    int portofolio_size = 2;

    std::stringstream portofolio_stream(params[4]); 
    portofolio_stream >> portofolio_size;
   
    if (!portofolio_stream) {
        std::cout << "Impossible to get value for portofolio_size, default value: " + intToString(portofolio_size) << std::endl;
    }
    make_combinations(assets, 0, portofolio_size);
    print_combinations();
    float current_return = 0.0;
    float best_return = -std::numeric_limits<float>::max();

    for (int i = 0; i < combinations.size(); ++i) {
        std::cout << "combi no " << i << ": " << combinations[i] << std::endl; 
        current_return = get_greatest_loss(params, data, combinations[i]);
        if (current_return > best_return) {
            best_return = current_return;
        }
    }

    std::cout << "Best return is: " << best_return;
    std::cout << "% if you start on ";
    std::cout << " and stop on ";
    std::cout << " with this combination of assets: ";
    std::cout << " and this percentages (respectively): " << std::endl;
} 

