#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    if(!isFormFilled())
    {
        ui->buttonBox->buttons().at(0)->setDisabled(true);
    }

    connect(this->ui->le_db_host, SIGNAL(textChanged(QString)), this, SLOT(toggleOkButton()));
    connect(this->ui->le_db_name, SIGNAL(textChanged(QString)), this, SLOT(toggleOkButton()));
    connect(this->ui->le_db_user, SIGNAL(textChanged(QString)), this, SLOT(toggleOkButton()));
    connect(this->ui->le_db_pass, SIGNAL(textChanged(QString)), this, SLOT(toggleOkButton()));
}

Dialog::~Dialog()
{
    delete ui;
}

DBDetais Dialog::getDBDetails()
{
    return db_details;
}

void Dialog::toggleOkButton()
{
    if(isFormFilled())
    {
        ui->buttonBox->buttons().at(0)->setDisabled(false);
    }else{
        ui->buttonBox->buttons().at(0)->setDisabled(true);
    }
}

bool Dialog::isFormFilled()
{
    if(!this->ui->le_db_host->text().isEmpty() &&
            !this->ui->le_db_name->text().isEmpty() &&
            !this->ui->le_db_user->text().isEmpty() &&
            !this->ui->le_db_pass->text().isEmpty())
    {
        return true;
    }
    return false;
}

void Dialog::on_buttonBox_accepted()
{
    db_details.db_host = this->ui->le_db_host->text();
    db_details.db_port = this->ui->sb_db_port->value();
    db_details.db_name = this->ui->le_db_name->text();
    db_details.db_user = this->ui->le_db_user->text();
    db_details.db_pass = this->ui->le_db_pass->text();
}

