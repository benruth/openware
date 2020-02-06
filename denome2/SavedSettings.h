#ifndef SAVEDSETTINGS_H
#define SAVEDSETTINGS_H

#include <QtCore>
#include <QSettings>
#include <QFont>

class SavedSettings
{
public:
    SavedSettings(const QString &saveFile);


    const QStringList &getKeys(){ return usedKeys; }

    template<class T>
    void saveSetting(T item, const QString &key)
    {
        setting.setValue(key, item);
        if(usedKeys.contains(key) == false)
        {
            usedKeys << key;
        }

    }

    bool loadSetting(QVariant &item, const QString &key)
    {
        bool res;

        res = setting.contains(key);
        if(true == res)
        {
            item = setting.value(key);
        }


        return res;
    }

private:
    QSettings setting;
    QStringList usedKeys;
};

#endif // SAVEDSETTINGS_H
