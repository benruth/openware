#include "tester.h"

tester::tester(Denonet *dnet)
{
    this->d = dnet;
    i=53;
    connect(d, SIGNAL(volumeChanged(int)), this, SLOT(onMasterVolChanged(int)));
}
