#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dialog_db_data = new Dialog(this);
    updDBDetails();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updDBDetails()
{
    ui->lbl_host_val->setText("[" + db_details.db_host + " : " + QString::number(db_details.db_port) + "]");
    ui->lbl_db_name_val->setText("[" + db_details.db_name + "]");
    ui->lbl_db_user_val->setText("[" + db_details.db_user + "]");
    ui->lbl_db_pass_val->setText("[" + db_details.db_pass + "]");
}

void MainWindow::on_pushButton_clicked()
{
    if(dialog_db_data->exec() == QDialog::Accepted)
    {
        db_details = dialog_db_data->getDBDetails();
        updDBDetails();
    };
}

