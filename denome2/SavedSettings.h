/*
 * Copyright (C) 2020 Benedikt Ruthenberg
 * https://github.com/benruth
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

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
