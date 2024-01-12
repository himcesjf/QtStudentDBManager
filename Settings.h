#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStandardPaths>
#include <QFileInfo>

class Settings : public QSettings {
public:
    Settings() : QSettings(getIniFilePath(), QSettings::IniFormat) {
        setDefaults();
    }

private:
    static QString getIniFilePath() {
        QString configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        return configPath + "/studentmanager.ini";
    }

    void setDefaults() {
        // Set default values if they don't exist
        if (!contains("server/host")) {
            setValue("server/host", "localhost"); // Replace with actual default IP
        }
        if (!contains("server/port")) {
            setValue("server/port", 12345); // Replace with actual default port number
        }
    }
};

#endif // SETTINGS_H
