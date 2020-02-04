#include "ui_main.h"
#include "ui_ui_main.h"

#include <QMessageBox>
#include <QStyleFactory>

ui_main::ui_main(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui_main)
    , sleepMinutes{10, 20, 30, 60, 90, 120}
    , ui_sleepStatus(this)
{
    ui->setupUi(this);
    com.init();

    /// standard inits
    connected = false;
    maxVolume = 98;
    sleepSelected = -1;
    delayTimer.setInterval(DELAY_MS_PER_TICK);
    delayTimer.start();
    connect(&delayTimer, SIGNAL(timeout()),
            this, SLOT(on_delayTimer()));

    sleepSetRecently = false;



    for(int i = 0; i<SLIDER_NUM; i++){
        sliderActionTriggered[i] = false;
        incommingChanges[i] = false;
    }

    lastVolumeValue = -1.0;
    lastVolumeChange.start();

    /// GUI setups
    enableGui(false);
    initInputSignals();

    ui->dial->setMaximum(maxVolume*2);

    ui->gbVolumeAbsolute->setMinimumHeight(90);
    ui->gbVolumeAbsolute->setMinimumWidth(200);
    ui->gbVolumeDB->setMinimumHeight(90);
    ui->gbVolumeDB->setMinimumWidth(200);
    ui->cbSource->setMinimumWidth(150);
    ui->pbClose->setMinimumHeight(25);
    ui->pbMute->setMinimumHeight(25);
    this->setMinimumWidth(500);


    ui->actionEcoAus->setCheckable(true);
    ui->actionEcoEin->setCheckable(true);
    ui->actionEcoAuto->setCheckable(true);

    ui->actionDimDim->setCheckable(true);
    ui->actionDimOff->setCheckable(true);
    ui->actionDimDark->setCheckable(true);
    ui->actionDimBright->setCheckable(true);


    onDimmerStateChanged(com.getDimmerState());



    /// toolbar init
    ui->toolBar->setIconSize(QSize(32,32));

    // == left side of toolbar
    ui->toolBar->addWidget(&ui_sleepStatus);
    ui_sleepStatus.setAlignment(Qt::AlignCenter);




    // == right side of Toolbar
    QWidget *temp = new QWidget();
    temp->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->toolBar->addWidget(temp);

    QAction* updateAction = ui->actionUpdate;
    ui->toolBar->addAction(updateAction);
    ui->toolBar->addSeparator();



    // Connection-Status
    ui->toolBar->addWidget(&ui_Power);
    ui_Power.setIcon(QIcon(":/diverse/StateDisconnected"));
    connect(&ui_Power, SIGNAL(clicked()), this, SLOT(onPowerButtonClicked()));




    /// Dockwidget management
    ui->actionAudio->setCheckable(true);
    ui->actionAudio->setChecked(true);
    connect(ui->actionAudio, SIGNAL(triggered(bool)), ui->dwSoundSettings, SLOT(setVisible(bool)));
    connect(ui->dwSoundSettings, SIGNAL(visibilityChanged(bool)), ui->actionAudio, SLOT(setChecked(bool)));







    /// Menu-Actions
    connect(ui->actionBeenden, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));

    /// Connection-Management
    //ui_hostInputDialog.setParent(this);
    ui_hostInputDialog.setLabelText("Host-Adresse:");
    ui_hostInputDialog.setTextValue("192.168.2.39");

    connect(ui->actionVerbindung, SIGNAL(triggered()), &ui_hostInputDialog, SLOT(open()));
    connect(&ui_hostInputDialog, SIGNAL(accepted()), this, SLOT(onHostInputDialogEnter()));




}

ui_main::~ui_main()
{
    delete ui;
}

void ui_main::onVolumeChanged(double v)
{
    if(sliderActionTriggered[SL_VOLUME] == false)
        ui->dial->setValue(static_cast<int>(v*2));
}

void ui_main::onMaxVolumeChanged(double v)
{
    this->maxVolume = static_cast<int>(v);
    ui->dial->setMaximum(maxVolume*2);
}

void ui_main::onStatusChanged(DenonState s)
{
    if(s == DenonState::ON)
        enableGui(true);

    switch (s)
    {
    case DenonState::ON:
        ui_Power.setIcon(QIcon(":/diverse/StateON"));
        break;

    case DenonState::STANDBY:
        ui_Power.setIcon(QIcon(":/diverse/StateStandby"));
        break;

    case DenonState::DISCONNECTED:
        ui_Power.setIcon(QIcon(":/diverse/StateDisconnected"));
        enableGui(false);
        break;
    }
}


