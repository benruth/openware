#ifndef HELPWIZARD_H
#define HELPWIZARD_H

/**********************************************************
 *
 * Last Edit:   25.02.16
 * Author:      Benedikt Ruthenberg
 *
 * Purpose:
 *  Loads and disply html-files within directory-tree.
 *  Used for Manual-Display and further informations.
 */


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
