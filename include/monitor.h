#ifndef MONITOR_H
#define MONITOR_H

#include <memory>

#include "configparser.h"
#include "logparser.h"
#include "trafficreport.h"
#include "trafficwebreport.h"
#include "timer.h"

class Monitor: public QObject
{
     Q_OBJECT
public:
    Monitor();
    ~Monitor();

    QTimer *getQTimer() const {return m_timer->getTimer();}
    Timer *getTimer() const {return m_timer.get();}
    void setConfig(std::shared_ptr<Config> config);

    const std::shared_ptr<TrafficReport> getTrafficReport() const;

    void start();
    void stop();
    void init(std::shared_ptr<Config> config);

private:
    void setWebConfig(bool generateWebReport);

signals:
    void logFileParsed();
    void shouldAlert(bool, int currThreshold);

//slots
private slots:
    void on_shouldRefresh();
    void on_shouldAlert();
    void generateWebReport();

//Attributes
private:
    //timer
    std::shared_ptr<Timer> m_timer;
    std::unique_ptr<TrafficWebReport> m_trafficWebReport;

    std::unique_ptr<LogParser> m_logParser;

    std::shared_ptr<Config> m_config;

    std::shared_ptr<TrafficReport> m_trafficReport;

    std::atomic<bool> m_monitoring;
    std::atomic<bool> m_refresh;
    std::atomic<bool> m_alert;
    std::atomic<bool> is_alerting;
};

#endif // MONITOR_H
