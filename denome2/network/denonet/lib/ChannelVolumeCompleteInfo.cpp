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
