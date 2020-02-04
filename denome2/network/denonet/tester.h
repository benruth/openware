#ifndef TESTER_H
#define TESTER_H

#include "lib/Denonet.h"

using namespace ns_denonet;

class tester : public QObject
{
    Q_OBJECT

    public:
    tester(Denonet* dnet);

    public slots:
    void onMasterVolChanged(int value){
        qDebug() << "... new Master volume: " << value/10.0;
    }

    private:
    Denonet *d;
    int i;
};

#endif // TESTER_H
