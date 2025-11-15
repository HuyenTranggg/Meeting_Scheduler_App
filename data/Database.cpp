#include "Database.h"
#include <cppconn/exception.h>
#include <iostream>

using namespace std;

Database::~Database() {
    if (con) {
        delete con;
    }
}

bool Database::connect() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(host, user, password);
        con->setSchema(database);
        cout << "Connected to database successfully!" << endl;
        return true;
    } catch (sql::SQLException& e) {
        cerr << "Connection error: " << e.what() << endl;
        return false;
    }
}

void Database::executeQuery(const string& query) {
    try {
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery(query);
        while (res->next()) {
            cout << "Data: " << res->getString(1) << endl;
        }
        delete res;
        delete stmt;
    } catch (sql::SQLException& e) {
        cerr << "Query error: " << e.what() << endl;
    }
}


// int main() {
//     Database db("root", "your_password", "ltm");

//     if (db.connect()) {
//         // db.executeQuery("SELECT * FROM your_table");
//         cout << "connected";
//     }
//     return 0;
// }