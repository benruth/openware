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
    void on_slider_customContextMenu(const QPoint &pos);

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
