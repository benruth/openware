#ifndef DENONET_H
#define DENONET_H

#include <QTcpSocket>
#include <QTimer>
#include <QTime>

#include "global.h"
#include "ChannelVolumeCompleteInfo.h"
#include "ToneSettings.h"

namespace ns_denonet
{


    class Denonet : public QObject

    {
        Q_OBJECT

    public:
        Denonet();
        ~Denonet();

        /**
         * @brief init initiate Socket and Signal-Slots. Call exactly once after instaciation
         * @return 0 - ok; -1 - error
         */
        int init();

        /// ------------------------------------------------------


        /**
         * @brief sendStatusRequest send a status-request. the device have to answer. The return of this methode is no garantie, that the status is send. QT-Events will be generated, only if the new Value has changed.
         * @param sr Defined status request
         * @return 0- ok; -1 error
         */
        int sendStatusRequest(StatusRequests sr);
        int sendMenuCommand(MenuControls mc);
        int setMasterVolume(double value);
        int setPowerState(DenonState ds);
        int setSoundType(SoundType st);
        int setSurroundMode(SurroundMode sm);
        int setChannelVolume(ChannelVolume c, double value);
        int setEcoMode(EcoStates es);
        int setSignalInput(SignalInputs si);
        int setAutoStandby(AutoStandby as);
        int setDrcMode(DRC_Modes dm);
        int setDimmerState(DimmerState ds);
        int setToneCtrl(bool on);
        int setToneBass(int value);
        int setToneTreble(int value);
        int setSubwooferState(bool active);
        int setMute(bool mute);

        /**
         * @brief setSleep request Auto-Sleep in x minutes
         * @param minutes [0,120] - 0: off
         * @return 0 on success
         */
        int setSleep(int minutes = 0);


        /// ------------------------------------------------------
        /* The following Methods are Getter-Functions for the current
         * saved States inside this Object. No garanty that those are the
         * actual values. The actual devise-state could change anytime
         */

        double getMasterVolume();
        double getMaxMasterVolume();
        DenonState getState() {return currentState; }
        SurroundMode getSurroundMode() { return currentSurMode; }
        EcoStates getEcoState() { return eco; }
        SignalInputs getSignalInput() { return currentInput; }
        AutoStandby getStandbyState() { return autoStandby; }
        bool isMenuActive() { return menuActive; }
        DRC_Modes getDynamicRangeCompression() { return drcMode; }
        ToneSettings getToneSettings() { return toneSettings; }
        bool isSubwooferStereoActive() { return subwooferStereoActive; }
        DimmerState getDimmerState() { return dimState; }
        /**
         * @brief getAutoSleepState get AutoSleep-State
         * @param sleepTime if autosleep is active, sleepTime contains the target-Time. Only if return isn't zero
         * @return countdown in seconds
         */
        int getAutoSleepState(QTime &sleepTime);

        /**
         * @brief getChannelVolume get volume-value of a specific channel
         * @param c target-channel
         * @param value contains value on return, if channel is supported
         * @return 0: ok, -1: channel not supported
         */
        int getChannelVolume(ChannelVolume c, double &value);


        /// ------------------------------------------------------
        /* Network-specific Methods. */


        /**
         * @brief testConnection Only tries to send a Status-Request. Check Signals for more info
         * @return  0 on Not Connected
         *          -1 on Error (No Connection)
         *          1 OK
         */
        int testConnection();

        /**
         * @brief connectToDenon establish new connection, current connection will be closed
         * @param h name/address of host.
         * @return 0 - ok, -1 - no connection, -2 - no Host
         */
        int connectToDenon(QString h = QString());

        bool isConnected() {  return currentState != DenonState::DISCONNECTED; }

        /**
         * @brief waitForConnected returns if successfully connected or timeout
         * @param timeout_ms timeout in ms
         * @return 0 - not connected; -1 - timeout incorrect; 1 - connected
         */
        int waitForConnected(int timeout_ms);

        void disconnectDenon();



        /// ------------------------------------------------------
        /* public Helper-Methods */

        /**
         * @brief convertAbsToDB convert Abolute Volume Value into dB-Value
         * @param abs Value to Convert [0,100]
         * @return [-80 , 20]  -80 := '-'
         */
        static double convertAbsToDB(double abs);





    signals:
        void volumeChanged(double value);
        void volumeMaxChanged(double value);
        void statusChanged(DenonState status);
        void soundTypeChanged(SoundType t);
        void surroundModeChanged(SurroundMode m);
        void channelVolumeChanged(ChannelVolumeCompleteInfo cvi);
        void toneControlChanged(ToneSettings ts);
        void subwStereoModeChanged(bool isActive);
        void ecoModeChanged(EcoStates e);
        void signalInputChanged(SignalInputs si);
        void muteChanged(bool mute);
        void autoStandbyChanged(AutoStandby as);
        void sleepSet(int min, QTime time);
        void sleepCanceled();
        void menuActiveChanged(bool isActive);
        void drcChanged(DRC_Modes m);
        void dimmerStateChanged(DimmerState ds);


    public slots:
        //Timeout-Management
        void onPingTimeout();

        //commands should be send every 50ms
        void senderTimeout();

        // Network-interface
        void connectionLost();
        void denonConnected();
        void newDataAvailable();





    private:
        /// Connection
        QTcpSocket *com;
        QTimer ping;
        QTimer sender;

        QByteArray statusCommands;

        //QByteArray bufferIn;
        QByteArray bufferOut;

        QString host, lastWorkingHost;
        bool waitingForAnswer;  //set when command is send. Reset on incomming

        /// Denon states
        int volume;     // 3-digit representation of absolute value (value x 10)
        int volumeMax;
        DenonState currentState;
        SoundType currentSType;
        SurroundMode currentSurMode;
        ChannelVolumeCompleteInfo currentChannel;   //Last finalized
        ChannelVolumeCompleteInfo tempChannel;      //new Channel Config, may not be final
        bool toneControl;
        ToneSettings toneSettings;
        bool subwooferStereoActive;
        EcoStates eco;
        SignalInputs currentInput;
        bool isMute;
        AutoStandby autoStandby;
        bool sleepActive;
        QTime sleepTime;
        bool menuActive;
        DRC_Modes drcMode;
        DimmerState dimState;

    private:
        void interpretData();


    private: //Helper
        int parseInteger(QByteArray input);
        int send(QByteArray c, bool delayed = false);
        QByteArray parseValueToA(double value, int zero);


    };

}

#endif // DENONET_H
