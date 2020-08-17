#include "../include/utils.h"
#include <iomanip>

#include <chrono>
#include <sstream>
#include <fstream>
#include <iostream>
#include <QDir>

const std::vector<std::string> Utils :: alertType_str ({"[ERROR] :: ", "[WARNING] :: ", "[INFO] :: ", "[LOG] :: "});

// Converts UTC time string to a time_t value.
std::time_t Utils::getEpochTime(const std::string& dateTime) {
    std::tm dt = getDateTime(dateTime);
    return std::mktime(&dt);
}

std::tm Utils::getDateTime(const std::string& dateTime) {
    std::istringstream ss(dateTime);
    std::tm dt;
    ss >> std::get_time(&dt, "%d/%m/%Y:%H:%M:%S");
    return dt;
}

std::time_t Utils::getEpochTime(std::tm& dt) {
    return std::mktime(&dt);
}

std::string Utils::getDateTimeNow() {
    auto sysTime = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(sysTime);
    return std::ctime(&end_time);
}

void Utils::initAppLogging() {
    std::string logPath = QDir::currentPath().toStdString() + "monitor.log";
    std::ofstream ofs(logPath);
    std::clog.rdbuf(ofs.rdbuf());
}

void Utils::log(alertType type, std::string msg) {
    std::clog<<Utils::getDateTimeNow()<<" - "<<Utils::alertType_str[type]<<msg<<std::endl;
   }
