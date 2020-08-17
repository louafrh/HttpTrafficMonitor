#ifndef LOGPARSER_H
#define LOGPARSER_H

#include "trafficreport.h"
#include "trafficreport.h"

#include <vector>
#include <fstream>
#include <memory>

#include <QObject>

/**
 * @brief Class used to read the the access.log file, split read lines and return a HttpRequest struct .
 * @see type.h for details about HttpRequest
 * @result HttpRequest
 */

class LogParser: public QObject
{
     Q_OBJECT

public:
    LogParser();
    void parseFile(std::shared_ptr<TrafficReport> trafficReport);
    void stopParsing();
    void setLogFile(const std::string& logFilePath);

private slots:
    void refresh();

private:
    //Methods
    void parseLine(std::istringstream line, std::shared_ptr<TrafficReport> trafficReport);
    int getHitSection(std::string field);

    //Attributes
    std::ifstream m_logFile;
    unsigned int m_seekLines;
    std::string m_logFilePath;

};

#endif // LOGPARSER_H
