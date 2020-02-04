#include "Denonet.h"


#include <QCoreApplication>
#include <QThread>

#include "command_strings.h"

using namespace ns_denonet;


Denonet::Denonet() :
    currentChannel(),
    tempChannel()
{

    com = nullptr;
    volume = 0;
    volumeMax = 980;
    currentState = DenonState::DISCONNECTED;
    currentSType = SoundType::ST_MUSIC;
    currentSurMode = SurroundMode::MS_STEREO;
    currentInput = SI_AUX1;
    eco = ECO_ON;
    isMute = false;
    sleepActive = false;
    menuActive = false;
    drcMode = DRC_Modes::DRC_OFF;
    dimState = DimmerState::DIM_BRIGHT;

    waitingForAnswer = false;

    statusCommands = "PW?\rMV?\rMS?\rCV?\rPSTONE CTRL ?\rPSBAS ?\rPSTRE ?\rECO?\rSI?\rMU?\rMNMEN?\rPSSWR ?\rSTBY?\rDIM ?\r";

    sender.setInterval(600);
    connect(&sender, SIGNAL(timeout()), this, SLOT(senderTimeout()));
    connect(&ping, SIGNAL(timeout()), this, SLOT(onPingTimeout()));
}

Denonet::~Denonet()
{
    if(com)
        delete com;
}

int Denonet::init()
{
    if( com != nullptr) throw 2;    //Call this ony once per object-instaciation

    com = new QTcpSocket(this);
    if(!com)
        return -1;

    connect(com, SIGNAL(disconnected()), this, SLOT(connectionLost()));
    connect(com, SIGNAL(connected()), this, SLOT(denonConnected()));
    connect(com, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));

    return 0;
}

int Denonet::setMasterVolume(double value)
{
    if(com == nullptr) throw 1;

    if(value < 0.0)
        return -1;


    if( value > 98.0)
        return -1;

    int absolute = static_cast<int>(round(value*10.0));



    QByteArray data;
    data.append(QString().sprintf("%03d", absolute));
    if(data.endsWith('0'))
        data.remove(2,1);
    qDebug() << "Set Volume: " << value <<  " - " << data;

    QByteArray message;
    message = "MV";
    message.append(data);
    message.append('\r');

    send(message);

    return -1;
}

int Denonet::setPowerState(DenonState ds)
{
    if(com == nullptr) throw 1;

    switch(ds)
    {
    case DenonState::ON:
        return send("PWON\r");
        //break;


    case DenonState::STANDBY:
        return send("PWSTANDBY\r");
        //break;

    case DenonState::DISCONNECTED:
        disconnectDenon();
        return 0;

    default:
        qDebug() << "unknown Denon-State";

    }

    return -1;
}

int Denonet::setSoundType(SoundType st)
{
    if(com == nullptr) throw 1;

    switch(st)
    {
    case ST_MOVIE:
        return send("MSMOVIE\r");

    case ST_MUSIC:
        return send("MSMUSIC\r");

    case ST_GAME:
        return send("MSGAME\r");

    case ST_DIRECT:
        return send("MSDIRECT\r");

    default:
        qDebug() << "unknown SoundType";
        break;
    }

    return -1;
}

int Denonet::setSurroundMode(SurroundMode sm)
{
    if(com == nullptr) throw 1;

    QByteArray c("MS");

    c.append(str_surroundMode_commands[static_cast<int>(sm)]);
    qDebug() << "sending Surround-Mode request: " << c;
    return send(c);
}

int Denonet::setChannelVolume(ChannelVolume cv, double value)
{
    if(com == nullptr) throw 1;

    QByteArray c("CV");
    c.append(str_channelvolume_command[cv]);
    c.append(' ');
    c.append(parseValueToA(value, 500));

    return send(c);
}

int Denonet::setEcoMode(EcoStates es)
{
    if(com == nullptr) throw 1;

    QByteArray c("ECO");

    switch(es)
    {
    case EcoStates::ECO_ON:
        c.append("ON");
        break;

    case EcoStates::ECO_OFF:
        c.append("OFF");
        break;

    case EcoStates::ECO_AUTO:
        c.append("AUTO");
        break;

    default:
        qDebug() << "unknown Eco-State...";
    }

    return send(c);
}

