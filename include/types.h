#ifndef TYPES_H
#define TYPES_H
#include <string>
#include <set>
#include <vector>

/**
 * @brief Class used to read the the access.log file, split read lines and return a HttpRequest struct .
 * @see type.h for details about HttpRequest
 */

struct Config {
    std::string logPath = "/temp/access.log";
    int threshold = 10;
    int logRefresh = 10;
    int monitorDelay = 120;
    bool generateWebReport = false;
};

struct HttpRequest {
    std::string host;
    std::string clientID;
    std::string userID;
    std::string date;
    std::string timeZone;
    std::string method;
    std::string url;
    std::string protocol;
    unsigned short status;
    unsigned int size;
};

enum alertType {
    ERROR = 0,
    WARNING,
    INFO,
    LOG
};

enum logFields {
    HOST = 0,
    CLIENTID,
    USERID,
    DATETIME,
    TIMEZONE,
    METHOD,
    URL,
    PROTOCOL,
    STATUS,
    BYTES
};
#endif // TYPES_H