void ui_main::onChannelVolumeChanged(ChannelVolumeCompleteInfo info)
{
    double v;
    if(info.contains(CH_SUBWOOFER, &v))
    {
        int step = static_cast<int>(v*2.0);
        if(sliderActionTriggered[SL_SUBWOOFER] == false)
        {
            ui->hsSubwoofer->setValue(step);
        }
    }
}

void ui_main::onToneControlChanged(ToneSettings s)
{
    ui->gbAudioSettings->setChecked(s.isOn);

    if(s.isOn)
    {
        if(sliderActionTriggered[SL_BASS] == false)
            ui->hsBass->setValue(s.bass);
        if(sliderActionTriggered[SL_TREBLE] == false)
            ui->hsTreble->setValue(s.treble);
    }
}

void ui_main::onSignalInputChanged(SignalInputs si)
{
    if(si >= SI_NUMS)
        return;

    ui->cbSource->setCurrentIndex(static_cast<int>(si));
}

void ui_main::onSubwooferStereoModeChanged(bool active)
{
    ui->gbSubwooferSettings->setChecked(active);
}

void ui_main::onMuteChanged(bool mute)
{
    ui->pbMute->setChecked(mute);
}

void ui_main::onEcoModeChanged(EcoStates e)
{

    switch(e)
    {
    case EcoStates::ECO_ON:
        ui->actionEcoEin->setChecked(true);
        ui->actionEcoAuto->setChecked(false);
        ui->actionEcoAus->setChecked(false);
        break;

    case EcoStates::ECO_OFF:

        ui->actionEcoAus->setChecked(true);
        ui->actionEcoAuto->setChecked(false);
        ui->actionEcoEin->setChecked(false);
        break;

    case EcoStates::ECO_AUTO:
        ui->actionEcoAuto->setChecked(true);
        ui->actionEcoEin->setChecked(false);
        ui->actionEcoAus->setChecked(false);
        break;

    }
}

void ui_main::onSleepSet(int min, QTime time)
{
    if(min <= 0)
    {
        delayTicks[DL_SLEEP_NOTICE] = DELAY_SECONDS(5);
        ui_sleepStatus.setText("Sleep Aus");
        sleepSelected = -1;
        return;
    } else {

        delayTicks[DL_SLEEP_NOTICE] = 0;
        ui_sleepStatus.setText(QString().sprintf("Sleep: %d min\n", min) + time.toString("hh:mm"));
        //ui->statusbar->showMessage(QString().sprintf("Sleep aktiviert: %d Minuten", min), 5000);
    }

}

void ui_main::onSleepCanceled()
{

    ui_sleepStatus.setText("Sleep Aus");
    delayTicks[DL_SLEEP_NOTICE] = DELAY_SECONDS(5);
    sleepSelected = -1;

    resetSleepSetRecently();

    ui->statusbar->showMessage(QString().sprintf("Sleep deaktiviert"), 5000);
}

void ui_main::onDimmerStateChanged(DimmerState d)
{

    switch(d)
    {
    case DimmerState::DIM_DARK:
        ui->actionDimDim->setChecked(false);
        ui->actionDimOff->setChecked(false);
        ui->actionDimDark->setChecked(true);
        ui->actionDimBright->setChecked(false);
        break;
    case DimmerState::DIM_DIM:
        ui->actionDimDim->setChecked(true);
        ui->actionDimOff->setChecked(false);
        ui->actionDimDark->setChecked(false);
        ui->actionDimBright->setChecked(false);
        break;
    case DimmerState::DIM_BRIGHT:
        ui->actionDimDim->setChecked(false);
        ui->actionDimOff->setChecked(false);
        ui->actionDimDark->setChecked(false);
        ui->actionDimBright->setChecked(true);
        break;
    case DimmerState::DIM_OFF:
        ui->actionDimDim->setChecked(false);
        ui->actionDimOff->setChecked(true);
        ui->actionDimDark->setChecked(false);
        ui->actionDimBright->setChecked(false);
        break;
    default:
        qDebug() << "DimmerStateChanged: unknown State";

    }

}

