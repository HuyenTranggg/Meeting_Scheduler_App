#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#include "../models/Response.h"
#include "../models/User.h"
#include "../repository/UserRepository.h"
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class UserController {
  private:
    UserRepository userRepository;

  public:
    UserController() {}

    vector<string> splitString(const string &str, char delimiter) {
        vector<string> tokens;
        string token;
        stringstream ss(str);

        // Sử dụng getline để tách chuỗi
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    Response login(const string &username, const string &password) {
        Response res;
        User user = userRepository.getUserByUsername(username);
        if (user.getId() == 0) {
            res.setStatus(1);
            res.setMessage("Invalid username|");
        } else if (user.getPassword() != password) {
            res.setStatus(2);
            res.setMessage("Incorrect password|");
        } else {
            res.setStatus(0);
            string message = "Successfully login|" + to_string(user.getId()) + "|" + user.getRole() + "|";
            res.setMessage(message);
        }

        return res;
    }
// Thành công: 0 | Successfully login | user_id | role | <END> 
//        (role: student/teacher)
// Không thành công - Tên đăng nhập sai: 1 | Invalid username | <END>
// Không thành công - mật khẩu sai: 2 | Incorrect password | <END> 
    Response registerAccount(const string &message) {
        vector<string> tokens = splitString(message, '|');
        Response res;
        string username = tokens[1];
        string password = tokens[2];
        string role = tokens[3];
        string first_name = tokens[4];
        string last_name = tokens[5];
        User user = userRepository.getUserByUsername(username);
        if (user.getId() != 0) {
            res.setStatus(3);
            res.setMessage("Username already exists|");
        } else {
            User newUser(username, password, role, first_name, last_name);
            userRepository.create(newUser);
            User user = userRepository.getUserByUsername(username);
            string message = "Successfully registered|" + to_string(user.getId()) + "|" + user.getRole() + "|" + user.getFirstName() + "|" + user.getLastName() + "|";
            res.setStatus(0);
            res.setMessage(message);
        }
        return res;
    }
};
//        0 | Successfully registered | user_id | role | first_name | last_name | <END>
// Không thành công - tên đăng nhập đã tồn tại: 
//        3 | Username already exists | <END>
#endif
