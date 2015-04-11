#include "../Headers/dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);  
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    setWindowFlags(flags);
    if(this->getNotificationSetting() == "true")
        ui->notificationCheckBox->setChecked(true);
}

Dialog::~Dialog()
{
    delete ui;
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
    if(ui->notificationCheckBox->isChecked())
        request.setSettingsValue("notification","true");
    else
        request.setSettingsValue("notification","false");
}

void Dialog::on_buttonBox_rejected()
{
    delete ui;
}
