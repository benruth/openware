#include "ui_main.h"

#include <QApplication>
#include <QStyleFactory>
#include <QStyle>

#include "SavedSettings.h"

int main(int argc, char *argv[])
{
    bool rs;
    bool restart;


    do{
        QApplication a(argc, argv);

        QString appdata = qEnvironmentVariable("appdata", "");

        shared_ptr<SavedSettings> settings = make_shared<SavedSettings>(appdata + "/denome2/state.sav");

        QVariant v;
        bool darkmode;
        rs = settings->loadSetting(v, "darkmode");
        if(rs)
        {
            darkmode = v.toBool();
        }else{
            darkmode = false;
        }





        if(darkmode == true)
        {
            a.setStyle(QStyleFactory::create("Fusion"));


            QPalette darkPalette = a.palette();
            darkPalette.setColor(QPalette::Window, QColor(53,53,53));
            darkPalette.setColor(QPalette::WindowText, QColor(250,230,200));
            darkPalette.setColor(QPalette::Base, QColor(30,25,25));
            darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
            darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Button, QColor(53,53,53));

            /*
            darkPalette.setColor(QPalette::Light, QColor(73,73,73));
            darkPalette.setColor(QPalette::Midlight, QColor(63,63,63));
            darkPalette.setColor(QPalette::Dark, QColor(33,33,33));
            darkPalette.setColor(QPalette::Mid, QColor(53,53,53));
            darkPalette.setColor(QPalette::Shadow, QColor(23,23,23));
            */
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);



      /*      darkPalette.setColor(QPalette::Inactive, QPalette::Button, QColor(53,53,53));
            darkPalette.setColor(QPalette::Inactive, QPalette::Light, QColor(73,73,73));
            darkPalette.setColor(QPalette::Inactive, QPalette::Midlight, QColor(63,63,63));
            darkPalette.setColor(QPalette::Inactive, QPalette::Dark, QColor(33,33,33));
            darkPalette.setColor(QPalette::Inactive, QPalette::Mid, QColor(53,53,53));
            darkPalette.setColor(QPalette::Inactive, QPalette::Shadow, QColor(23,23,23));
    */

            darkPalette.setColor(QPalette::Disabled, QPalette::Window, QColor(25,25,25));
            darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(25,25,25));
            darkPalette.setColor(QPalette::Disabled, QPalette::Link, QColor(25,25,25));
            darkPalette.setColor(QPalette::Disabled, QPalette::Button,QColor(25,25,25));
            darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(25,25,25));
            darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(25,25,25));
            darkPalette.setColor(QPalette::Disabled, QPalette::Text, Qt::darkGray);



            a.setPalette(darkPalette);


            a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
        }



        ui_main w(settings, darkmode);
        w.show();
        rs = a.exec();

        restart = w.restartRequested();
    }while(restart == true);

    return rs;
}
