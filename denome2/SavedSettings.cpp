#include "SavedSettings.h"

SavedSettings::SavedSettings(const QString &saveFile)
    : setting(saveFile, QSettings::IniFormat)
{

    this->usedKeys = setting.allKeys();

}
