#include "../include/timer.h"

Timer::Timer()
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}


Timer::~Timer() {
    delete m_timer;
}

void Timer::start(int refreshDelay, int monitorDelay) {
    m_monitorDelay = monitorDelay;
    m_refreshDelay =  refreshDelay;
    m_refreshRemaining = m_refreshDelay;
    m_alertRemaining = m_monitorDelay;
    m_timer->start(1000);
}

void Timer::setMonitorDelay(int monitorDelay){
    m_monitorDelay = monitorDelay;
}

void Timer::setRefresDelay(int refreshDelay){
    m_refreshDelay =  refreshDelay;
}

void Timer::stop() {
    m_timer->stop();
}

void Timer::update() {
    m_refreshRemaining--;
    if(m_refreshRemaining == 0){
        m_refreshRemaining = m_refreshDelay;
        emit shouldRefresh();
    }

    m_alertRemaining--;
    if(m_alertRemaining == 0) {
        m_alertRemaining = m_monitorDelay;
        emit shouldAlert();
    }
}
