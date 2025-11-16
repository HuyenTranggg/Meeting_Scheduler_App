#ifndef RESPONSE_H
#define RESPONSE_H
#include <iostream>
using namespace std;

class Response {
    int status;
    string message;

  public:
    // Constructor
    Response() {};
    Response(const int &nstatus, const string &nmessage) : status(nstatus), message(nmessage) {}

    // Getters
    int getStatus() const { return status; }
    string getMessage() const { return message; }

    // Setters
    void setStatus(const int &newStatus) { status = newStatus; }
    void setMessage(const string &newMessage) { message = newMessage + "<END>"; }

    void show() { cout << "Response, Status: " << status << " ,message: " << message << endl; }
};

#endif
