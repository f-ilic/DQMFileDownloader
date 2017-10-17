#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QSettings>


class SettingsManager
{
public:
    static void writeSettings(QString key, QString value) {
        QSettings settings("./settings.ini", QSettings::IniFormat);
        settings.setValue(key, value);
    }

    static QString getSetting(QString key) {
        QSettings settings("./settings.ini", QSettings::IniFormat);
        return settings.value(key).toString();
    }

private:
    SettingsManager();
};

#endif // SETTINGSMANAGER_H