int Denonet::setSignalInput(SignalInputs si)
{
    if(com == nullptr) throw 1;

    QByteArray c("SI");
    c.append(str_signalinput_command[si]);
    return send(c);
}

int Denonet::setAutoStandby(AutoStandby as)
{
    if(com == nullptr) throw 1;

    QByteArray c("STBY");
    c.append(str_autostandby_command[as]);
    return send(c);
}

int Denonet::sendMenuCommand(MenuControls mc)
{
    if(com == nullptr) throw 1;

    QByteArray c("MN");
    c.append(str_menucontrols_commands[mc]);
    return send(c);

}

int Denonet::setDrcMode(DRC_Modes dm)
{
    if(com == nullptr) throw 1;

    QByteArray c("PS");
    c.append(str_dynamicrange_commands[dm]);
    return send(c);
}

int Denonet::setDimmerState(DimmerState ds)
{
    if(com == nullptr) throw 1;

    QByteArray c("DIM ");
    c.append(str_dimmerstate_commands[ds]);
    return send(c);
}

int Denonet::setToneCtrl(bool on)
{
    if(com == nullptr) throw 1;

    QByteArray c("PSTONE CTRL ");
    if(on)
        c.append("ON\r");
    else
        c.append("OFF\r");
    return send(c);
}

int Denonet::setToneBass(int value)
{
    if(com == nullptr) throw 1;

    if(value < -6)
        return -1;
    if(value > 6)
        return -1;

    QByteArray c("PSBAS ");
    c.append(QString().sprintf("%02d\r", value+50));
    return send(c);
}

int Denonet::setToneTreble(int value)
{
    if(com == nullptr) throw 1;

    if(value < -6)
        return -1;
    if(value > 6)
        return -1;

    QByteArray c("PSTRE ");
    c.append(QString().sprintf("%02d\r", value+50));
    return send(c);
}

int Denonet::setSubwooferState(bool active)
{
    if(com == nullptr) throw 1;

    QByteArray c("PSSWR ");
    if(active)
        c.append("ON\r");
    else
        c.append("OFF\r");
    return send(c);
}

int Denonet::setMute(bool mute)
{
    if(com == nullptr) throw 1;

    QByteArray c("MU");

    if(mute)
        c.append("ON\r");
    else
        c.append("OFF\r");
    return send(c);
}

int Denonet::setSleep(int minutes)
{
    if(com == nullptr) throw 1;

    QByteArray c("SLP");
    if(minutes > 120)
        return -1;

    if(minutes < 0)
        minutes = 0;

    if(minutes == 0)
    {
        c.append("OFF\r");
    }else{
        c.append(QString().sprintf("%03d\r", minutes));
    }

    return send(c);

}

double Denonet::getMasterVolume()
{
    return volume/10.0;
}

double Denonet::getMaxMasterVolume()
{
    return volumeMax/10.0;
}

int Denonet::getChannelVolume(ChannelVolume c, double &value)
{
    double v;
    if(currentChannel.contains(c, &v))
    {
        value = v;
        return 0;
    }
    return -1;

}

int Denonet::getAutoSleepState(QTime &st)
{
    if(sleepActive)
    {
        st = this->sleepTime;
        return QTime::currentTime().secsTo(sleepTime);
    }
    return 0;

}

int Denonet::testConnection()
{
    qDebug() << "Testing Connection";

    if(com == nullptr) throw 1;  //No Init

    if(currentState == DenonState::DISCONNECTED){
        qDebug() << "testConnection: Denon is already disconnected";
        return 0;
    }

    if(com->isOpen())
    {
        com->write("PW?\r");
        if(com->waitForBytesWritten(WAITING_TO))
        {
            waitingForAnswer = true;
            ping.start(PING_TIME);
            return 1;
        }else{
            qDebug() << "testConnection: no Response on send";
            com->close();
        }

    }

    qDebug() << "testConnection: Com not open";

    currentState = DenonState::DISCONNECTED;
    emit statusChanged(currentState);
    return -1;
}

