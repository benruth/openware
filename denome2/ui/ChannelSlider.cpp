#include "ChannelSlider.h"

ChannelSlider::ChannelSlider(QWidget* parent, QGridLayout* grid, ChannelVolumeData &channel, Qt::Orientation o, Denonet *denonet) :
    QObject(parent),
    type(channel),
    layout(grid),
    com(denonet)
{
    int rows[3] = {0};
    int cols[3] = {0};

    actionWasTriggered = false;

    orientation = o;
    Qt::Orientation sliderOrientation;
    Qt::Alignment sliderAlignment;


    if(Qt::Horizontal == o)
    {
        sliderOrientation = Qt::Horizontal;
        rows[0] = rows[1] = rows[2] = layout->rowCount();
        cols[0] = 0;
        cols[1] = 1;
        cols[2] = 2;
        layout->setRowStretch(1,0);
        layout->setColumnStretch(1,1);
        sliderAlignment = Qt::AlignVCenter;

    }else{
        sliderOrientation = Qt::Vertical;
        cols[0] = cols[1] = cols[2] = layout->columnCount();
        rows[0] = 0;
        rows[1] = 1;
        rows[2] = 2;
        layout->setColumnStretch(1,0);
        layout->setRowStretch(1,1);
        sliderAlignment = Qt::AlignHCenter;
    }

    label = new QLabel("Text", parent);
    slider = new QSlider(sliderOrientation, parent);
    lineEdit = new QLineEdit("Text", parent);

    label->setText(channel.name);
    label->setAlignment(Qt::AlignCenter);

    slider->setMinimum(-24);
    slider->setMaximum(24);
    slider->setValue(static_cast<int>(channel.value*2.0));
    slider->setTickInterval(2);
    slider->setTickPosition(QSlider::TicksAbove); //Above = TicksLeft
    slider->setPageStep(2);
    slider->setContextMenuPolicy(Qt::CustomContextMenu);

    lineEdit->setText(QString().sprintf("+%4.1f dB", channel.value));
    lineEdit->setMinimumWidth(55);
    lineEdit->setMaximumWidth(65);
    lineEdit->setAlignment(Qt::AlignCenter);
    lineEdit->setReadOnly(true);


    layout->addWidget(label, rows[0], cols[0]);
    layout->addWidget(slider, rows[1], cols[1], sliderAlignment);
    layout->addWidget(lineEdit, rows[2], cols[2]);

    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged(int)));
    connect(slider, SIGNAL(actionTriggered(int)), this, SLOT(on_slider_actionTriggered(int)));
    connect(slider, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_slider_customContextMenu(const QPoint&)));
}

ChannelSlider::~ChannelSlider()
{
    disconnect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_valueChanged(int)));
    disconnect(slider, SIGNAL(actionTriggered(int)), this, SLOT(on_slider_actionTriggered(int)));
    disconnect(slider, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(on_slider_customContextMenu(const QPoint&)));

    layout->removeWidget(label);
    layout->removeWidget(slider);
    layout->removeWidget(lineEdit);

    delete label;
    delete slider;
    delete lineEdit;

}

void ChannelSlider::reorderItems(Qt::Orientation o)
{
    int rows[3] = {0};
    int cols[3] = {0};


    orientation = o;
    Qt::Orientation sliderOrientation;

    if(Qt::Horizontal == o)
    {
        sliderOrientation = Qt::Horizontal;
        rows[0] = rows[1] = rows[2] = layout->rowCount();
        cols[0] = 0;
        cols[1] = 1;
        cols[2] = 2;
        layout->setRowStretch(1,0);
        layout->setColumnStretch(1,1);

    }else{
        sliderOrientation = Qt::Vertical;
        cols[0] = cols[1] = cols[2] = layout->columnCount();
        rows[0] = 0;
        rows[1] = 1;
        rows[2] = 2;
        layout->setColumnStretch(1,0);
        layout->setRowStretch(1,1);
    }

    slider->setOrientation(sliderOrientation);

    layout->addWidget(label, rows[0], cols[0]);
    layout->addWidget(slider, rows[1], cols[1]);
    layout->addWidget(lineEdit, rows[2], cols[2]);
}

void ChannelSlider::changeVolume(double v)
{
    slider->setValue(static_cast<int>(v*2.0));

}

void ChannelSlider::on_slider_valueChanged(int value)
{
    lineEdit->setText(QString().sprintf("%+4.1f dB", value/2.0));

    if(actionWasTriggered)
    {
        qDebug() << "Slider: Action was triggered";
        actionWasTriggered = false;
        com->setChannelVolume(type.c, value/2.0);
    }
}

void ChannelSlider::on_slider_actionTriggered(int /*action*/)
{
    actionWasTriggered = true;
}

void ChannelSlider::on_slider_customContextMenu(const QPoint &pos)
{
    if(0 == slider->value() )
        return;

    slider->setValue(0);
    com->setChannelVolume(type.c, 0.0);
}
