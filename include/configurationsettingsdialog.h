#ifndef CONFIGURATIONSETTINGSDIALOG_H
#define CONFIGURATIONSETTINGSDIALOG_H

#include <QDialog>
#include <QAbstractButton>

#include "../include/types.h"
#include <QtWidgets>


namespace Ui {
class ConfigurationSettingsDialog;
}

class ConfigurationSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationSettingsDialog(QWidget *parent = nullptr);

    ~ConfigurationSettingsDialog();

    std::shared_ptr<Config> getConfig() const { return m_config; }

    void initConfig(int argc, char *argv[]);
    void displayConfig();

private:
    bool isConfigValid();

signals:
    void configUpdated(std::shared_ptr<Config> m_config);

private slots:
    void browse(const char* filter, const QString& currentDir, QComboBox* uiCombo, bool newConfig);

    void on_saveButton_clicked();

    void on_browseLogFileButton_clicked();

    void on_browseConfigPathButton_clicked();


private:
    Ui::ConfigurationSettingsDialog *ui;
    std::shared_ptr<Config> m_config;

    QString errorStyleSheet = "border-style: outset; border-width: 1px; border-color: red;";
    QString validStyleSheet = "border-width: 0px;";
};

#endif // CONFIGURATIONSETTINGSDIALOG_H
