#include "../include/monitor.h"
#include <iostream>

/**
 * @brief Monitor::Monitor
 */
Monitor::Monitor()
{
    m_timer = std::make_unique<Timer>();
    m_trafficReport = std::make_shared<TrafficReport>();
    m_logParser = std::make_unique<LogParser>();
    m_trafficWebReport = std::make_unique<TrafficWebReport>();
    is_alerting = false;
    //connect signals/slots
    connect(m_timer.get(), SIGNAL(shouldRefresh()), this, SLOT(on_shouldRefresh()));
    connect(m_timer.get(), SIGNAL(shouldAlert()), this, SLOT(on_shouldAlert()));
}

Monitor::~Monitor(){

}

/**
 * @brief Monitor::init
 */
void Monitor::init(std::shared_ptr<Config> config) {
    setConfig(config);
    m_timer->start(m_config->logRefresh, m_config->monitorDelay);
    m_refresh = true;
    m_alert = false;
    setWebConfig(m_config->generateWebReport);
}

/**
 * @brief Monitor::start
 */
void Monitor::start() {
    m_monitoring = true;
    while(m_monitoring) {
        if(m_refresh) {
            m_refresh = false;
            m_trafficReport->setCurrDateTime();
            m_logParser->parseFile(m_trafficReport);
            emit logFileParsed();
        }
        if(m_alert){
            m_alert = false;
            if(m_trafficReport->getThreshold() >= m_config->threshold){
                emit shouldAlert(true, m_trafficReport->getThreshold());
                is_alerting = true;

            }
            else {
                if(is_alerting) {
                    emit shouldAlert(false, m_trafficReport->getThreshold());
                    is_alerting = false;
                }
            }
            m_trafficReport->addThreshold();
        }
    }
    m_monitoring = false;
}

/**
 * @brief Monitor::on_shouldRefresh
 */
void Monitor::on_shouldRefresh() {
    m_refresh = true;
}


void Monitor::on_shouldAlert() {
    m_alert = true;
}

/**
 * @brief Monitor::stop
 */
void Monitor::stop() {
    m_monitoring = false;
    m_timer->stop();
}

/**
 * @brief Monitor::getTrafficReport
 * @return
 */
const std::shared_ptr<TrafficReport> Monitor::getTrafficReport() const {
    return m_trafficReport;
}

/**
 * @brief Monitor::setConfig
 * @param config
 */
void Monitor::setConfig(std::shared_ptr<Config> config) {
    m_config = config;
    m_logParser->setLogFile(m_config->logPath);
    setWebConfig(m_config->generateWebReport);
}

void Monitor::setWebConfig(bool generateWebReport){
    if(generateWebReport)
        connect(m_timer.get(), SIGNAL(shouldRefresh()), this, SLOT(generateWebReport()));
    else
        disconnect(m_timer.get(), SIGNAL(shouldRefresh()), this, SLOT(generateWebReport()));
}

void Monitor::generateWebReport(){
    m_trafficWebReport->generateReport(m_trafficReport);
}
