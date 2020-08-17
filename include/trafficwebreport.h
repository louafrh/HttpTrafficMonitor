#ifndef TRAFFICWEBREPORT_H
#define TRAFFICWEBREPORT_H

#include "trafficreport.h"

#include <QObject>
#include <memory>


class TrafficWebReport: public QObject
{
    Q_OBJECT

public:
    TrafficWebReport();
    void generateReport(std::shared_ptr<TrafficReport> trafficReport);

private:
  //  string buildChartDiv();

};

#endif // TRAFFICWEBREPORT_H
