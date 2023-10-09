#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    stopwatch   =   new Stopwatch(this);

    RsvResetSig();
    ui->pb_start_stop->setText("Старт");
    ui->pb_start_stop->setCheckable(true);
    ui->pb_lap->setText("Круг");
    ui->pb_reset->setText("Очистить");
    QObject::connect(stopwatch, &Stopwatch::sig_TimerChanged, this, &MainWindow::RsvUpdateSig);
    QObject::connect(stopwatch, &Stopwatch::sig_SendToggleSig, this, &MainWindow::RsvToggleSig);
    QObject::connect(stopwatch, &Stopwatch::sig_SendResetSig, this, &MainWindow::RsvResetSig);
    QObject::connect(stopwatch, &Stopwatch::sig_SendRecLapTimeSig, this, &MainWindow::RsvRecLapTimeSig);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::RsvUpdateSig()
{
    ui->lb_output->setText(stopwatch->c_str(stopwatch->getTime()));
}

void MainWindow::RsvToggleSig()
{
    if(ui->pb_start_stop->isChecked())
    {
        ui->pb_start_stop->setText("Стоп");
        ui->pb_lap->setDisabled(false);
    }else{
        ui->pb_start_stop->setText("Старт");
        ui->pb_lap->setDisabled(true);
    }
}

void MainWindow::RsvResetSig()
{
    ui->lb_output->setText("0:0.0");
    ui->pb_start_stop->setChecked(false);
    ui->pb_start_stop->setText("Старт");
    ui->pb_lap->setDisabled(true);
}

void MainWindow::RsvRecLapTimeSig()
{
    ui->tb_logs->append("Круг " + QString::number(stopwatch->getLapsCount()) + ", время: " + stopwatch->c_str(stopwatch->getLastLapTime()) + " сек.");
}


void MainWindow::on_pb_start_stop_toggled(bool checked)
{
    stopwatch->ToggleStopwatch();
}


void MainWindow::on_pb_reset_clicked()
{
    stopwatch->ResetStopwatch();
}


void MainWindow::on_pb_lap_clicked()
{
    stopwatch->RecordLapTime();
}