void ui_main::onHostInputDialogEnter()
{
    QString input = ui_hostInputDialog.textValue();
    qDebug() << "connect to " << input;
    if(connectDenon(input) != 0)
    {
        QMessageBox::critical(this, "Fehler", "Verbindung konnte nicht hergstellt werden");
        ui_hostInputDialog.open();
    }

}

void ui_main::onPowerButtonClicked()
{
    DenonState s = com.getState();

    switch (s)
    {
    case DenonState::ON:
        com.setPowerState(DenonState::STANDBY);
        break;

    case DenonState::STANDBY:
        com.setPowerState(DenonState::ON);
        break;

    case DenonState::DISCONNECTED:
        ui_hostInputDialog.open();
        break;
    }
}

int ui_main::connectDenon(QString host)
{
    int rs;

    rs = com.connectToDenon(host);

    if(rs != 0)
    {
        qDebug() << "Cant connect device - " << host;
        return rs;
    }

    connect(&com, SIGNAL(volumeChanged(double)),
            this, SLOT(onVolumeChanged(double)));
    connect(&com, SIGNAL(volumeMaxChanged(double)),
            this, SLOT(onMaxVolumeChanged(double)));
    connect(&com, SIGNAL(statusChanged(DenonState)),
            this, SLOT(onStatusChanged(DenonState)));
    connect(&com, SIGNAL(signalInputChanged(SignalInputs)),
            this, SLOT(onSignalInputChanged(SignalInputs)));
    connect(&com, SIGNAL(toneControlChanged(ToneSettings)),
            this, SLOT(onToneControlChanged(ToneSettings)));
    connect(&com, SIGNAL(channelVolumeChanged(ChannelVolumeCompleteInfo)),
            this, SLOT(onChannelVolumeChanged(ChannelVolumeCompleteInfo)));
    connect(&com, SIGNAL(subwStereoModeChanged(bool)),
            this, SLOT(onSubwooferStereoModeChanged(bool)));
    connect(&com, SIGNAL(muteChanged(bool)),
            ui->pbMute, SLOT(setChecked(bool)));
    connect(&com, SIGNAL(ecoModeChanged(EcoStates)),
            this, SLOT(onEcoModeChanged(EcoStates)));
    connect(&com, SIGNAL(sleepSet(int, QTime)),
            this, SLOT(onSleepSet(int, QTime)));
    connect(&com, SIGNAL(sleepCanceled()),
            this, SLOT(onSleepCanceled()));
    connect(&com, SIGNAL(dimmerStateChanged(DimmerState)),
            this, SLOT(onDimmerStateChanged(DimmerState)));


    connected = true;
    return 0;
}

void ui_main::closeEvent(QCloseEvent *event)
{
    if(connected)
    {
        disconnect(&com, SIGNAL(volumeChanged(double)),
                this, SLOT(onVolumeChanged(double)));
        disconnect(&com, SIGNAL(volumeMaxChanged(double)),
                this, SLOT(onMaxVolumeChanged(double)));
        disconnect(&com, SIGNAL(statusChanged(DenonState)),
                this, SLOT(onStatusChanged(DenonState)));
        disconnect(&com, SIGNAL(signalInputChanged(SignalInputs)),
                this, SLOT(onSignalInputChanged(SignalInputs)));
        disconnect(&com, SIGNAL(toneControlChanged(ToneSettings)),
                this, SLOT(onToneControlChanged(ToneSettings)));
        disconnect(&com, SIGNAL(channelVolumeChanged(ChannelVolumeCompleteInfo)),
                this, SLOT(onChannelVolumeChanged(ChannelVolumeCompleteInfo)));
        disconnect(&com, SIGNAL(subwStereoModeChanged(bool)),
                this, SLOT(onSubwooferStereoModeChanged(bool)));
        disconnect(&com, SIGNAL(muteChanged(bool)),
                ui->pbMute, SLOT(setChecked(bool)));
        disconnect(&com, SIGNAL(ecoModeChanged(EcoStates)),
                this, SLOT(onEcoModeChanged(EcoStates)));
        disconnect(&com, SIGNAL(sleepSet(int, QTime)),
                this, SLOT(onSleepSet(int, QTime)));
        disconnect(&com, SIGNAL(sleepCanceled()),
                this, SLOT(onSleepCanceled()));
        disconnect(&com, SIGNAL(dimmerStateChanged(DimmerState)),
                this, SLOT(onDimmerStateChanged(DimmerState)));


        com.disconnectDenon();
    }
    event->accept();
}

