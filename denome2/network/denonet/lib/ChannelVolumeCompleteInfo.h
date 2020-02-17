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

#ifndef CHANNELVOLUMECOMPLETEINFO_H
#define CHANNELVOLUMECOMPLETEINFO_H


#include "global.h"
#include <QString>
#include <QList>

/**
 * @brief Contains Infos about one channel
 */
struct ChannelVolumeData
{
    ChannelVolume c;
    QString name;
    double value;
};


/**
 * @brief Stores all channelinformations. No Network-Functions. Only Storage
 */
class ChannelVolumeCompleteInfo
{
private:
    QList<struct ChannelVolumeData> channel;
    bool finalized;

public:
    ChannelVolumeCompleteInfo();

    int addChannel(ChannelVolume c, double value);
    bool contains(ChannelVolume c, double *value);
    bool setVolume(ChannelVolume c, double value);
    void finalize();
    bool isFinale(){ return finalized; }

    void clear();
    int size() { return channel.size(); }

    ChannelVolumeData operator[](int index) { return channel.at(index); }
    ChannelVolumeCompleteInfo &operator=(const ChannelVolumeCompleteInfo& info); //probably obsolete
    bool operator==(const ChannelVolumeCompleteInfo& info); //compare only channel-types, not values

private:
    int indexOf(ChannelVolume c);

};


#endif // CHANNELVOLUMECOMPLETEINFO_H
