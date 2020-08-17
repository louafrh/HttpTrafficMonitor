#include "../include/logparser.h"
#include "../include/utils.h"

#include <sstream>

LogParser::LogParser() : m_seekLines(0){
}

void LogParser::stopParsing() {
    m_seekLines = m_logFile.tellg();
    m_logFile.close();
}

void LogParser::setLogFile(const std::string&logFilePath) {
    if (m_logFilePath == logFilePath)
            return;
    if(m_logFile.is_open())
        m_logFile.close();
    m_logFilePath = logFilePath;
    m_logFile.open(logFilePath, std::ios::binary);
    m_seekLines = 0;
}

void LogParser::refresh(){

}

void LogParser::parseFile(std::shared_ptr<TrafficReport> trafficReport) {
    if(m_logFile.is_open()) {
        if(m_seekLines > 0)
            m_logFile.seekg(m_seekLines, std::ios::beg);
        std::string line;
        //ifstream::open does not read the file into memory. an ifstreram will only load a small buffer into memory at any one time.
        while(getline(m_logFile, line)){
            parseLine(std::istringstream (line), trafficReport);
        }
        m_seekLines = m_logFile.tellg();
    }
}

void LogParser::parseLine(std::istringstream line, std::shared_ptr<TrafficReport> trafficReport) {
    int i = 0;
    std::string field;
    HttpRequest httpRequest;
    //line ie. 127.0.0.1 user-identifier frank [10/Oct/2000:13:55:36 -0700] "GET /apache_pb.gif HTTP/1.0" 200 2326
    while(getline(line, field, ' ')) {
        switch(i) {
            case HOST:
                httpRequest.host = field;
                break;
            case CLIENTID:
                httpRequest.clientID = field;
                break;
            case USERID:
                httpRequest.userID = field;
                break;
            case DATETIME:
                httpRequest.date = field.substr(field.find("[") + 1);
            case TIMEZONE:
                httpRequest.timeZone = field.substr(0, field.find("]"));
                break;
            case METHOD:
                httpRequest.method = field;
                break;
            case URL:
                httpRequest.url = field.substr(0, getHitSection(field));
            case PROTOCOL:
                httpRequest.protocol = field;
                break;
            case STATUS:
                try {
                    httpRequest.status = field.empty() ? 0 : std::stoi(field);
                } catch (std::exception e) {
                    httpRequest.status = 0;
                }
                break;
            case BYTES:
                try {
                    httpRequest.size = field.empty() ? 0 : std::stoi(field);
                } catch (std::exception e) {
                    httpRequest.size = 0;
                }
                break;
        }
        if(i == BYTES)
            break;
        i++;
    }
    if(i == BYTES)
        trafficReport->addNewTrafficEntry(httpRequest);
    else //no request size provided!!
        Utils::log(WARNING, "LogParser:: Malformatted log line, it will be ignored");
}

int LogParser::getHitSection(std::string field) {
    int pos = field.find_first_of("/");
    if (pos == std::string::npos)
        return field.length();
    pos = field.find_first_of("/", pos+1);
    if (pos == std::string::npos)
        return field.length();
    return pos;
}
