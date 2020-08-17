#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <memory>
#include <QtCore/qobject.h>
#include "types.h"

/**
 * @static
 * @brief Class used by main.cpp to read command line arguments or formatted cfg file, split the content and return the
 * application config fields within a struct.
 * @result Struct Config
 */

class ConfigParser: public QObject
{
     Q_OBJECT

public:
    static void parseConfig(std::shared_ptr<Config> config, int argc, char* argv[]);
    static void parseConfig(std::shared_ptr<Config> config, const std::string& configFilePath);

signals:
   void parseConfigEnd();

private:
    static void fillConfig(std::shared_ptr<Config> config, const std::string& name, const std::string& value);
    static void setIntIfValid(int* param, const std::string& value, const std::string& paramName);

};

#endif // CONFIGPARSER_H
