#include "database.h"
#include "csv.h"
#include <iostream>
#include <fstream>

static const std::string TABLE_NAME = "dcor_record";

PGconn* connect_db() {
    PGconn *conn = NULL;
    // Read from env. variables
    conn = PQconnectdb("");

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed.\n";
        close_conne(conn);
    }
    std::cout << "Connection to database - OK\n";

    return conn;
}

void close_conne(PGconn * conn) {
    PQfinish(conn);
    getchar();
}

void create_params_table(PGconn *conn) {
    //TODO: not actual query, must be updated...
    PGresult *res = PQexec(conn, "CREATE TABLE params (id PRIMARY KEY AUTO INCREMENT NOT NULL, asset_id integer NOT NULL, date date NOT NULL, closed_price integer NOT NULL");
  
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Create params table failed\n";
        PQclear(res);
        close_conne(conn);
    }
    std::cout << "Create user table - OK\n";
    // Clear result
    PQclear(res);
}

void insert_asset_record(PGconn *conn, std::string asset_id, std::string date, std::string closed_price) {
    // Append the SQL statment
    std::string sSQL;
    sSQL.append("INSERT INTO " + TABLE_NAME  + " (asset_id, date, closed_price) VALUES (");
    sSQL.append(asset_id);
    sSQL.append(", '");
    sSQL.append(date);
    sSQL.append("', ");
    sSQL.append(closed_price);
    sSQL.append(");");

    //std::cout << sSQL << std::endl;
    //Execute with sql statement
    PGresult *res = PQexec(conn, sSQL.c_str());
            
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Insert asset record failed\n";
        PQclear(res);
        close_conne(conn);
    }
    std::cout << "Insert asset record - OK\n";
    // Clear result
    PQclear(res);
}

void fill_tables(PGconn *conn) {
    std::ifstream file("data/three_assets.csv");

    std::string line;
    std::string headers;
    getline(file, headers);
    while (getline(file, line)) {
        std::vector<std::string> str = split(line, ',');
        insert_asset_record(conn, "4", str[0], str[1]);
        insert_asset_record(conn, "5", str[0], str[2]);
        insert_asset_record(conn, "6", str[0], str[3]);
    }
}

std::vector< std::vector<std::string> > fetch_data(PGconn *conn, std::vector<std::string> params) {
    std::vector< std::vector<std::string> > data;
    std::string sql = "DECLARE emprec CURSOR FOR SELECT r.date, a.name, r.closed_price FROM "+ TABLE_NAME + " r INNER JOIN dcor_asset a ON r.asset_id = a.id WHERE r.date BETWEEN '" + params[6]  + "' AND '" + params[1] + "' AND a.name IN ('" + params[2] + "', '" + params[0] + "');";
    
    std::cout << sql << std::endl; 
    int n_fields;
    
    // Start a transaction block
    PGresult *res  = PQexec(conn, "BEGIN");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "BEGIN command failed\n";
        std::cout << PQerrorMessage(conn);
        PQclear(res);
        close_conne(conn);
    }

    // Clear result
    PQclear(res);

    // Fetch rows from asset record table
    res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "DECLARE CURSOR failed\n";
        std::cout << PQerrorMessage(conn);
        PQclear(res);
        close_conne(conn);
    }

    // Clear result
    PQclear(res);

    res = PQexec(conn, "FETCH ALL in emprec");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cout << "FETCH ALL failed\n";
        std::cout << PQerrorMessage(conn);
        PQclear(res);
        close_conne(conn);
    }

    n_fields = PQnfields(res);

    //std::cout << "\nFetch record:";
    //std::cout << "\n********************************************************************\n";
    for (int i = 0; i < n_fields; ++i) {
        //printf("%-30s", PQfname(res, i));
        std::vector<std::string> temp_vector;
        data.push_back(temp_vector);
    }
    //std::cout << "\n********************************************************************\n";

    // Next, print out the asset price record for each row
    std::string first_occur_date;
    for (int i = 0; i < PQntuples(res); ++i) {
        first_occur_date = PQgetvalue(res, i, 0);
        if ( (i != 0)  && (first_occur_date.compare(PQgetvalue(res, i - 1, 0)) == 0) ) { 
            //std::cout << "row " << i << " was skipped" << std::endl;
            continue;
        }
        else {
            data[0].push_back(PQgetvalue(res, i, 0));
            data[1].push_back(PQgetvalue(res, i, 2));
            data[2].push_back(PQgetvalue(res, i+1, 2));
            //std::cout << data[0].back() << "|" << data[1].back() << "|" << data[2].back() << std::endl;
        }
    }

    PQclear(res);

    // Close the emprec
    res = PQexec(conn, "CLOSE emprec");
    PQclear(res);

    // End the transaction
    res = PQexec(conn, "END");

    // Clear result
    PQclear(res);

    return data;
}

