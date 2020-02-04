#include <QCoreApplication>
#include "lib/Denonet.h"
#include "tester.h"


#include <QThread>
#include <QTextStream>

using namespace ns_denonet;
using namespace std;



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream cin(stdin);
    QTextStream cout(stdout);
    QString text;

    Denonet d;
    d.init();

    tester t(&d);

    d.connectToDenon("192.168.2.39");

    qDebug() << "... waiting";
    int status = d.waitForConnected(1000);

    if(status != 1)
    {
        qDebug() << "...NOT connected";

    }else{
        qDebug() << "...connected";
    }


/*
    do{
        cin >> text;
        if(text == "CV")
        {
             d.checkChannelValues();
        }
    }while(text != "q");
*/
    //d.disconnectDenon();


    return a.exec();
}
