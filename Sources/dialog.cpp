#include "../Headers/dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->configureDialog();
}

Dialog::~Dialog()
{
    delete ui;
}

//Configure how dialog looks and such.  The flag is to remove the ? button from windows.
//Check if the box should be checked by default.
void Dialog::configureDialog()
{
    Qt::WindowFlags flags = windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    flags = flags & (~helpFlag);
    setWindowFlags(flags);
    //Set the notification display to checked if true
    if(this->getNotificationSetting() == "true")
        ui->notificationCheckBox->setChecked(true);
    ui->liveCheckbox->setEnabled(false);
    if(request.checkForLiveStreamer())
    {
        ui->liveCheckbox->setEnabled(true);
        if(this->getLivestreamSetting() == "true")
            ui->liveCheckbox->setChecked(true);
    }
    else
        ui->liveCheckbox->setEnabled(false);
}

//Check the saved setting for notification display
QString Dialog::getLivestreamSetting()
{
    QString answer = request.getSettingsValue("livestream");
    if(answer.isNull())
        return "false";
    else
        return answer;
}


//Check the saved setting for notification display
QString Dialog::getNotificationSetting()
{
    QString answer = request.getSettingsValue("notification");
    if(answer.isNull())
        return "false";
    else
        return answer;
}

//When Ok is pressed in the dialog, save results.
void Dialog::on_buttonBox_accepted()
{
    if(ui->notificationCheckBox->isChecked())
        request.setSettingsValue("notification","true");
    else
        request.setSettingsValue("notification","false");
    if(ui->liveCheckbox->isChecked())
        request.setSettingsValue("livestream","true");
    else
        request.setSettingsValue("livestream","false");
}

//Destroy the dialog on cancel
void Dialog::on_buttonBox_rejected()
{
    delete ui;
}
