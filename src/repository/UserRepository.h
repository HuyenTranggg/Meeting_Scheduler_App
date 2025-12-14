#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include "../../data/Database.h"
#include "../models/User.h"
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <vector>

using namespace std;

class UserRepository {
  private:
    Database db;

  public:
    UserRepository() {}

    User getUserByUsername(const string &username) {
        User user;

        if (db.connect()) {
            string query = "SELECT * FROM users WHERE username = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setString(1, username);
                sql::ResultSet *res = pstmt->executeQuery();

                if (res->next()) {
                    user.setId(res->getInt("id"));
                    user.setUsername(res->getString("username"));
                    user.setPassword(res->getString("password"));
                    user.setRole(res->getString("role"));
                    user.setFirstName(res->getString("first_name"));
                    user.setLastName(res->getString("last_name"));
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Error getting user: " << e.what() << endl;
            }
        } else {
            cout << "Error accessing the database." << endl;
        }

        return user;
    }

    User getUserById(const int &id) {
        User user;

        if (db.connect()) {
            string query = "SELECT * FROM users WHERE id = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, id);
                sql::ResultSet *res = pstmt->executeQuery();

                if (res->next()) {
                    user.setId(res->getInt("id"));
                    user.setUsername(res->getString("username"));
                    user.setPassword(res->getString("password"));
                    user.setRole(res->getString("role"));
                    user.setFirstName(res->getString("first_name"));
                    user.setLastName(res->getString("last_name"));
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Error getting user: " << e.what() << endl;
            }
        } else {
            cout << "Error accessing the database." << endl;
        }
        return user;
    }

    bool checkUsernameExists(const string &username) {
        if (db.connect()) {
            string query = "SELECT COUNT(*) FROM users WHERE username = ?";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setString(1, username);

                sql::ResultSet *res = pstmt->executeQuery();
                if (res->next() && res->getInt(1) > 0) {
                    delete res;
                    delete pstmt;
                    return true;
                }

                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Error checking username: " << e.what() << endl;
            }
        } else {
            cout << "Error accessing the database." << endl;
        }

        return false; // Username does not exist
    }

    void create(const User &user) {
        if (db.connect()) {
            if (checkUsernameExists(user.getUsername())) {
                cout << "Username already exists!" << endl;
                return;
            }
            string query = "INSERT INTO users (username, password, role, first_name, last_name) VALUES (?, ?, ?, ?, ?)";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setString(1, user.getUsername());
                pstmt->setString(2, user.getPassword());
                pstmt->setString(3, user.getRole());
                pstmt->setString(4, user.getFirstName());
                pstmt->setString(5, user.getLastName());

                pstmt->executeUpdate();
                cout << "User: " << user.getUsername() << " has been created successfully" << endl;
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Error adding user: " << e.what() << endl;
            }
        } else {
            cout << "Error accessing the database." << endl;
        }
    }
    vector<User> getAllTeachers() {
        vector<User> teachers;

        if (db.connect()) {
            string query = "SELECT * FROM users WHERE role = 'teacher'";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                sql::ResultSet *res = pstmt->executeQuery();

                while (res->next()) {
                    User user;
                    user.setId(res->getInt("id"));
                    user.setUsername(res->getString("username"));
                    user.setPassword(res->getString("password"));
                    user.setRole(res->getString("role"));
                    user.setFirstName(res->getString("first_name"));
                    user.setLastName(res->getString("last_name"));
                    teachers.push_back(user);
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                cerr << "Lỗi lấy dữ liệu user: " << e.what() << endl;
            }
        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        return teachers;
    }    

    User getTeacherFromMeeting(const int &meeting_id) {
        User teacher;

        if (db.connect()) {
            string query = R"(
                SELECT users.*
                FROM users
                INNER JOIN timeslots ON timeslots.teacher_id = users.id
                INNER JOIN meetings ON meetings.timeslot_id = timeslots.id
                WHERE meetings.id = ? 
            )";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, meeting_id);
                sql::ResultSet *res = pstmt->executeQuery();

                if (res->next()) {
                    teacher.setId(res->getInt("id"));
                    teacher.setUsername(res->getString("username"));
                    teacher.setPassword(res->getString("password"));
                    teacher.setRole(res->getString("role"));
                    teacher.setFirstName(res->getString("first_name"));
                    teacher.setLastName(res->getString("last_name"));
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ meetings: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        return teacher;
    }
    
    vector<User> getStudentsInHistory(const int &teacher_id) {
        vector<User> students;

        if (db.connect()) {
            string query = R"(
                SELECT DISTINCT users.*                
                FROM users
                INNER JOIN attendances ON attendances.student_id = users.id
                INNER JOIN meetings ON meetings.id = attendances.meeting_id
                INNER JOIN timeslots ON timeslots.id = meetings.timeslot_id
                WHERE timeslots.teacher_id = ? 
                AND meetings.status = 'completed'
            )";
            try {
                sql::PreparedStatement *pstmt = db.getConnection()->prepareStatement(query);
                pstmt->setInt(1, teacher_id);
                sql::ResultSet *res = pstmt->executeQuery();

                while (res->next()) {
                    User student;
                    student.setId(res->getInt("id"));
                    student.setUsername(res->getString("username"));
                    student.setPassword(res->getString("password"));
                    student.setRole(res->getString("role"));
                    student.setFirstName(res->getString("first_name"));
                    student.setLastName(res->getString("last_name"));
                    students.push_back(student);
                }
                delete res;
                delete pstmt;
            } catch (sql::SQLException &e) {
                std::cerr << "Lỗi khi lấy dữ liệu từ meetings: " << e.what() << std::endl;
            }

        } else {
            cout << "Lỗi không thể truy cập cơ sở dữ liệu." << endl;
        }

        return students;
    }    
};

#endif
