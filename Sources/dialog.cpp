#include "../Headers/dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->getStartupSetting();
    if(this->getStartupSetting() == "true")
        ui->checkBox->setChecked(true);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getStartupSetting()
{
    QString answer = request.getSettingsValue("boot");
    if(answer.isNull())
        return "false";
    else
        return answer;
}

void Dialog::on_buttonBox_accepted()
{
    if(ui->checkBox->isChecked())
        request.setSettingsValue("boot","true");
    else
        request.setSettingsValue("boot","false");
}

void Dialog::on_buttonBox_rejected()
{
    delete ui;
}
