#include "database.h"
#include "csv.h"
#include <iostream>
#include <fstream>
using namespace std;

static const std::string TABLE_NAME = "dcor_record";

void close_conne(PGconn * conn) {
    PQfinish(conn);
    getchar();
}

PGconn* connect_db() {
    PGconn *conn = NULL;
    conn = PQconnectdb("user=guillet password=hipercic dbname=hpcc_s13 host=shelob2.cs.stolaf.edu port=5432");

    if (PQstatus(conn) != CONNECTION_OK) {
        cout << "Connection to database failed.\n";
        close_conne(conn);
    }
    cout << "Connection to database - OK\n";

    return conn;
}

void create_params_table(PGconn *conn) {
    // not actual query, must be updated...
    PGresult *res = PQexec(conn, "CREATE TABLE params (id PRIMARY KEY AUTO INCREMENT NOT NULL, asset_id integer NOT NULL, date date NOT NULL, closed_price integer NOT NULL");
  
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cout << "Create params table failed\n";
        PQclear(res);
        close_conne(conn);
    }
    cout << "Create user table - OK\n";
    // Clear result
    PQclear(res);
}

void insert_asset_record(PGconn *conn, string asset_id, string date, string closed_price) {
    // Append the SQL statment
    string sSQL;
    sSQL.append("INSERT INTO dcor_record (asset_id, date, closed_price) VALUES (");
    sSQL.append(asset_id);
    sSQL.append(", '");
    sSQL.append(date);
    sSQL.append("', ");
    sSQL.append(closed_price);
    sSQL.append(");");

    //cout << sSQL << endl;
    //Execute with sql statement
    PGresult *res = PQexec(conn, sSQL.c_str());
            
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cout << "Insert asset record failed\n";
        PQclear(res);
        close_conne(conn);
    }
    cout << "Insert asset record - OK\n";
    // Clear result
    PQclear(res);
}

void fill_tables(PGconn *conn) {
    std::ifstream file("data/three_assets.csv");

    string line;
    string headers;
    getline(file, headers);
    while (getline(file, line)) {
        vector<string> str = split(line, ',');
        insert_asset_record(conn, "4", str[0], str[1]);
        insert_asset_record(conn, "5", str[0], str[2]);
        insert_asset_record(conn, "6", str[0], str[3]);
    }
}

vector< vector<string> > fetch_data(PGconn *conn, vector<string> params) {
    vector< vector<string> > data;
    string sql = "DECLARE emprec CURSOR FOR SELECT r.date, a.name, r.closed_price FROM "+ TABLE_NAME + " r INNER JOIN dcor_asset a ON r.asset_id = a.id WHERE r.date BETWEEN '" + params[6]  + "' AND '" + params[1] + "' AND a.name IN ('" + params[2] + "', '" + params[0] + "');";
    
    cout << sql << endl; 
    int n_fields;
    
    // Start a transaction block
    PGresult *res  = PQexec(conn, "BEGIN");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cout << "BEGIN command failed\n";
        cout << PQerrorMessage(conn);
        PQclear(res);
        close_conne(conn);
    }

    // Clear result
    PQclear(res);

    // Fetch rows from asset record table
    res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cout << "DECLARE CURSOR failed\n";
        cout << PQerrorMessage(conn);
        PQclear(res);
        close_conne(conn);
    }

    // Clear result
    PQclear(res);

    res = PQexec(conn, "FETCH ALL in emprec");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        cout << "FETCH ALL failed\n";
        cout << PQerrorMessage(conn);
        PQclear(res);
        close_conne(conn);
    }

    n_fields = PQnfields(res);

    cout << "\nFetch record:";
    cout << "\n********************************************************************\n";
    for (int i = 0; i < n_fields; ++i) {
        printf("%-30s", PQfname(res, i));
        vector<string> temp_vector;
        data.push_back(temp_vector);
    }
    cout << "\n********************************************************************\n";

    // Next, print out the asset price record for each row
    string first_occur_date;
    for (int i = 0; i < PQntuples(res); ++i) {
        first_occur_date = PQgetvalue(res, i, 0);
        if ( (i != 0)  && (first_occur_date.compare(PQgetvalue(res, i - 1, 0)) == 0) ) { 
            //cout << "row " << i << " was skipped" << endl;
            continue;
        }
        else {
            data[0].push_back(PQgetvalue(res, i, 0));
            data[1].push_back(PQgetvalue(res, i, 2));
            data[2].push_back(PQgetvalue(res, i+1, 2));
            //cout << data[0].back() << "|" << data[1].back() << "|" << data[2].back() << endl;
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