int Denonet::connectToDenon(QString h)
{
    qDebug() << "Connecting to Denon: " << h;

    if(com == nullptr) throw 1;  //No Init

    bool status;

    com->close();

    if ( ! h.isEmpty() )
    {
        host = h;
    }
    if (host.isEmpty())
        return -2;


    com->connectToHost(host, DENON_PORT);       // see also denonConnected()

    status = com->waitForConnected(5000);

    if( status == false )
    {
        com->close();
        currentState = DenonState::DISCONNECTED;
        return -1;
    }


    return 0;
}

int Denonet::waitForConnected(int timeout_ms)
{
    if( timeout_ms <= 0)
        return -1;

    qDebug() << "Waiting for Connection";

    QTime tempTime = QTime::currentTime();
    QTime timeoutTime = tempTime.addMSecs(timeout_ms);

    do
    {
        QThread::msleep(50);
        QCoreApplication::processEvents();

        if(isConnected())
        {
            QCoreApplication::processEvents();
            return 1;
        }

        tempTime = QTime::currentTime();
    }while(tempTime < timeoutTime);


    return 0;
}

void Denonet::disconnectDenon()
{
    if(currentState != DenonState::DISCONNECTED)
    {
        com->close();
    }

    ping.stop();
}

double Denonet::convertAbsToDB(double abs)
{
    return -80.0 + abs;
}

int Denonet::sendStatusRequest(StatusRequests sr)
{
    if(com == nullptr) throw 1;

    QByteArray c(str_statusreq_commands[static_cast<int>(sr)]);
    return send(c);
}



void Denonet::onPingTimeout()
{
    if(waitingForAnswer)
    {
        waitingForAnswer=false;

        qDebug() << "Ping Timeout... no Answer in time";
        qDebug() << "Connection will be closed";

        //No Anwser in time
        com->close();
        currentState = DenonState::DISCONNECTED;
        emit statusChanged(currentState);
        return;
    }

    int rs = testConnection();

    switch (rs) {
    case -1:
        //connection lost
        break;
    case 0:
        //try to reconnect
        connectToDenon();
        ping.start(PING_TIME);
        break;
    case 1:
        // ok... waiting for answer
        break;
    default:
        throw 2;
    }
}

void Denonet::senderTimeout()
{
    qDebug() << "Sender-Timeout called";

    sender.stop();

    if(bufferOut.isEmpty())
    {
        sender.stop();
        return;
    }


    send(bufferOut);

    bufferOut.clear();

}

void Denonet::connectionLost()
{
    qDebug() << "Connection Lost";

    if(currentState != DenonState::DISCONNECTED)
    {
        currentState = DenonState::DISCONNECTED;
        emit statusChanged(currentState);

        waitingForAnswer=false;

        sender.stop();
    }

}

void Denonet::denonConnected()
{
    qDebug() << "Connection set";

    com->write(statusCommands);
    if(com->waitForBytesWritten(WAITING_TO))
    {
        waitingForAnswer = true;

        ping.start(PING_TIME);

        lastWorkingHost = host;
    }else{
        if(currentState != DenonState::DISCONNECTED)
        {
            currentState = DenonState::DISCONNECTED;
            emit statusChanged(currentState);
        }
        com->close();
    }
}

