#ifndef DATABASE_H
#define DATABASE_H

#include "../environment.h"
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <memory>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <string>

using namespace std;

class Database {
  private:
    Environment env;
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    string host = env.getHost();
    string user = env.getUser();
    string password = env.getPassword();
    string database = "ltm";

  public:
    Database() : driver(nullptr), con(nullptr) {}

    ~Database() {
        disconnect(); // Đảm bảo ngắt kết nối khi đối tượng bị hủy
    }

    sql::Connection *getConnection() {
        if (!con) {
            // Nếu chưa có kết nối, thực hiện kết nối mới
            connect();
        }
        return con;
    }

    bool connect() {
        if (con) {
            return true; // Nếu đã có kết nối thì không làm gì thêm
        }

        try {
            if (!driver) {
                driver = sql::mysql::get_mysql_driver_instance();
            }
            con = driver->connect(host, user, password);
            con->setSchema(database);
            return true;
        } catch (sql::SQLException &e) {
            cerr << "Connection error: " << e.what() << endl;
            return false;
        }
    }

    void disconnect() {
        if (con) {
            try {
                delete con;    // Giải phóng tài nguyên
                con = nullptr; // Đảm bảo con không trỏ đến địa chỉ cũ
            } catch (sql::SQLException &e) {
                cerr << "Disconnection error: " << e.what() << endl;
            }
        }
    }

    void executeQuery(const string &query) {
        sql::Connection *connection = getConnection();
        if (!connection) {
            cerr << "No database connection." << endl;
            return;
        }

        try {
            sql::Statement *stmt = connection->createStatement();
            sql::ResultSet *res = stmt->executeQuery(query);
            while (res->next()) {
                cout << "Data: " << res->getString(1) << endl;
            }
            delete res;
            delete stmt;
        } catch (sql::SQLException &e) {
            cerr << "Query error: " << e.what() << endl;
        }
    }

    // Thực thi truy vấn UPDATE, INSERT, DELETE
    void executeUpdate(const string &query) {
        sql::Connection *connection = getConnection();
        if (!connection) {
            cerr << "No database connection." << endl;
            return;
        }

        try {
            sql::Statement *stmt = connection->createStatement();
            int affectedRows = stmt->executeUpdate(query);
            cout << "Query executed, " << affectedRows << " rows affected." << endl;
            delete stmt;
        } catch (sql::SQLException &e) {
            cerr << "Update error: " << e.what() << endl;
        }
    }
};

#endif // DATABASE_H
