/*
 * Header file containing application settings and configurations.
 * Defines various constants and parameters used throughout the application.
 * Note: Make sure to delete the config file before every change as this code only changes value if the parameters don't exist at all
 */

#pragma once

#include <QSettings>
#include <QStandardPaths>
#include <QDebug>

class Settings : public QSettings {
public:
    static Settings& instance() {
        static Settings instance;
        return instance;
    }

    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

private:
    Settings() : QSettings(getIniFilePath(), QSettings::IniFormat) {
        setDefaults();
    }

    static QString getIniFilePath() {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        return configPath + "/qtstudentdbmanager.ini";
    }

    void setDefaults() {
        if (!contains("server/host")) {
            setValue("server/host", "127.0.0.1");
        }
        if (!contains("server/port")) {
            setValue("server/port", 12345);
        }
        
        if (!contains("server/dbHost")) {
            setValue("server/dbHost", "127.0.0.1");
        }
        
        if (!contains("server/dbPort")) {
            setValue("server/dbPort", 5432);
        }
        
        if (!contains("server/dbUser")) {
            setValue("server/dbUser", "myuser");
        }
        
        if (!contains("server/dbPassword")) {
            setValue("server/dbPassword", "myuser");
        }
        
        if (!contains("server/dbName")) {
            setValue("server/dbName", "studentsdb");
        }
        
        if (!contains("client/serverHost")) {
            setValue("client/serverHost", "127.0.0.1");
        }
        if (!contains("client/serverPort")) {
            setValue("client/serverPort", 12345);
        }
    }
};
