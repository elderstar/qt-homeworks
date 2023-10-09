#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->groupBox->setTitle("Установка программы:");
    ui->rb_for_me->setText("Для меня");
    ui->rb_all_users->setText("Для всех");
    ui->comboBox->addItem("Выбери игру для установки");

    games = new Games();
    auto games_list = games->getGames();

    for(const auto& game:games_list)
    {
        ui->comboBox->addItem(game);
    }
    ui->pushButton->setText("Запустить после установки");
    ui->pushButton->setCheckable(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->pb_install->setText("Установить");
    ui->lb_status->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete games;
}

void MainWindow::changeProgress(int val)
{
    int new_val = ui->progressBar->value()+val;

    ui->progressBar->setValue(new_val <= 100 ? new_val : 0);
}


void MainWindow::on_pushButton_toggled(bool checked)
{
    changeProgress(10);
}


void MainWindow::on_rb_for_me_toggled(bool checked)
{
    changeProgress(10);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    changeProgress(10);
}


void MainWindow::on_pb_install_clicked()
{
    while(ui->progressBar->value() < 100)
    {
        changeProgress(10);
        QThread::sleep(1);
    }
    ui->lb_status->setText("Игра установлена!");
    ui->lb_status->show();
}


void MainWindow::on_rb_all_users_toggled(bool checked)
{
    changeProgress(10);
}

