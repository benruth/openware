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

#ifndef HELPITEM_H
#define HELPITEM_H


#include <QTreeWidgetItem>

/**
 * Beschreibt eine Informationseinheit, die HTML als Infotext beinhaltet kann
 */
class HelpItem
{
public:
    explicit HelpItem(QTreeWidgetItem *item, QString topic, QString *datas=nullptr);

    ~HelpItem();

    QTreeWidgetItem *getItem();
    QString* getDatas();
    QString getTopic();

signals:

public slots:

private:
    QTreeWidgetItem *item;
    QString *datas;
    QString topic;
};

#endif // HELPITEM_H
