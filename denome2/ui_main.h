#ifndef UI_MAIN_H
#define UI_MAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QInputDialog>
#include <QToolButton>
#include <QSlider>
#include <QElapsedTimer>

#include "network/denonet/lib/Denonet.h"

#define DELAY_MS_PER_TICK 200 //ms
#define DELAY_SECONDS(x) (static_cast<int>(x*1000/DELAY_MS_PER_TICK))

using namespace std;
using namespace ns_denonet;

QT_BEGIN_NAMESPACE
namespace Ui { class ui_main; }
QT_END_NAMESPACE

class ui_main : public QMainWindow
{
    Q_OBJECT

public:
    ui_main(QWidget *parent = nullptr);
    ~ui_main();

public slots:
    void onVolumeChanged(double v);
    void onMaxVolumeChanged(double v);
    void onStatusChanged(DenonState s);
    void onChannelVolumeChanged(ChannelVolumeCompleteInfo info);
    void onToneControlChanged(ToneSettings s);
    void onSignalInputChanged(SignalInputs si);
    void onSubwooferStereoModeChanged(bool active);
    void onMuteChanged(bool mute);
    void onEcoModeChanged(EcoStates e);
    void onSleepSet(int min, QTime time);
    void onSleepCanceled();
    void onDimmerStateChanged(DimmerState d);



private slots:
    void onPowerButtonClicked();
    void onHostInputDialogEnter();

    void on_hsBass_valueChanged(int value);

    void on_hsTreble_valueChanged(int value);

    void on_hsSubwoofer_valueChanged(int value);

    void on_hsSubwoofer_customContextMenuRequested(const QPoint &pos);

    void on_hsBass_customContextMenuRequested(const QPoint &pos);

    void on_hsTreble_customContextMenuRequested(const QPoint &pos);

    void on_hsSubwoofer_actionTriggered(int action);

    void on_hsBass_actionTriggered(int action);

    void on_hsTreble_actionTriggered(int action);

    void on_gbAudioSettings_clicked(bool checked);

    void on_gbSubwooferSettings_clicked(bool checked);

    void on_dial_sliderPressed();

    void on_dial_sliderReleased();

    void on_dial_valueChanged(int value);

    void on_pbMute_clicked(bool checked);

    void on_actionMusik_triggered();

    void on_actionMovie_triggered();

    void on_actionGame_triggered();

    void on_actionDirect_triggered();

    void on_actionEcoEin_triggered();

    void on_actionEcoAus_triggered();

    void on_actionEcoAuto_triggered();

    void on_actionSleep_triggered();

    void on_delayTimer();

    void on_actionDimDark_triggered();

    void on_actionDimDim_triggered();

    void on_actionDimBright_triggered();

    void on_actionDimOff_triggered();

private:
    Ui::ui_main *ui;
    Denonet com;
    int maxVolume;
    bool connected;
    double lastVolumeValue;
    QElapsedTimer lastVolumeChange;
    QTimer delayTimer;

    bool sleepSetRecently;


    enum DelayObjects {
        DL_SLEEP_NOTICE,
        DL_SLEEP_SET_TIMER,

        DELAY_NUMS
    };
    int delayTicks[DELAY_NUMS];

    enum SliderObject {
        SL_SUBWOOFER,
        SL_BASS,
        SL_TREBLE,
        SL_VOLUME,
        SLIDER_NUM
    };
    bool sliderActionTriggered[SLIDER_NUM];
    bool incommingChanges[SLIDER_NUM];


    int sleepSelected;
    int sleepMinutes[6];



    /// UI-Elemente
    QToolButton ui_Power;
    QInputDialog ui_hostInputDialog;
    QLabel ui_sleepStatus;








    int connectDenon(QString host);

    void closeEvent(QCloseEvent *event);

    void enableGui(bool enable);
    void initInputSignals();

    void sliderCustomContextMenu(const QPoint &pos, QSlider *slider);

    void setSleepSetRecently();
    void resetSleepSetRecently();



};
#endif // UI_MAIN_H
