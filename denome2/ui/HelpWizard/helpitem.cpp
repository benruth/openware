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
