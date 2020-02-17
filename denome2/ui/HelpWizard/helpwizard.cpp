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

#include "helpwizard.h"
#include "ui_helpwizard.h"

#include <QDebug>

HelpWizard::HelpWizard(QString dialogname, QString rootfolder, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpWizard),
    firstStart(true)
{
    QSize tempSize;

    ui->setupUi(this);

    this->setWindowTitle(dialogname);

    ui->twItems->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twItems->clear();
    ui->tbInfo->clear();

    ui->twItems->setColumnCount(1);

    interpretFolder(nullptr, rootfolder);

    this->ui->twItems->expandAll();

    this->ui->twItems->setMaximumWidth(250);
    tempSize = ui->tbInfo->size();
    tempSize.rwidth() += 450;
    ui->tbInfo->resize(tempSize);

    this->setWindowFlags(Qt::Window);
}

HelpWizard::~HelpWizard()
{
    delete ui;
}

void HelpWizard::setExtFolder(QString extFolder)
{
    this->extFolder = extFolder;

    HelpItem *pluginFolder = new HelpItem(
                new QTreeWidgetItem(this->ui->twItems),
                QString("Plugins"));

    interpretFolder(pluginFolder, extFolder);

    if(pluginFolder->getItem()->childCount() == 0)
    {
        delete pluginFolder;
    }
}


void HelpWizard::show(QString caption)
{
    firstStart = false;

    if(caption.isEmpty())
    {
        QDialog::show();
        return;
    }

    foreach(HelpItem* helpItem, this->allItems)
    {
        if(helpItem->getTopic() == caption)
        {
            ui->twItems->clearSelection();

            QDialog::show();
            on_twItems_itemClicked(helpItem->getItem(), 0);
            return;
        }
    }

}


bool HelpWizard::interpretFolder(HelpItem *folderItem, QString pfad)
{
    QRegExp rx("_[0-9]{2}_");
    QString anzeige;
    QDir *folder = new QDir(pfad);
    QStringList entrys, nameFilter;
    QString entry;
    QFile *file;
    HelpItem* aktFolder=nullptr;
    HelpItem* tempItem=nullptr;
    QString *datas;
    bool foldersFound=false;

    if(!folder->exists())
        return false;

    entrys = folder->entryList(QDir::Filter::AllDirs|QDir::Filter::NoDotAndDotDot,QDir::SortFlag::Name);
    foreach(entry, entrys)
    {
        if(entry.startsWith("."))
            continue;

        anzeige = entry.left(4);

        //Reihenfolge einhalten
        if(rx.exactMatch(anzeige))
        {
            anzeige=entry;
            anzeige.remove(0,4);
        }else{
            anzeige=entry;
        }

        anzeige.replace("_"," ");

        //Verzeichnisknoten erstellen
        if(folderItem)
        {
            aktFolder = new HelpItem(
                        new QTreeWidgetItem(folderItem->getItem()),
                        anzeige);
        }else{
            aktFolder = new HelpItem(
                        new QTreeWidgetItem(this->ui->twItems),
                        anzeige);
        }

        //Unterverzeichnis interpretieren
        if(!interpretFolder(aktFolder, pfad+QString(QDir::separator())+entry)){
            delete aktFolder;
        }else{
            foldersFound = true;
        }
    }

    //Prüfe die Dateien
    nameFilter << "*.html";
    entrys = folder->entryList(nameFilter,QDir::Filter::Files|QDir::Filter::NoDotAndDotDot,QDir::SortFlag::Name);

    if(entrys.isEmpty()){
        if(foldersFound)
            return true;   //Keine Einträge aber volle unterverzeichnisse
        else
            return false;   //dieses Verzeichnis ist leer
    }

    foreach(entry, entrys)
    {
        //Datei auslesen
        file = new QFile(pfad + QDir::separator() + entry);
        file->open(QIODevice::ReadOnly);
        datas = new QString(file->readAll());
        file->close();
        delete file;

        //Dateiendung für die Anzeige entfernen
        entry = entry.left(entry.lastIndexOf("."));

        //Die ersten vier Zeigen betrachten.
        anzeige = entry.left(4);

        //Reihenfolge einhalten
        if(rx.exactMatch(anzeige))
        {
            anzeige=entry;
            anzeige.remove(0,4);
        }

        datas->replace("src=\"./", "src=\"" + pfad + "/");

        datas->replace("src=\"../", "src=\"" + pfad.left(pfad.lastIndexOf(QDir::separator())) + "/");

        anzeige.replace("_"," ");

        //item erstellen
        if(folderItem){
            tempItem = new HelpItem(
                        new QTreeWidgetItem(folderItem->getItem()),
                        anzeige,
                        datas);
        }else{
            tempItem = new HelpItem(
                        new QTreeWidgetItem(this->ui->twItems),
                        anzeige,
                        datas);
        }

        allItems.append(tempItem);
    }

    allItems.append(folderItem);
    return true;
}

void HelpWizard::on_twItems_itemChanged(QTreeWidgetItem *item, int  /*column*/)
{
    HelpItem* helpItem = getHelpItem(item);
    if(!helpItem)
        return;
    if(helpItem->getDatas())
        ui->tbInfo->setHtml(*helpItem->getDatas());
}

HelpItem* HelpWizard::getHelpItem(QTreeWidgetItem *item)
{
    foreach(HelpItem* helpItem, this->allItems)
    {
        if(helpItem->getItem() == item)
        {
            return helpItem;
        }
    }
    return nullptr;
}

void HelpWizard::on_twItems_itemClicked(
        QTreeWidgetItem *item,
        int  /*column*/)
{
    QString * data;
    HelpItem* helpItem = getHelpItem(item);
    if(!helpItem)
        return;

    helpItem->getItem()->setSelected(false);


    while(helpItem)
    {
        data = helpItem->getDatas();    //Check Data

        if(data==nullptr || data->isEmpty())    //if no data available
        {
            if(helpItem->getItem()->childCount() > 0)   //check first Children
            {
                helpItem = getHelpItem(helpItem->getItem()->child(0));
            }
            else
            {
                //if no children available, clear gui
                ui->tbInfo->clear();
                return;
            }
        } else {
            //show data
            helpItem->getItem()->setSelected(true);
            ui->tbInfo->setHtml(*data);
            return;
        }
    }

    //No data found
    ui->tbInfo->clear();
    return;
}
