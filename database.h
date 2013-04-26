#ifndef DATABASE_H
#define DATABABSE_H

#include <string>
#include <vector>
#include "postgresql/libpq-fe.h"

PGconn* connect_db();
void close_conne(PGconn *conn);
void create_params_table(PGconn *conn);
void insert_asset_record(PGconn *conn, std::string asset_id, std::string date, std::string closed_price);
void fill_tables(PGconn *conn);
std::vector< std::vector< std::string> > fetch_data(PGconn *conn, std::vector< std::string > params);

#endif
