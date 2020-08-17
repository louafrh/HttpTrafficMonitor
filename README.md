# HttpTrafficMonitor
A simple UI program that monitors HTTP traffic on machine. 

* Display stats every 10s about the traffic during those 10s: the sections of the web site with the most hits, as well as interesting summary statistics on the traffic as a whole. A section is defined as being what's before the second '/' in the resource section of the log line. For example, the section for "/pages/create" is "/pages"
Make sure a user can keep the app running and monitor the log file continuously
* Whenever total traffic for the past 2 minutes exceeds a certain number on average, print or display a message saying that “High traffic generated an alert - hits = {value}, triggered at {time}”. The default threshold should be 10 requests per second, and should be overridable
* Whenever the total traffic drops again below that value on average for the past 2 minutes, print or display another message detailing when the alert recovered


#### Design
* ConfigParser :: a static class usinf to parse a file or CLI arguments and update the struct Config
* LogParser :: a unique_ptr used by the class Monitor, reads the log file every 20 second (define refresh time) and builds a HttpRequest struct that will be added to the trafic Report
* MainWindow:: The main App interface that displays the stats, alerts, timers and give access to the config ui, contains the config UI and a unique_ptr of the class Monitor that is rinning in a independant thread
* Monitor :: handles the wholes monitoring process, sends continuously signals to the UI to update the display and to the LogParser to update the report
* Timer :: Handles the refresh log and the monitor delay timers, notifies the UI ans the monitor 
* ConfigurationSettingsDialog : Qt interface to alllow user to configure the monitor 
* TrafficReport:: Handles the monitor statistics
* Utils :: a static class that contains the date/time and Application  log utilities
* type:: a header file, declares the structs HttpRequest that translates a single line parsed from the log file and Config that contains the define monitor configs

#### Development 
* C++14MainWindow
* GCC ( I used GCC 8 just in case I need C++17 functionalities
 but earlier versions can be used too )
* Cmake (3.16 is used in this project)
* QT5 is used to display ui and to insure the design pattern observer used by signal/Slots communication

## Testing
* Unit tests using Qtest framework by QT5

## Possible improvements
* Reading and parsing file take time and cause a delay to deliver 
the traffic statistics. watching the traffic directly without waiting
 for the access.log to be filled would generate a real time statistics.
 
* A more responsive Design instead of a the current UI would ease reading and
interpreting the statistic. I added a web page Dashboard as an optional 
output for this application, in order give a clear idea about how I 
imagined the monitoring dashboard. A web page Dashboard makes monitoring traffic 
easier and accessible from any device or machine without ssh to the server or, 
and that was the reason for which I chose to generate a web page 
instead of designing a responsive desktop UI, for instance.

* Additional alerts for recurrent errors or if the report errors / hits
is high (prefixed value at 30% for example) to underline that something 
is going south inside the server or the network and that the problem 
should be mitigated if necessary.

*  Alarming concerned people directly (ie. e-mail, push notifications, pop up ...) 
in case of suspicious behaviour or traffic problems 
(to be defined. ie. too many http requests detected, packets are lost ...)

* Saving the monitoring history would allow comparing the traffic behaviour, 
detect recurrent issues and get traffic statistics for a larger period of time.

  