void Denonet::newDataAvailable()
{
    QByteArray c;
    int value=0;
    bool searchSuccess = false;

    ping.start(PING_TIME_LONG);  //reset timer
    waitingForAnswer = false;

    QByteArray data;
    QList<QByteArray> commands;

    QByteArray pendingCommands;

    data = com->readAll();

    do
    {

        commands = data.split('\r');

        for(int c_index = 0; c_index<commands.size(); c_index++)
        {
            c = commands.at(c_index);

            if (c.isEmpty())
            {
                continue;
            }

            qDebug() << "=== New Command: " << c;

            if(c.startsWith("MV"))
            {
                c = c.remove(0,2);
                if(c.startsWith("MAX"))
                {
                    c = c.remove(0,4);      // remove MAX_xx
                    value = parseInteger(c);
                    if(volumeMax != value)
                    {
                        volumeMax = value;
                        emit Denonet::volumeMaxChanged(volumeMax/10.0);
                        qDebug() << "New Max Volume Value " << value;
                    }

                } else {
                    value = parseInteger(c);
                    if(volume != value)
                    {
                        volume = value;
                        qDebug() << "New Volume Value " << value;
                        emit Denonet::volumeChanged(value/10.0);
                    }
                }

                continue;
            }

            if(c.startsWith("PW"))
            {
                c = c.remove(0,2);
                if(c.startsWith("ON"))
                {
                    if(currentState != DenonState::ON)
                    {
                        currentState = DenonState::ON;
                        emit Denonet::statusChanged(currentState);
                        qDebug() << "New State: ON";
                    }
                }else{
                    if(currentState != DenonState::STANDBY)
                    {
                        currentState = DenonState::STANDBY;
                        emit Denonet::statusChanged(currentState);
                        qDebug() << "New State: STANDBY";
                    }
                }

                continue;
            }

            if(c.startsWith("CV"))
            {
                c = c.remove(0,2);

                QByteArray channel;
                QByteArray parameter;

                int index = c.indexOf(' ');
                if(index >= 0){
                    channel = c.left(index);
                    parameter = c.remove(0,index+1);
                }else{
                    channel = c;
                }

                if(channel == "END")
                {
                    // All Channel send
                    tempChannel.finalize();
                    currentChannel = tempChannel;
                    tempChannel.clear();
                    qDebug() << "Channelconfig finalized";
                    emit channelVolumeChanged(this->currentChannel);

                }else{

                    searchSuccess = false;
                    for(int i = 0; i < static_cast<int>(ChannelVolume::CHANNEL_VOL_NUM); i++)
                    {
                        if(channel == str_channelvolume_command[i])
                        {
                            searchSuccess = true;
                            double channelValue = (parseInteger(parameter)-500)/10.0;
                            tempChannel.addChannel(static_cast<ChannelVolume>(i), channelValue);

                            break;
                        }
                    }

                    if(searchSuccess == false)
                        qDebug() << "Unsupported Parameter: Channel Volume - " << c;
                }

                continue;
            }

            if(c.startsWith("PS"))
            {
                c = c.remove(0,2);

                if(c.startsWith("TONE CTRL"))
                {
                    c = c.remove(0,10);
                    if(c == "ON")
                    {
                        toneSettings.isOn = true;
                    }else{
                        toneSettings.isOn = false;
                    }

                    emit toneControlChanged(toneSettings);
                    qDebug() << "Tonesettings changed: " << toneSettings.isOn;
                    continue;
                }

                if(c.startsWith("BAS"))
                {
                    c = c.remove(0,4);
                    toneSettings.bass =  c.toInt() - 50;
                    emit toneControlChanged(toneSettings);
                    qDebug() << "Bass set: " << toneSettings.bass;
                    continue;
                }

                if(c.startsWith("TRE"))
                {
                    c = c.remove(0,4);
                    toneSettings.treble =  c.toInt() - 50;
                    emit toneControlChanged(toneSettings);
                    qDebug() << "Treble set: " << toneSettings.treble;
                    continue;
                }

                if(c.startsWith("SWR"))
                {
                    c = c.remove(0,4);

                    bool newSubState = (c == "ON");

                    if(subwooferStereoActive != newSubState)
                    {
                        subwooferStereoActive = newSubState;
                        qDebug() << "Subwoofer for Stereo changed: " << newSubState;
                        emit subwStereoModeChanged(subwooferStereoActive);

                        send(QByteArray("CV?\r"), true);
                    }

                    continue;
                }

                if(c.startsWith("DRC"))
                {
                    c = c.remove(0,4); //parameter seperated by space

                    searchSuccess = false;
                    for(int i = 0; i < static_cast<int>(DRC_NUMS); i++)
                    {
                        if(c == str_dynamicrange_commands[i])
                        {
                            searchSuccess = true;

                            DRC_Modes newDRC = static_cast<DRC_Modes>(i);
                            if(newDRC != this->drcMode)
                            {
                                this->drcMode = newDRC;
                                qDebug() << "DRC changed: " << this->drcMode;
                                emit drcChanged(this->drcMode);
                            }

                            break;
                        }
                    }

                    if(searchSuccess == false)
                        qDebug() << "Unsupported Parameter: PC DRC - " << c;

                    continue;
                }

                qDebug() << "Unsupported Parameter: PS - " << c;
                continue;
            }

            if(c.startsWith("SS"))
            {
                c = c.remove(0,2);
                if(c == "VCTZMALIM ")
                {
                    c = c.remove(0,10);
                    if(c == "OFF")
                    {
                        if(volumeMax != 980)
                        {
                            volumeMax = 980;
                            emit volumeMaxChanged(98.0);
                            qDebug() << "New Max Volume Value " << volumeMax;
                        }
                    }else{
                        bool ok;
                        int newValue = c.toInt(&ok) * 10;

                        if(!ok)
                        {
                            qDebug() << "invalid conversion to int... MaxVolume";
                            continue;
                        }

                        if(newValue != volumeMax)
                        {
                            volumeMax = newValue;
                            emit volumeMaxChanged(newValue/10);
                            qDebug() << "New Max Volume Value " << volumeMax;
                        }

                    }
                    continue;
                }

                qDebug() << "Unsupported Parameter: SS - " << c;
                continue;
            }

            if(c.startsWith("ECO"))
            {
                EcoStates ecoTemp;
                c = c.remove(0,3);
                if(c == "ON")
                {
                    ecoTemp = ECO_ON;
                }else if(c == "AUTO")
                {
                    ecoTemp = ECO_AUTO;
                }else{
                    ecoTemp = ECO_OFF;
                }

                if(ecoTemp != eco)
                {
                    eco = ecoTemp;

                    qDebug() << "Eco-Mode changed: " << QString(str_ecostates[eco]);
                    emit ecoModeChanged(eco);
                }

                continue;
            }

            if(c.startsWith("SI"))
            {
                c = c.remove(0,2);

                SignalInputs tempInput;

                searchSuccess = false;
                for(int i = 0; i< static_cast<int>(SI_NUMS); i++)
                {
                    if(c == str_signalinput_command[i])
                    {
                        searchSuccess = true;

                        tempInput = static_cast<SignalInputs>(i);
                        if(tempInput != currentInput)
                        {
                            currentInput = tempInput;
                            qDebug() << "Signal-Input changed: " << str_signalinputs[i];
                            emit signalInputChanged(currentInput);
                            break;
                        }
                    }
                }

                if(searchSuccess == false)
                    qDebug() << "Unsupported Parameter: Signal Input - " << c;

                continue;
            }

            if(c.startsWith("MS"))
            {
                c = c.remove(0,2);

                searchSuccess = false;

                for(int i = 0; i < static_cast<int>(SoundType::STYPE_NUM); i++)
                {
                    if(c.startsWith(str_soundtypes[i]))
                    {
                        if( currentSType != static_cast<SoundType>(i) )
                        {
                            currentSType = static_cast<SoundType>(i);
                            emit soundTypeChanged(currentSType);
                            qDebug() << "SoundType changed: " << QString(str_soundtypes[i]);
                            searchSuccess = true;
                            break;
                        }
                    }
                }

                searchSuccess = false;
                if( ! searchSuccess)
                {
                    for(int i = 0; i < static_cast<int>(SurroundMode::MS_Nums); i++)
                    {
                        if(c == str_surroundMode_commands[i])
                        {
                            searchSuccess = true;
                            if( currentSurMode != static_cast<SurroundMode>(i) )
                            {
                                currentSurMode = static_cast<SurroundMode>(i);
                                emit surroundModeChanged(currentSurMode);
                                qDebug() << "Surround-Mode changed: " << QString(str_surroundMode_commands[i]);
                                break;
                            }
                        }
                    }
                }

                if(searchSuccess == false)
                    qDebug() << "Unsupported Parameter: MS - " << c;
                continue;
            }

            if(c.startsWith("MN"))
            {
                c = c.remove(0,2);

                bool newMenuActive = (c=="MEN ON");

                if(this->menuActive != newMenuActive)
                {
                    menuActive = newMenuActive;
                    emit menuActiveChanged(menuActive);
                }

                continue;
            }

            if(c.startsWith("MU"))
            {
                c = c.remove(0,2);
                bool newMute = (c=="ON");

                if(newMute != this->isMute)
                {
                    qDebug() << "Mute changed: " << newMute;
                    isMute = newMute;
                    emit muteChanged(isMute);
                }
                continue;
            }

            if(c.startsWith("SLP"))
            {
                c = c.remove(0,3);
                if(c == "OFF")
                {
                    if(sleepActive)
                    {
                        sleepActive = false;
                        qDebug() << "Sleeptimer deactivated";
                        emit sleepCanceled();
                    }
                }else{
                    int sleepValue = c.toInt();
                    sleepActive = true;
                    sleepTime = QTime::currentTime().addSecs(sleepValue*60);
                    qDebug() << "Sleeptimer activated. Sleep at " << sleepTime;
                    emit sleepSet(sleepValue, sleepTime);
                }
                continue;
            }

            if(c.startsWith("STBY"))
            {
                AutoStandby tempStby;
                c = c.remove(0,4);
                searchSuccess = false;

                for(int i=0; i < ASTBY_NUM; i++)
                {
                    if(c == str_autostandby_command[i])
                    {
                        searchSuccess = true;
                        tempStby = static_cast<AutoStandby>(i);

                        if(tempStby != autoStandby)
                        {
                            autoStandby = tempStby;
                            qDebug() << "Auto-Standby changed: " << autoStandby;
                            emit autoStandbyChanged(autoStandby);
                        }

                        break;
                    }
                }

                if(searchSuccess == false)
                    qDebug() << "Unsupported Parameter: Autostandby - " << c;

                continue;
            }

            if(c.startsWith("DIM"))
            {
                c = c.remove(0,4);

                searchSuccess = false;
                for(int i = 0; i < static_cast<int>(DIM_NUMS); i++)
                {
                    if(c == str_dimmerstate_commands[i])
                    {
                        searchSuccess = true;

                        DimmerState newState = static_cast<DimmerState>(i);
                        if(this->dimState != newState)
                        {
                            this->dimState = newState;
                            qDebug() << "Dimmer-State changed: " << str_dimmerstate[i];
                            emit dimmerStateChanged(newState);
                        }

                        continue;
                    }
                }

                if(searchSuccess == false)
                    qDebug() << "Unsupported Parameter: Dimmer - " << c;

                continue;
            }

            qDebug() << "Unknown Command: " << c;

        } // foreach command

        data = com->readAll();
    }while( ! data.isEmpty());

}