void ui_main::enableGui(bool enable)
{
    ui->dial->setEnabled(enable);
    ui->gbAudioSettings->setEnabled(enable);
    ui->gbSubwooferSettings->setEnabled(enable);
    ui->actionUpdate->setEnabled(enable);
    ui->cbSource->setEnabled(enable);
    ui->menuSound_Mode->setEnabled(enable);
    ui->pbMute->setEnabled(enable);
    ui->menuEco->setEnabled(enable);
    ui->menuDimmer->setEnabled(enable);
    ui->actionSleep->setEnabled(enable);
    ui->actionGame->setEnabled(enable);
    ui->actionMusik->setEnabled(enable);
    ui->actionMovie->setEnabled(enable);
    ui->actionDirect->setEnabled(enable);
}

void ui_main::initInputSignals()
{
    ui->cbSource->clear();

    for(int i = 0 ; i< SI_NUMS; i++)
    {
        ui->cbSource->addItem(QString(str_signalinputs[i]));
    }

    ui->cbSource->setCurrentIndex(0);
}

void ui_main::sliderCustomContextMenu(const QPoint &pos, QSlider *slider)
{
    slider->setValue(0);
    return;

    /*QMenu m(this);
    QAction *aReset = m.addAction("Zurücksetzen");

    QAction *result = m.exec(slider->mapToGlobal(pos));

    if(result == aReset)
    {
        slider->setValue(0);
    }*/
}

void ui_main::setSleepSetRecently()
{
    sleepSetRecently = true;
    delayTicks[DL_SLEEP_SET_TIMER] = DELAY_SECONDS(2.5);
}

void ui_main::resetSleepSetRecently()
{
    sleepSetRecently = false;
    delayTicks[DL_SLEEP_SET_TIMER] = DELAY_SECONDS(0);
}


void ui_main::on_hsBass_valueChanged(int value)
{
    ui->leBass->setText(QString().sprintf("%+d dB", value));

    if(sliderActionTriggered[SL_BASS])
    {
       sliderActionTriggered[SL_BASS] = false;
       com.setToneBass(value);
    }
}

void ui_main::on_hsTreble_valueChanged(int value)
{
    ui->leTreble->setText(QString().sprintf("%+d dB", value));

    if(sliderActionTriggered[SL_TREBLE])
    {
       sliderActionTriggered[SL_TREBLE] = false;
       com.setToneTreble(value);
    }
}

void ui_main::on_hsSubwoofer_valueChanged(int value)
{
    double db = value/2.0;
    ui->leLfe->setText(QString().sprintf("%+3.1f dB", db));

    if(sliderActionTriggered[SL_SUBWOOFER])
    {
       sliderActionTriggered[SL_SUBWOOFER] = false;
       com.setChannelVolume(ChannelVolume::CH_SUBWOOFER, db);
    }
}

void ui_main::on_hsSubwoofer_customContextMenuRequested(const QPoint &pos)
{
    sliderCustomContextMenu(pos, ui->hsSubwoofer);
    com.setChannelVolume(ChannelVolume::CH_SUBWOOFER, 0.0);
}

void ui_main::on_hsBass_customContextMenuRequested(const QPoint &pos)
{
    sliderCustomContextMenu(pos, ui->hsBass);
    com.setToneBass(0);
}

void ui_main::on_hsTreble_customContextMenuRequested(const QPoint &pos)
{
    sliderCustomContextMenu(pos, ui->hsTreble);
    com.setToneTreble(0);
}

void ui_main::on_hsSubwoofer_actionTriggered(int action)
{
    sliderActionTriggered[SL_SUBWOOFER] = true;
}

void ui_main::on_hsBass_actionTriggered(int action)
{
    sliderActionTriggered[SL_BASS] = true;
}

void ui_main::on_hsTreble_actionTriggered(int action)
{
    sliderActionTriggered[SL_TREBLE] = true;
}



void ui_main::on_gbAudioSettings_clicked(bool checked)
{
    com.setToneCtrl(checked);
}

void ui_main::on_gbSubwooferSettings_clicked(bool checked)
{
    com.setSubwooferState(checked);
}

void ui_main::on_dial_sliderPressed()
{
    sliderActionTriggered[SL_VOLUME] = true;
}

void ui_main::on_dial_sliderReleased()
{
    sliderActionTriggered[SL_VOLUME] = false;
}


