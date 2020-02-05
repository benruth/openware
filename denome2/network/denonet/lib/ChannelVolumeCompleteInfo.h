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
    ChannelVolumeCompleteInfo &operator=(const ChannelVolumeCompleteInfo& info);
    bool operator==(const ChannelVolumeCompleteInfo& info);

private:
    int indexOf(ChannelVolume c);

};


#endif // CHANNELVOLUMECOMPLETEINFO_H
