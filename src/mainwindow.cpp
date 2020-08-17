#include "../include/mainwindow.h"
#include "./ui_mainwindow.h"
#include "../include/utils.h"

#include <memory>
#include <iostream>

/**
 * @brief MainWindow::MainWindow : the main monitor's UI
 * @param parent
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setting Dialog
    m_configSettingsDialog = new ConfigurationSettingsDialog(this);
    connect(m_configSettingsDialog, SIGNAL(configUpdated()), this, SLOT(on_configUpdated()));

    //ui
    initActions();

    clearUI();
    //monitor
    initMonitor();

    //Diplay timers
    ui->flushLcdNumber->display(m_configSettingsDialog->getConfig()->monitorDelay);
    ui->refreshLcdNumber->display(m_configSettingsDialog->getConfig()->logRefresh);
}

void MainWindow::initMonitor() {
    m_monitor = std::make_unique<Monitor>();
    m_monitor->init(m_configSettingsDialog->getConfig());
    connect(m_monitor->getQTimer(), SIGNAL(timeout()), this, SLOT(on_timerUpdated()));
    connect(m_monitor.get(), SIGNAL(logFileParsed()), this, SLOT(on_logFileParsed()));
    connect(m_monitor.get(), SIGNAL(shouldAlert(bool, int)), this, SLOT(on_shouldAlert(bool, int)));

    m_threadMonitor = std::thread(&Monitor::start, m_monitor.get());
}

void MainWindow::initActions() {
     //Enable Action menu buttons
    ui->actionHelp->setEnabled(true);
    ui->actionExport_Stats->setEnabled(true);
    ui->settingsAction->setEnabled(true);
    ui->quitAction->setEnabled(true);

     //Connect Action menu buttons slots
    connect(ui->quitAction, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->settingsAction, &QAction::triggered, m_configSettingsDialog, &QDialog::show);
}

void MainWindow::init(int argc, char *argv[]) {
    m_configSettingsDialog->initConfig(argc, argv);
}

MainWindow::~MainWindow()
{
    delete m_configSettingsDialog;
    delete ui;
}


void MainWindow::on_logFileParsed() {
    showSummary();
    showStats();
}


void MainWindow::on_timerUpdated(){
    int val = ui->flushLcdNumber->value() - 1;
    if(val < 0)
        val = m_configSettingsDialog->getConfig()->monitorDelay;
    ui->flushLcdNumber->display(val);

    val = ui->refreshLcdNumber->value() - 1;
    if(val < 0)
        val = m_configSettingsDialog->getConfig()->logRefresh;

    ui->refreshLcdNumber->display(val);
}

void MainWindow::on_refresh() {
    ui->logtimeLabelVal->setText(QString::fromStdString(Utils::getDateTimeNow()));
    showSummary();
    showStats();
}

void MainWindow::clearUI() {
    //clear summary section
    ui->logtimeLabelVal->setText("");
    ui->thresholdLabelVal->setText("");
    ui->totalHitsLabelVal->setText("");
    ui->sucessfulHitsLabelVal->setText("");
    ui->avgRequestSizeLabelVal->setText("");
    ui->totalBytesLabelVal->setText("");
    ui->topURLLabelVal->setText("");
    ui->topStatusLabelVal->setText("");
    ui->topProtocolLabelVal->setText("");

    //clear stats
    ui->methodTableWidget->clearContents();
    ui->statusTableWidget->clearContents();
    ui->dateTimeTableWidget->clearContents();
    ui->thresholdTableWidget->clearContents();
    ui->protocolTableWidget->clearContents();
    ui->urlTableWidget->clearContents();

    //clear alerts
    ui->alertslistWidget->clear();
}

void MainWindow::showStats() {

    ui->methodTableWidget->setRowCount(0);
    ui->statusTableWidget->setRowCount(0);
    ui->dateTimeTableWidget->setRowCount(0);
    ui->thresholdTableWidget->setRowCount(0);
    ui->protocolTableWidget->setRowCount(0);
    ui->urlTableWidget->setRowCount(0);

    auto report = m_monitor->getTrafficReport();
    int row = 0;
    for(auto it : report->getRequestsPerDateTime()) {
        row = ui->dateTimeTableWidget->rowCount();
        ui->dateTimeTableWidget->insertRow(row);
        ui->dateTimeTableWidget->setItem(row,0,new QTableWidgetItem(QString::fromStdString(it.first)));
        ui->dateTimeTableWidget->setItem(row,1,new QTableWidgetItem(QString::number(it.second)));
        auto itr = report->getTotalErrorsPerDateTime().find(it.first);
        float sucessRate = 100;
        if( itr != report->getTotalErrorsPerDateTime().end() && it.second > 0) {
            sucessRate = ((it.second - itr->second) / it.second) * 100;
        }
        ui->dateTimeTableWidget->setItem(row,2,new QTableWidgetItem(QString::number(sucessRate)));
    }

    for(auto it : report->getRequestsPerMethod()) {
        row = ui->methodTableWidget->rowCount();
        ui->methodTableWidget->insertRow(row);
        ui->methodTableWidget->setItem(row,0,new QTableWidgetItem(QString::fromStdString(it.first)));
        ui->methodTableWidget->setItem(row,1,new QTableWidgetItem(QString::number(it.second)));
        auto itr = report->getTotalErrorsPerMethod().find(it.first);
        float sucessRate = 100;
        if( itr != report->getTotalErrorsPerMethod().end() && it.second > 0) {
            sucessRate = ((it.second - itr->second) / it.second) * 100;
        }
        ui->methodTableWidget->setItem(row,2,new QTableWidgetItem(QString::number(sucessRate)));
    }

    for(auto it : report->getRequestsPerStatus()) {
        row = ui->statusTableWidget->rowCount();
        ui->statusTableWidget->insertRow(row);
        ui->statusTableWidget->setItem(row,0,new QTableWidgetItem(QString::number(it.first)));
        ui->statusTableWidget->setItem(row,1,new QTableWidgetItem(QString::number(it.second)));
    }

    for(auto it : report->getThresholdHistory()) {
        row = ui->thresholdTableWidget->rowCount();
        ui->thresholdTableWidget->insertRow(row);
        ui->thresholdTableWidget->setItem(row,0,new QTableWidgetItem(QString::fromStdString(it.first)));
        ui->thresholdTableWidget->setItem(row,1,new QTableWidgetItem(QString::number(it.second)));
    }

    for(auto it : report->getRequestsPerProtocol()) {
        row = ui->protocolTableWidget->rowCount();
        ui->protocolTableWidget->insertRow(row);
        ui->protocolTableWidget->setItem(row,0,new QTableWidgetItem(QString::fromStdString(it.first)));
        ui->protocolTableWidget->setItem(row,1,new QTableWidgetItem(QString::number(it.second)));
        auto itr = report->getTotalErrorsPerProtocol().find(it.first);
        float sucessRate = 100;
        if( itr != report->getTotalErrorsPerProtocol().end() && it.second > 0) {
            sucessRate = ((it.second - itr->second) / it.second) * 100;
        }
        ui->protocolTableWidget->setItem(row,2,new QTableWidgetItem(QString::number(sucessRate)));
    }

    for(auto it : report->getRequestsPerUrl()) {
        row = ui->urlTableWidget->rowCount();
        ui->urlTableWidget->insertRow(row);
        ui->urlTableWidget->setItem(row,0,new QTableWidgetItem(QString::fromStdString(it.first)));
        ui->urlTableWidget->setItem(row,1,new QTableWidgetItem(QString::number(it.second)));
        auto itr = report->getTotalErrorsPerUrl().find(it.first);
        float sucessRate = 100;
        if( itr != report->getTotalErrorsPerUrl().end() && it.second > 0) {
            sucessRate = ((it.second - itr->second) / it.second) * 100;
        }
        ui->urlTableWidget->setItem(row,2,new QTableWidgetItem(QString::number(sucessRate)));
    }
}

void MainWindow::showSummary() {
    auto report = m_monitor->getTrafficReport();
    ui->logtimeLabelVal->setText(QString::fromStdString(report->getDateTime()));
    ui->thresholdLabelVal->setNum(report->getThreshold());
    ui->totalHitsLabelVal->setNum(report->getTotalRequestsCount());
    ui->sucessfulHitsLabelVal->setNum(report->getTotalSuccessCount());
    ui->avgRequestSizeLabelVal->setNum(report->getAvgTransmittedBytes());
    ui->totalBytesLabelVal->setNum(report->getAvgTransmittedBytes());
    //ui->topUserLabelVal->setText(QString::fromStdString(report->getMostActiveUser().first));
    ui->topURLLabelVal->setText(QString::fromStdString(report->getMostRequestedUrl().first));
    ui->topStatusLabelVal->setNum(report->getMostReturnedStatus().first);
    ui->topProtocolLabelVal->setText(QString::fromStdString(report->getMostUsedProtocol().first));
}

void MainWindow::on_shouldAlert(bool isHighTraffic, int threshold) {
    auto report = m_monitor->getTrafficReport();
    if(isHighTraffic){
        ui->alertslistWidget->addItem("High traffic generated an alert - hits = " + QString::number(threshold) + ", triggered at" + QString::fromStdString(report->getDateTime()));
        ui->alertslistWidget->item(ui->alertslistWidget->count() - 1)->setTextColor(QColor("red"));
    }

    else {
        ui->alertslistWidget->addItem("Normal traffic generated recovered - hits = " + QString::number(threshold) + ", triggered at" + QString::fromStdString(report->getDateTime()));
        ui->alertslistWidget->item(ui->alertslistWidget->count() - 1)->setTextColor(QColor("blue"));
    }
}

void MainWindow::on_configUpdated(){
    m_monitor->setConfig(m_configSettingsDialog->getConfig());
}

