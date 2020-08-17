#ifndef TIMER_H
#define TIMER_H

#include <QObject>
#include <QTimer>

class Timer: public QObject
{
     Q_OBJECT

public:
    Timer();
    ~Timer();
    void start(int refreshDelay, int monitorDelay);
    void stop();
    QTimer *getTimer() const {return m_timer;}

    void setMonitorDelay(int monitorDelay);
    void setRefresDelay(int refreshDelay);

signals:
    void shouldRefresh();
    void shouldAlert();

private slots:
    void update();

private:
    QTimer *m_timer;
    int m_refreshRemaining;
    int m_alertRemaining;

    std::atomic<int> m_monitorDelay;
    std::atomic<int> m_refreshDelay;
};

#endif // TIMER_H
