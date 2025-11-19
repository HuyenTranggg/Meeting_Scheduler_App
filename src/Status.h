#ifndef STATUS_H
#define STATUS_H

enum class Status {
    SUCCESS = 0,
    USER_NOT_FOUND = 1,
    WRONG_PASSWORD = 2,
    USERNAME_EXISTS = 3,
    UNKNOWN_ERROR = 99,
    INVALID_TIME = 6,   
    TIME_CONFLICT = 14
};

#endif