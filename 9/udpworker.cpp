#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{



}


/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    manualInputUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    manualInputUdpSocket->bind(QHostAddress::LocalHost, BIND_MAN_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(manualInputUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingInputDatagrams);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadManInputDatagram(QNetworkDatagram datagram)
{
    QByteArray data;
    data = datagram.data();
    QDataStream inStr(&data, QIODevice::ReadOnly);
    QString input_data;
    inStr >> input_data;

    emit sig_sendInputToGUI(input_data, datagram.senderAddress().toString());
}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendInputDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    manualInputUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_MAN_PORT);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */

    while(serviceUdpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
        ReadDatagram(datagram);
    }
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingInputDatagrams( void )
{
    /*
     *  Производим чтение принятых датаграмм
     */

    while(manualInputUdpSocket->hasPendingDatagrams())
    {
        QNetworkDatagram datagram = manualInputUdpSocket->receiveDatagram();
        ReadManInputDatagram(datagram);
    }
}
