#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>
#include "types.h"

/**
 * @brief Class used to read the the access.log file, split read lines and return a HttpRequest struct .
 * @see type.h for details about HttpRequest
 */

class Utils
{
public:
    static time_t getEpochTime(const std::string& dateTime);
    static std::string getDateTimeNow();
    static std::tm getDateTime(const std::string& dateTime);
    static time_t getEpochTime(std::tm& dt);
    static void startTimer();
    static void initAppLogging();
    static void log(alertType type, std::string msg);

private:
    Utils();
    Utils(Utils const&) = delete;
    void operator = (Utils const&) = delete;

private:
    static const std::vector<std::string> alertType_str;

};
#endif // UTILS_H
