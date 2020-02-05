#ifndef CHANNELSLIDER_H
#define CHANNELSLIDER_H

#include <QObject>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>

#include <network/denonet/lib/Denonet.h>

using namespace ns_denonet;

class ChannelSlider : public QObject
{
    Q_OBJECT

public:
    ChannelSlider(QWidget* parent, QGridLayout* layout, ChannelVolumeData &channel, Qt::Orientation o, Denonet *com);
    ~ChannelSlider();

    void reorderItems(Qt::Orientation o);
    void changeVolume(double v);
    ChannelVolume getChannel() { return type.c; }


public slots:
    void on_slider_valueChanged(int value);
    void on_slider_actionTriggered(int action);

private:
    ChannelVolumeData type;

    QGridLayout * layout;
    QSlider *slider;
    QLabel *label;
    QLineEdit *lineEdit;
    Qt::Orientation orientation;
    QBoxLayout::Direction layoutDirection;

    bool actionWasTriggered;

    Denonet *com;

};

#endif // CHANNELSLIDER_H
