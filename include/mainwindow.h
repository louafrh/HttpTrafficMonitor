#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configurationsettingsdialog.h"
#include "../include/monitor.h"

#include <QMainWindow>
#include <thread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class ConfigurationSettingsDialog;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init(int argc, char *argv[]);

private:
    void initActions();
    void showStats();
    void showSummary();
    void clearUI();
    void initMonitor();

private slots:
    void on_timerUpdated();
    void on_logFileParsed();
    void on_refresh();
    void on_shouldAlert(bool isHighTraffic, int threshold);
    void on_configUpdated();

private:
    //Monitoring
    std::unique_ptr<Monitor> m_monitor;
    std::thread m_threadMonitor;

    //UI widgets
    Ui::MainWindow *ui;
    ConfigurationSettingsDialog *m_configSettingsDialog = nullptr;
};
#endif // MAINWINDOW_H
