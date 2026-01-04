#ifndef UTILS_H
#define UTILS_H
#include <cctype>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Utils {

  public:
    // Constructor
    Utils() {};

    vector<int> getCurrentDateTimeVector() {
        time_t t = time(nullptr);
        tm *now = localtime(&t);
        vector<int> dateTime = {now->tm_year + 1900, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min};
        return dateTime;
    }

    vector<int> getDateVector(const string &stringDate) {
        vector<int> dateVector;
        istringstream ss(stringDate);
        string token;

        while (getline(ss, token, '-')) {
            dateVector.push_back(stoi(token));
        }
        return dateVector;
    }

    vector<int> getTimeVector(const string &stringTime) {
        vector<int> timeVector;
        istringstream ss(stringTime);
        string token;

        while (getline(ss, token, ':')) {
            timeVector.push_back(stoi(token));
        }
        return timeVector;
    }

    bool checkHour(const string &hour) {
        if (hour.length() != 2)
            return false;

        for (char c : hour) {
            if (!isdigit(c))
                return false;
        }

        int h = stoi(hour);
        return h >= 0 && h <= 23;
    }

    bool checkMinute(const string &minute) {
        if (minute.length() != 2)
            return false;

        for (char c : minute) {
            if (!isdigit(c))
                return false;
        }

        int h = stoi(minute);
        return h >= 0 && h <= 59;
    }

    bool checkDay(const string &day) {
        if (day.length() != 2)
            return false;

        for (char c : day) {
            if (!isdigit(c))
                return false;
        }

        int h = stoi(day);
        return h >= 1 && h <= 31;
    }

    bool checkMonth(const string &month) {
        if (month.length() != 2)
            return false;

        for (char c : month) {
            if (!isdigit(c))
                return false;
        }

        int h = stoi(month);
        return h >= 1 && h <= 12;
    }

    bool checkYear(const string &year) {
        if (year.length() != 4)
            return false;

        for (char c : year) {
            if (!isdigit(c))
                return false;
        }

        int h = stoi(year);
        return h >= 1000 && h <= 2500;
    }


    bool isTimeInvalid(const std::string &startTime, const std::string &endTime) {
        // Với định dạng "HH:MM:SS", so sánh chuỗi trực tiếp là cách đơn giản và chính xác.
        // Trả về true nếu startTime >= endTime.
        return startTime >= endTime;
    }

    bool checkTimeGreater(const string &time1, const string &time2) {
        vector<int> t1 = getTimeVector(time1);
        vector<int> t2 = getTimeVector(time2);
        if (t1[0] == t2[0]) {
            if (t1[1] == t2[1]) {
                if (t1[2] > t2[2]) {
                    return true;
                } else {
                    return false;
                }
            } else if (t1[1] > t2[1]) {
                return true;
            } else {
                return false;
            }
        } else if (t1[0] > t2[0]) {
            return true;
        } else {
            return false;
        }
    }

    bool isDateBeforeToday(const string &date) {
        vector<int> dateVec = getDateVector(date);
        vector<int> currentDateVec = getCurrentDateTimeVector();

        if (dateVec[0] < currentDateVec[0]) {
            return true;
        } else if (dateVec[0] == currentDateVec[0]) {
            if (dateVec[1] < currentDateVec[1]) {
                return true;
            } else if (dateVec[1] == currentDateVec[1]) {
                if (dateVec[2] < currentDateVec[2]) {
                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
    }

    bool isDateToday(const string &date) {
        vector<int> dateVec = getDateVector(date);
        vector<int> currentDateVec = getCurrentDateTimeVector();

        return (dateVec[0] == currentDateVec[0] &&
                dateVec[1] == currentDateVec[1] &&
                dateVec[2] == currentDateVec[2]);
    }

    string getCurrentTime() {
        vector<int> currentDateTime = getCurrentDateTimeVector();
        char buffer[6];
        snprintf(buffer, sizeof(buffer), "%02d:%02d", currentDateTime[3], currentDateTime[4]);
        return string(buffer);
    }
};

#endif
