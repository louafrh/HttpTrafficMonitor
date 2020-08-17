//C++
#include <fstream>
#include <string>

//Local project
#include "../include/configparser.h"
#include "../include/utils.h"

void ConfigParser::parseConfig(std::shared_ptr<Config> config, int argc, char* argv[]) {
    std::string prevArg = "";
    for (int i=1; i < argc; ++i) {
        if(!prevArg.empty() && prevArg.find("--") != std::string::npos) {
            ConfigParser::fillConfig(config, prevArg,argv[i]);
        }
        else
            prevArg = std::string(argv[i]);
    }
}

void ConfigParser::parseConfig(std::shared_ptr<Config> config, const std::string& configFilePath) {
    std::ifstream cFile(configFilePath);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            // remove white spaces
            line.erase(remove_if(line.begin(), line.end(), isspace),line.end());
            //ignore comments and empty lines
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            // ignore invalid config
            if(delimiterPos == std::string::npos)
                continue;
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            fillConfig(config, name, value);
        }
    }
    else {
      //  AlertsHandler::echoCannotOpenFile(configFilePath);
    }
}

void ConfigParser::fillConfig(std::shared_ptr<Config> config, const std::string& name, const std::string& value) {
    // fill config
    if(name.find("logPath") != std::string::npos) {
        //check if the log path is valis
        std::ifstream validPath(value);
        if (!validPath)
            Utils::log(ERROR, "ConfigParser:: The file doesn't exist: " + value);
        else
            config->logPath = value;
    }
    else if(name.find("logRefresh") != std::string::npos) {
        setIntIfValid(&config->logRefresh, value, name);
    }
    else if(name.find("monitorDelay") != std::string::npos) {
        setIntIfValid(&(config->monitorDelay), value, name);
    }
    else if(name.find("threshold") != std::string::npos) {
        setIntIfValid(&(config->threshold), value, name);
    }
    else
        Utils::log(WARNING, "ConfigParser :: Unknown config parameter: " + name);
}

void ConfigParser::setIntIfValid(int* param, const std::string& value, const std::string& paramName) {
    try
    {
        *param = stoi(value);
    }
    catch (std::invalid_argument const &e)
    {
        Utils::log(WARNING, "ConfigParser ::Config parameter should be an integer: " + paramName);
    }
    catch (std::out_of_range const &e)
    {
        Utils::log(WARNING, "ConfigParser ::Config parameter is out of range: " + paramName);
    }
}

