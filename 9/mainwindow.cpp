#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_send_datagram->setText("Отправить датаграмму");

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, &UDPworker::sig_sendTimeToGUI, this, &MainWindow::DisplayTime);
    connect(udpWorker, &UDPworker::sig_sendInputToGUI, this, &MainWindow::DisplayInput);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);

    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
}

void MainWindow::DisplayInput(QString input, QString sender_ip)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    int size_bytes = input.toUtf8().size();

    QString str = "Принято " + input + " от " + sender_ip + ", размер сообщения(байт) " + QString::number(size_bytes);

    ui->te_result->append(str);
}

void MainWindow::DisplayTime(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));
}


void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
}


void MainWindow::on_pb_send_datagram_clicked()
{
    QString input = ui->le_datagram->text();
    QByteArray input_byte_data;
    QDataStream input_str (&input_byte_data, QIODevice::WriteOnly);

    input_str << input;

    udpWorker->SendInputDatagram(input_byte_data);
}

