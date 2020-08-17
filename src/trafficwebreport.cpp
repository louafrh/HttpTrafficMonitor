#include "../include/trafficwebreport.h"
#include <iostream>
#include <fstream>

#include <QDir>

TrafficWebReport::TrafficWebReport()
{

}

void TrafficWebReport::generateReport(std::shared_ptr<TrafficReport> trafficReport){
    std::string webPagePath = QDir::currentPath().toStdString() + "statsReport.html";
    std::ofstream webPage (webPagePath);

    //set page header
    webPage<<"<!DOCTYPE html> \
              <html lang=\"en\">\
              <head>\
                <meta charset=\"UTF-8\" http-equiv=\"refresh\" content=\"20\">\
                <title>Traffic monitor</title>\
                <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css\" integrity=\"sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk\" crossorigin=\"anonymous\">\
                <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/chartist.js/latest/chartist.min.css\">\
                <script src=\"https://cdn.jsdelivr.net/npm/chartist@0.11.4/dist/chartist.min.js\"></script>\
                <style>\
                  .col-6 {\
                    height: 250px;\
                  }\
                </style>\
              </head>\
              <body>"<<std::endl;

    // insert charts
    webPage<<"<div class=\"container mt-5\">\
             <div class=\"row\">\
               <div class=\"col-6\">\
                 <div class=\"line-chart ct-perfect-fourth\"></div>\
               </div>\
               <div class=\"col-6\">\
                 <div class=\"bar-chart ct-perfect-fourth\"></div>\
               </div>\
             </div>\
             <div class=\"row\">\
               <div class=\"col-6\">\
                 <div class=\"dot-chart ct-perfect-fourth\"></div>\
               </div>\
               <div class=\"col-6\">\
                 <div class=\"pie-chart ct-perfect-fourth\"></div>\
               </div>\
             </div>\
           </div>"<<std::endl;

    // insert stats table
    // table header
    webPage<<"<div class=\"container mt-5\">\
             <table class=\"table table-sm\">\
               <thead>\
               <tr>\
                 <th scope=\"col\">Date/Time</th>\
                 <th scope=\"col\">Hits</th>\
                 <th scope=\"col\">Success rate</th>\
               </tr>\
               </thead>\
               <tbody>"<<std::endl;
    for(auto it : trafficReport->getRequestsPerDateTime()) {
            float suceessRate = ((it.second - trafficReport->getTotalErrorsPerUrl().find("")->second) / it.second) * 100;
          webPage<<"<tr>\
                   <td>"<<it.first<<"</td>\
                   <td>"<<it.second<<"</td>\
                   <td>"<<suceessRate<<"%</td>\
                 </tr>";
}

     webPage<<"</tbody>\
             </table>\
            </div>"<<std::endl;
    //insert JS
    webPage<<"<script>"<<std::endl;
    webPage<<"const chartHeight = 250"<<std::endl;
    webPage<<"new Chartist.Line('.line-chart', {labels: [";
    std::string hitsList = "";
    for(auto it : trafficReport->getRequestsPerStatus()) {
        webPage<<"'"<<it.first << "',";
        hitsList += it.second + ",";
    }
    webPage<<"],series: [["<<hitsList<<"]\
             ]\
           }, {\
             fullWidth: true,\
             height: chartHeight,\
             chartPadding: {\
               right: 40\
             }\
           });";
    std::string successHitsList = "";
    std::string failedHitsList="";
    webPage<<"new Chartist.Bar('.bar-chart', {labels: [";
    for(auto it: trafficReport->getRequestsPerProtocol()){
        webPage<<"'"<<it.first << "',";
        auto itr = trafficReport->getTotalErrorsPerProtocol().find(it.first);
        int i = itr == trafficReport->getTotalErrorsPerProtocol().end() ? 0 : itr->second;
        successHitsList += (it.second - i) + ",";
        failedHitsList += i + ",";
    }
     webPage<<"],series: [["<<successHitsList<<"],\
              ["<<failedHitsList<<"]]\
                }, {\
              height: chartHeight,\
              stackBars: true,\
              axisY: {\
                labelInterpolationFnc: function(value) {\
                  return value;\
                }\
              }\
            }).on('draw', function(data) {\
              if(data.type === 'bar') {\
                data.element.attr({\
                  style: 'stroke-width: 30px'\
                });\
              }\
            });";

     webPage<<"new Chartist.Line('.dot-chart', {labels: [";
     hitsList = "";
     for(auto it: trafficReport->getThresholdHistory()){
         webPage<<"'"<<it.first << "',";
         hitsList += it.second + ",";
     }
     webPage<<"],series: [["<<hitsList<<"]\
              ]\
            }, {\
              fullWidth: true,\
              height: chartHeight,\
              chartPadding: {\
                right: 10\
              },\
              low: 0\
            });";

     webPage<<"let dataPie = {labels: [";
     hitsList = "";
     for(auto it: trafficReport->getRequestsPerUrl()){
         webPage<<"'"<<it.first << "',";
         hitsList += it.second + ",";
     }
     webPage<<"],series: ["<<hitsList<<"]};\
              new Chartist.Pie('.pie-chart', dataPie, {\
                height: chartHeight\
              });"<<std::endl;
     webPage<<"</script>"<<std::endl;
    // close HTLM tags
    webPage<<"</body>\
             </html>"<<std::endl;
    webPage.close();
}


