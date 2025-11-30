#ifndef STATUS_H
#define STATUS_H

enum class Status {
    SUCCESS = 0,

    USER_NOT_FOUND = 1,
    WRONG_PASSWORD = 2,
    USERNAME_EXISTS = 3,

    INVALID_TIME = 6,  
    NO_MEETING_FOUND = 8,
    TEACHER_NO_AVAILABLE_TIME = 4,

    TIME_SLOT_NOT_FOUND = 7,
    TIME_SLOT_CONFLICTS = 14,   // Time slot conflicts with another slot

    UNKNOWN_ERROR = 99
};

#endif