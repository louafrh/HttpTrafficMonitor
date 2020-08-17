#ifndef TRAFFICREPORT_H
#define TRAFFICREPORT_H

#include <string>
#include <map>
#include "types.h"


/**
 * @brief Class used to read the the access.log file, split read lines and return a HttpRequest struct .
 * @see type.h for details about HttpRequest
 * @result HttpRequest
 */
class TrafficReport
{
public:
    TrafficReport();

    // Get summary statistics on the traffic as a whole
    int getTotalSuccessCount() const { return m_totalRequestsCount - m_totalErrorsCount; }
    int getTotalRequestsCount() const { return m_totalRequestsCount; }
    int getTotalErrorsCount() const { return m_totalErrorsCount; }
    int getTotalBytesCount() const { return m_totalBytesCount; }
    float getAvgTransmittedBytes() const { return (m_totalRequestsCount > 0 ? (m_totalBytesCount/m_totalRequestsCount) : 0) ; }
    int getThreshold() const { return m_threshold; }
    std::string getDateTime() { return m_currTime; }
    void setCurrDateTime();

    const std::pair<std::string, int>& getMostActiveHost() const {return m_mostActiveHost;}
    const std::pair<std::string, int>& getMostRequestedUrl() const {return m_mostRequestedUrl;}
    const std::pair<std::string, int>& getMostUsedProtocol() const {return m_mostUsedProtocol;}
    const std::pair<int, int>& getMostReturnedStatus() const {return m_mostReturnedStatus;}

    const std::map<std::string, int>& getRequestsPerMethod() const {return m_requestsPerMethod; }
    const std::map<std::string, int>& getRequestsPerUrl() const {return m_requestsPerUrl; }
    const std::map<int, int>& getRequestsPerStatus() const {return m_requestsPerStatus; }
    const std::map<std::string ,int>& getRequestsPerDateTime() const {return m_requestsPerDateTime; }
    const std::map<std::string, int>& getRequestsPerProtocol() const {return m_requestsPerProtocol; }

    const std::map<std::string,int>& getTotalErrorsPerProtocol() const {return m_errorsPerProtocol; }
    const std::map<std::string, int>& getTotalErrorsPerMethod() const {return m_errorsPerMethod; }

    const std::map<std::string, int>& getTotalErrorsPerUrl() const {return m_errorsPePerUrl; }

    const std::map<std::string , int>& getTotalErrorsPerDateTime() const {return m_errorsPerDateTime; }
    const std::map<std::string , int>& getThresholdHistory() const {return m_thresholdHistoty; }


    void addThreshold();
  //Set report stat
  void addNewTrafficEntry(HttpRequest request);

private:
  std::set<unsigned short> m_httpSuccessCodes = {100, 101, 102, 103, 200, 201, 202, 203, 204, 205, 206, 207, 208,
                                              210, 226, 300, 301, 302, 303, 304, 305, 306, 307, 308};
  std::string m_currTime;
  unsigned int m_totalRequestsCount;
  unsigned int m_totalErrorsCount;
  unsigned int m_totalBytesCount;
  int m_threshold;

  std::pair<std::string, int> m_mostActiveHost;
  std::pair<std::string, int> m_mostRequestedUrl;
  std::pair<std::string, int> m_mostUsedProtocol;
  std::pair<int, int> m_mostReturnedStatus;

  std::map<std::string, int> m_thresholdHistoty;

  std::map<std::string , int> m_requestsPerDateTime;
  std::map<std::string, int> m_errorsPerDateTime;

  std::map<std::string, int> m_requestsPerUrl;
   std::map<std::string, int> m_errorsPePerUrl;

  std::map<int,int> m_requestsPerStatus;

  std::map<std::string, int> m_errorsPerProtocol;
  std::map<std::string, int> m_requestsPerProtocol;

  std::map<std::string,int> m_requestsPerMethod;
  std::map<std::string, int> m_errorsPerMethod;
};

#endif // TRAFFICREPORT_H
