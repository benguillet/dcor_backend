#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include "postgresql/libpq-fe.h"

PGconn* connect_db();
void close_conne(PGconn *conn);
void create_params_table(PGconn *conn);
void insert_asset_record(PGconn *conn, std::string asset_name, std::string date, std::string closed_price);
void insert_asset(PGconn *conn, std::string asset_name);
void fill_tables(PGconn *conn, char* file_path);
std::map <std::string, std::vector<float> > fetch_data(PGconn *conn, std::vector<std::string> params);

#endif