void ui_main::on_dial_valueChanged(int value)
{
    double abs = value/2.0;
    double db = Denonet::convertAbsToDB(abs);


    if(lastVolumeValue >= 0.0)
    {
        double diff = abs - lastVolumeValue;

        double speed = diff/lastVolumeChange.elapsed();
        double speed2 = abs*abs-(lastVolumeValue*lastVolumeValue);

        if((speed >= 25) || (speed2 >= 1500.0))
        {
            disconnect(ui->dial, SIGNAL(valueChanged(int)),
                       this, SLOT(on_dial_valueChanged(int)));

            ui->dial->setValue(static_cast<int>(lastVolumeValue*2.0));

            connect(ui->dial, SIGNAL(valueChanged(int)),
                               this, SLOT(on_dial_valueChanged(int)));

            lastVolumeChange.start();

            return;
        }
    }




    ui->lbVolume->setText(QString().sprintf("%4.1f", abs));

    if(db <= -80.0)
        ui->lbVolumeDb->setText("   ---  ");
    else
        ui->lbVolumeDb->setText(QString().sprintf("%-04.1f dB", db));

    com.setMasterVolume(abs);

    lastVolumeValue = abs;
    lastVolumeChange.start();

}

void ui_main::on_pbMute_clicked(bool checked)
{
    com.setMute(checked);
}

void ui_main::on_actionMusik_triggered()
{
    com.setSoundType(SoundType::ST_MUSIC);
}

void ui_main::on_actionMovie_triggered()
{
     com.setSoundType(SoundType::ST_MOVIE);
}

void ui_main::on_actionGame_triggered()
{
     com.setSoundType(SoundType::ST_GAME);
}

void ui_main::on_actionDirect_triggered()
{
     com.setSoundType(SoundType::ST_DIRECT);
}

void ui_main::on_actionEcoEin_triggered()
{

    ui->actionEcoEin->setChecked(true);
    ui->actionEcoAus->setChecked(false);
    ui->actionEcoAuto->setChecked(false);

    com.setEcoMode(ECO_ON);
}

void ui_main::on_actionEcoAus_triggered()
{

    ui->actionEcoEin->setChecked(false);
    ui->actionEcoAus->setChecked(true);
    ui->actionEcoAuto->setChecked(false);

    com.setEcoMode(ECO_OFF);
}

void ui_main::on_actionEcoAuto_triggered()
{

    ui->actionEcoEin->setChecked(false);
    ui->actionEcoAus->setChecked(false);
    ui->actionEcoAuto->setChecked(true);

    com.setEcoMode(ECO_AUTO);
}

void ui_main::on_actionSleep_triggered()
{

    if(sleepSelected != -1)
    {
        if( ! sleepSetRecently)
        {
            // reset sleep timer
            com.setSleep(sleepMinutes[sleepSelected]);
            setSleepSetRecently();
            ui->statusbar->showMessage("Sleep zurückgesetzt", 5000);
            return;
        }
    }

    sleepSelected++;

    if(sleepSelected >= 6)
    {
        sleepSelected = -1;
        com.setSleep(0);
    }else{
        com.setSleep(sleepMinutes[sleepSelected]);
        setSleepSetRecently();
    }
}

void ui_main::on_delayTimer()
{

    if(delayTicks[DL_SLEEP_NOTICE] > 0)
    {
        delayTicks[DL_SLEEP_NOTICE] --;
        if(delayTicks[DL_SLEEP_NOTICE] == 0)
        {
            ui_sleepStatus.setText("");
        }
    }

    if(delayTicks[DL_SLEEP_SET_TIMER] > 0)
    {
        delayTicks[DL_SLEEP_SET_TIMER] --;
        if(delayTicks[DL_SLEEP_SET_TIMER] == 0)
        {
            sleepSetRecently=false;
        }

    }

}

void ui_main::on_actionDimDark_triggered()
{
    com.setDimmerState(DimmerState::DIM_DARK);
}

void ui_main::on_actionDimDim_triggered()
{
    com.setDimmerState(DimmerState::DIM_DIM);
}

void ui_main::on_actionDimBright_triggered()
{
    com.setDimmerState(DimmerState::DIM_BRIGHT);
}

void ui_main::on_actionDimOff_triggered()
{
    com.setDimmerState(DimmerState::DIM_OFF);
}