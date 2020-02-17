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

#include "helpitem.h"

HelpItem::HelpItem(QTreeWidgetItem *item, QString topic, QString *datas)
{
    this->item = item;
    this->datas = datas;
    this->topic = topic;

    this->item->setText(0,topic);
}

HelpItem::~HelpItem(){
    if(item)
        delete item;
    item=nullptr;

    if(datas)
        delete datas;
    datas = nullptr;
}

QTreeWidgetItem* HelpItem::getItem()
{
    return this->item;
}

QString* HelpItem::getDatas(){
    return datas;
}

QString HelpItem::getTopic(){
    return topic;
}
