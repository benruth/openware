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

#include "ChannelVolumeCompleteInfo.h"

ChannelVolumeCompleteInfo::ChannelVolumeCompleteInfo()
{
    finalized = false;
}

int ChannelVolumeCompleteInfo::addChannel(ChannelVolume c, double value)
{
    if(finalized) return 0;

    int index = indexOf(c);
    if(index >= 0)
        return -1;

    ChannelVolumeData temp;
    temp.c = c;
    temp.name = QString(str_channelvolume[static_cast<int>(c)]);
    temp.value = value;

    channel.append(temp);

    return channel.size();
}

bool ChannelVolumeCompleteInfo::contains(ChannelVolume c, double *value)
{
    int index = indexOf(c);
    if(index >= 0)
    {
        if(value != nullptr)
            *value = channel.at(index).value;

        return true;
    }
    return false;
}

bool ChannelVolumeCompleteInfo::setVolume(ChannelVolume c, double value)
{
    int index = indexOf(c);
    if(index >= 0)
    {
        ChannelVolumeData temp;
        temp = channel.at(index);
        temp.value = value;

        channel.replace(index, temp);
        return true;
    }
    return false;
}

void ChannelVolumeCompleteInfo::finalize()
{
    this->finalized = true;
}

void ChannelVolumeCompleteInfo::clear()
{
    channel.clear();
    finalized = false;
}

ChannelVolumeCompleteInfo &ChannelVolumeCompleteInfo::operator=(const ChannelVolumeCompleteInfo &info)
{
    if(this != &info)
    {
        channel.clear();
        finalized = info.finalized;
        channel = info.channel;
        /*
        for(int i=0; i< info.channel.size(); i++)
        {
            this->channel.append(info.channel.at(i));
        }
        */

    }
    return *this;
}

bool ChannelVolumeCompleteInfo::operator==(const ChannelVolumeCompleteInfo &info)
{
    if(info.channel.size() != this->channel.size())
        return false;

    for(int i=0; i<channel.size(); i++)
    {
        if(info.channel.at(i).c != this->channel.at(i).c)
            return false;
    }

    return true;
}

int ChannelVolumeCompleteInfo::indexOf(ChannelVolume c)
{
    for(int i=0; i<channel.size(); i++)
    {
        if(channel.at(i).c == c)
        {
            return i;
        }
    }
    return -1;
}
