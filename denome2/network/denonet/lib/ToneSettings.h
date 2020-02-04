#ifndef TONESETTINGS_H
#define TONESETTINGS_H


class ToneSettings
{
public:
    ToneSettings(){
        isOn = true;
        bass = 0;
        treble = 0;
    }
    bool isOn;
    int bass;
    int treble;
};



#endif // TONESETTINGS_H
