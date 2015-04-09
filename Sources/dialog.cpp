#include "../Headers/dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->getStartupSetting();
    if(this->getStartupSetting() == "true")
        ui->bootCheckBox->setChecked(true);
    if(this->getNotificationSetting() == "true")
        ui->notificationCheckBox->setChecked(true);
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

QString Dialog::getNotificationSetting()
{
    QString answer = request.getSettingsValue("notification");
    if(answer.isNull())
        return "false";
    else
        return answer;
}

void Dialog::on_buttonBox_accepted()
{
    if(ui->bootCheckBox->isChecked())
        request.setSettingsValue("boot","true");
    else
        request.setSettingsValue("boot","false");

    if(ui->notificationCheckBox->isChecked())
        request.setSettingsValue("notification","true");
    else
        request.setSettingsValue("notification","false");
}

void Dialog::on_buttonBox_rejected()
{
    delete ui;
}
