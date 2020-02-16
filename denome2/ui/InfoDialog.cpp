#include "InfoDialog.h"
#include "ui_InfoDialog.h"

InfoDialog::InfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Info Denome 2");
}

InfoDialog::~InfoDialog()
{
    delete ui;
}

void InfoDialog::on_pbClose_clicked()
{
    this->close();
}
