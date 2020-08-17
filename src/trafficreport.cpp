#include "../include/trafficreport.h"
#include "../include/utils.h"

#include <iostream>

TrafficReport::TrafficReport() {
    m_currTime = Utils::getDateTimeNow();
    m_totalRequestsCount = 0;
    m_totalBytesCount = 0;
    m_totalErrorsCount = 0;
    m_threshold = 0;
}

void TrafficReport::addNewTrafficEntry(HttpRequest request) {
    m_totalRequestsCount++;
    m_threshold++;
    m_totalBytesCount += request.size;

    if(m_httpSuccessCodes.find(request.status) == m_httpSuccessCodes.end()) {
        m_totalErrorsCount++;

        m_errorsPerProtocol[request.protocol]++;
        m_errorsPerMethod[request.method]++;
        m_errorsPePerUrl[request.url]++;
        m_errorsPerDateTime[request.date]++;
    }

    m_requestsPerProtocol[request.protocol]++;
    m_requestsPerMethod[request.method]++;
    m_requestsPerUrl[request.url]++;
    m_requestsPerStatus[request.status]++;
    m_requestsPerDateTime[request.date]++;

    if(m_requestsPerProtocol.find(request.protocol)->second > m_mostUsedProtocol.second) {
        m_mostUsedProtocol.first = m_requestsPerProtocol.find(request.protocol)->first;
        m_mostUsedProtocol.second = m_requestsPerProtocol.find(request.protocol)->second;
    }

    if(m_requestsPerUrl.size() == 0) {
        m_mostRequestedUrl.first = request.url;
        m_mostRequestedUrl.second = 1;
    }
    else if(m_requestsPerUrl.find(request.url)->second > m_mostRequestedUrl.second) {
        m_mostRequestedUrl.first = m_requestsPerUrl.find(request.url)->first;
        m_mostRequestedUrl.second = m_requestsPerUrl.find(request.url)->second;
    }


    if(m_requestsPerStatus.size() == 0) {
        m_mostReturnedStatus.first = request.status;
        m_mostReturnedStatus.second = 1;
    }
    else if(m_requestsPerStatus.find(request.status)->second > m_mostReturnedStatus.second) {
        m_mostReturnedStatus.first = m_requestsPerStatus.find(request.status)->first;
        m_mostReturnedStatus.second = m_requestsPerStatus.find(request.status)->second;
    }

    if(m_requestsPerProtocol.size() == 0) {
        m_mostUsedProtocol.first = request.protocol;
        m_mostUsedProtocol.second = 1;
    }
    else if(m_requestsPerProtocol.find(request.protocol)->second > m_mostUsedProtocol.second) {
        m_mostUsedProtocol.first = m_requestsPerProtocol.find(request.protocol)->first;
        m_mostUsedProtocol.second = m_requestsPerProtocol.find(request.protocol)->second;
    }



}

void TrafficReport::addThreshold(){
    m_thresholdHistoty[Utils::getDateTimeNow()] = m_threshold;
    m_threshold = 0;
}

void TrafficReport::setCurrDateTime() {
    m_currTime = Utils::getDateTimeNow();
}
