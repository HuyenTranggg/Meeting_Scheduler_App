#ifndef MESSAGEUTILS_H
#define MESSAGEUTILS_H

#include "../Status.h"
#include <string>

class MessageUtils {
  public:
    static std::string createMessage(Status status, const std::string &content) {
        return std::to_string(static_cast<int>(status)) + "|" + content;
    }

    static Status getStatusFromMessage(const std::string &message) {
        int statusCode = std::stoi(message.substr(0, message.find('|')));
        return static_cast<Status>(statusCode);
    }

    static std::string getContentFromMessage(const std::string &message) {
        return message.substr(message.find('|') + 1);
    }
};

#endif