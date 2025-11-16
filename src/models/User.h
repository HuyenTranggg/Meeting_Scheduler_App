#ifndef USER_H
#define USER_H
#include <iostream>
using namespace std;

class User {
  protected:
    int id = 0;
    string username;
    string password;
    string role;
    string first_name;
    string last_name;

  public:
    // Constructor
    User() {};
    User(const string &uname, const string &pass, const string &role, const string &fname, const string &lname)
        : username(uname), password(pass), role(role), first_name(fname), last_name(lname) {}

    // Getters
    int getId() const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getRole() const { return role; }
    string getFirstName() const { return first_name; }
    string getLastName() const { return last_name; }

    // Setters
    void setId(const int &newId) { id = newId; }
    void setUsername(const string &uname) { username = uname; }
    void setPassword(const string &pass) { password = pass; }
    void setRole(const string &r) { role = r; }
    void setFirstName(const string &fname) { first_name = fname; }
    void setLastName(const string &lname) { last_name = lname; }

    string toStringProfile() {
      return to_string(id) + "|" + first_name + "|" + last_name;
    }

    void show() {
        cout << "User: " << username << " ,pass: " << password << " ,role: " << role << " , ho: " << first_name
             << ", ten: " << last_name << endl;
    }
};

#endif
