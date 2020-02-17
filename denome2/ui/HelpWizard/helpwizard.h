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

#ifndef HELPWIZARD_H
#define HELPWIZARD_H


#include <QDialog>
#include <qfile.h>
#include <qdir.h>

#include "helpitem.h"

namespace Ui {
class HelpWizard;
}

/**
 * Erzeugt ein Dialog, in dem Hilfe-Texte in Form von HTML-Files angezeigt werden.
 * Die Dateien sollten in einem Bestimmten Format in einem Verzeichnis hinterlegt sein.
 * Das Verzeichnis wird über den Konstruktor bekannt gemacht.
 * Mit show() wird das Fenster angezeigt.
 */
class HelpWizard : public QDialog
{
    Q_OBJECT

public:
    explicit HelpWizard(QString dialogname, QString rootfolder, QWidget *parent = 0);
    ~HelpWizard();

    void setExtFolder(QString extFolder);
    bool isFirstStart(){return firstStart;}
public slots:
    void show(QString caption = QString());

private slots:
    void on_twItems_itemChanged(QTreeWidgetItem *item, int  column);

    void on_twItems_itemClicked(QTreeWidgetItem *item, int  column);

private:
    Ui::HelpWizard *ui;

    QList<HelpItem*> allItems;

    bool interpretFolder(HelpItem *folderItem, QString pfad);

    HelpItem* getHelpItem(QTreeWidgetItem* item);

    QString extFolder;

    bool firstStart;
};

#endif // HELPWIZARD_H
