/* author: Benjamin Guillet <benjamin.guillet@gmail.com>
Lost in the code? Any questions? Shout me an email. */

#include "database.h"
#include "utils.h"
#include <iostream>
#include <fstream>

static const std::string TABLE_NAME = "dcor_record";

PGconn* connect_db() {
    PGconn *conn = NULL;
    // Read from env. variables
    //conn = PQconnectdb("");
    // Have to hard code value because django can't read env. variables...
    conn = PQconnectdb("user=guillet password=hipercic dbname=hpcc_s13 host=shelob2.cs.stolaf.edu port=5432");//
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cout << "Connection to database failed.\n";
        close_conne(conn);
    }
    if (DEBUG) {
        std::cout << "Connection to database - OK\n";
    }
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

void insert_asset(PGconn *conn, std::string asset_name) {
    std::string sSQL;
    sSQL.append("INSERT INTO dcor_asset (name) VALUES (");
    sSQL.append("'" + asset_name + "'");
    sSQL.append(");");
    std::cout << sSQL << std::endl;

    //Execute with sql statement
    PGresult *res = PQexec(conn, sSQL.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Insert new asset failed" << std::endl;
        std::cout << PQerrorMessage(conn) << std::endl; 
    }
    else {
        std::cout << "Insert new asset -  OK\n";
    }
    PQclear(res);
}


void insert_asset_record(PGconn *conn, std::string asset_name, std::string date, std::string closed_price) {
    std::string sSQL;
    sSQL.append("INSERT INTO " + TABLE_NAME  + " (asset_id, date, closed_price) SELECT a.id, "); 
    sSQL.append("'" + date + "'");
    sSQL.append(", ");
    sSQL.append(closed_price);    
    sSQL.append(" FROM dcor_asset a WHERE a.name = '" + asset_name + "' LIMIT 1;");
    std::cout << sSQL << std::endl;

    //Execute with sql statement
    PGresult *res = PQexec(conn, sSQL.c_str());
            
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "Insert asset record failed\n";
        std::cout << PQerrorMessage(conn) << std::endl; 
    }
    else {
        std::cout << "Insert asset record - OK\n";
    }
    // Clear result
    PQclear(res);
}

void fill_tables(PGconn *conn, char* file_path) {
    std::ifstream file(file_path);

    std::string line;
    std::string headers_line;
    getline(file, headers_line);
    std::vector<std::string> headers = split(headers_line, ',');
    int n_headers = headers.size();
    for (int i = 1; i < n_headers; ++i) {
        insert_asset(conn, headers[i]);
    }
    while (getline(file, line)) {
        std::vector<std::string> records = split(line, ',');
        for (int i = 1; i < records.size(); ++i) {
            insert_asset_record(conn, headers[i], records[0], records[i]);
        }
    }
}

std::map <std::string, std::vector<float> > fetch_data(PGconn *conn, std::vector<std::string> params) {
    std::map <std::string, std::vector<float> > data;
    std::vector<std::string> assets = split(params[2].c_str(), ',');
    int nb_assets = assets.size();
    
    std::string sql = "DECLARE emprec CURSOR FOR SELECT r.date, a.name, r.closed_price FROM "+ TABLE_NAME + " r INNER JOIN dcor_asset a ON r.asset_id = a.id WHERE r.date BETWEEN '" + params[0]  + "' AND '" + params[1] + "' AND a.name IN ("; 

    for (int i = 0; i < nb_assets; ++i) {
        sql += "'" + assets[i] + "'";
        if (i != assets.size() - 1) {
            sql += ",";
        }
    }
    sql += ") ORDER BY r.date, CASE a.name";
    for (int i = 0; i < nb_assets; ++i) {
        sql += " when '" + assets[i] + "' then " + intToString(i + 1);
    }
    sql += " end;"; 
    
    if (DEBUG) {
        std::cout << sql << std::endl; 
    }
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

    for (int i = 0; i < PQntuples(res); ++i) {
        //std::cout << PQgetvalue(res, i, 0) << "=>" << PQgetvalue(res, i, 1) << "|" << PQgetvalue(res, i, 2) << std::endl; 
        std::string current_row_date = PQgetvalue(res, i, 0); 
        data.insert( std::pair<std::string, std::vector<float> >(current_row_date, std::vector<float>()));
        data[current_row_date].push_back(::atof(PQgetvalue(res, i, 2)));
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

