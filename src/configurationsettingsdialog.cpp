//local project
#include "../include/configurationsettingsdialog.h"
#include "ui_configurationsettingsdialog.h"
#include "../include/configparser.h"

//C++
#include <iostream>
#include <fstream>

//QT
#include <QDir>


ConfigurationSettingsDialog::ConfigurationSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationSettingsDialog)
{
    ui->setupUi(this);
    m_config = std::make_shared<Config>();
    displayConfig();
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::close);
}

ConfigurationSettingsDialog::~ConfigurationSettingsDialog()
{
    delete ui;
}

void ConfigurationSettingsDialog::browse(const char* filter, const QString& currentDir, QComboBox* uiCombo, bool newConfig)
{
    QString directory = QDir::toNativeSeparators(QFileDialog::getOpenFileName(this, tr("Find Files"), currentDir, tr(filter)));

    if (!directory.isEmpty()) {
        if (uiCombo->findText(directory) == -1)
            uiCombo->addItem(directory);
        uiCombo->setCurrentIndex(uiCombo->findText(directory));
        if(newConfig) {

            ConfigParser::parseConfig(m_config, directory.toStdString());
            displayConfig();
        }
    }
}

void ConfigurationSettingsDialog::on_browseLogFileButton_clicked()
{
    if(!ui->settingWarningsLabel->text().isEmpty()) {
        ui->settingWarningsLabel->setText("");
        ui->loglocationCombo->setStyleSheet(validStyleSheet);
    }
     QString path = QDir::cleanPath(ui->loglocationCombo->currentText());
     if(path.isEmpty())
         path = QDir::currentPath();

     browse("files (*.log)", path, ui->loglocationCombo, false);
}

void ConfigurationSettingsDialog::on_browseConfigPathButton_clicked()
{
     ui->configPathCombo->setStyleSheet(validStyleSheet);
     QString path = QDir::cleanPath(ui->configPathCombo->currentText());
     if(path.isEmpty())
         path = QDir::currentPath();

     browse("files (*.cfg)", path, ui->configPathCombo, true);
}

void ConfigurationSettingsDialog::on_saveButton_clicked()
{
    if(isConfigValid()){
        m_config->logPath = ui->loglocationCombo->currentText().toStdString();
        m_config->threshold = ui->thresholdLimitSpinner->text().toInt();
        m_config->logRefresh = ui->refreshDelaySpinner->text().toInt();
        m_config->monitorDelay = ui->flushDelaySpinner->text().toInt();
        m_config->generateWebReport = ui->webDashboardCombo->currentText() == "No" ? false : true;
        emit configUpdated(m_config);
        ConfigurationSettingsDialog::close();
    }
}

bool ConfigurationSettingsDialog::isConfigValid() {
    std::ifstream logPath(ui->loglocationCombo->currentText().toStdString());
    if(!logPath) {
        ui->settingWarningsLabel->setText("Log file path is not valid!");
        ui->loglocationCombo->setStyleSheet(errorStyleSheet);
        return false;
    }
    ui->settingWarningsLabel->setText("");
    ui->loglocationCombo->setStyleSheet(validStyleSheet);
    return true;
}

void ConfigurationSettingsDialog::initConfig(int argc, char *argv[]) {
    ConfigParser::parseConfig(m_config, argc, argv);
}

void ConfigurationSettingsDialog::displayConfig(){
    ui->loglocationCombo->setCurrentText(QString::fromStdString(m_config->logPath));
    ui->thresholdLimitSpinner->setValue(m_config->threshold);
    ui->refreshDelaySpinner->setValue(m_config->logRefresh);
    ui->flushDelaySpinner->setValue(m_config->monitorDelay);
    ui->webDashboardCombo->setCurrentIndex(m_config->generateWebReport ? 1 : 0);
}