int Denonet::parseInteger(QByteArray input)
{
    if(input.size()==2)
        input.push_back('0');

    return input.toInt();
}

QByteArray Denonet::parseValueToA(double value, int zero)
{
    /**
     * zero = 500 wenn   50 = 0 db
     * 0.0 db -> 0+500 -> 50
     * 0.5 db -> 5+500 -> 505
     * -10.5 db -> -105+500 -> 395
     * 12.0 db -> 120+500 -> 62
     */

    QByteArray v;

    value *= 10.0;
    v = QByteArray::number(static_cast<int>(value)+zero);

    if(v.endsWith('0'))
        v.remove(v.length()-1,1);

    return v;
}

int Denonet::send(QByteArray commands, bool delayed)
{
    if(commands.isEmpty())return -1;

    if(commands.endsWith('\r') == false)
    {
        commands.append('\r');
    }

    if(delayed)
    {
        bufferOut.append(commands);
        if(sender.isActive())
        {
        }else{
            sender.start();
        }
        return 0;
    }



    QList<QByteArray> toSend = commands.split('\r');
    QByteArray c;


    for(int i = 0; i<toSend.size(); i++)
    {
        c = toSend.at(i);
        if(c.isEmpty())
            continue;

        qDebug() << "=== Send command: " << c;

        c += '\r';

        com->write(c);

        if( ! com->waitForBytesWritten(WAITING_TO))
        {
            com->close();
            if(currentState != DenonState::DISCONNECTED)
            {
                currentState = DenonState::DISCONNECTED;
                emit statusChanged(currentState);
            }
            sender.stop();
            return -1;
        }


        /*
        if(c.startsWith("MN"))
        {
            if(c.startsWith("MNMEN") || c.startsWith("MNZST"))
            {

            } else {
                continue;   //Command only
            }
        }


        // Some commands are invalid in specific cases.

        waitingForAnswer = true;
        ping.start(PING_TIME);
        */
    }

    return 0;
}
