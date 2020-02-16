#ifndef HELPITEM_H
#define HELPITEM_H

/**********************************************************
 *
 * Last Edit:   25.02.16
 * Author:      Benedikt Ruthenberg
 *
 * Purpose:
 *  Stores GUI-Informations and Help-File-Content (html-file)
 */


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
