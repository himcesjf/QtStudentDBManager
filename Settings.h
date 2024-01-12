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
    }
};
