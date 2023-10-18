#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define BIND_MAN_PORT 23456
class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void ReadManInputDatagram(QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );
    void SendInputDatagram(QByteArray data);

private slots:
    void readPendingDatagrams(void);
    void readPendingInputDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket* manualInputUdpSocket;
signals:
    void sig_sendTimeToGUI(QDateTime data);
    void sig_sendInputToGUI(QString str, QString sender_ip);
};

#endif // UDPWORKER_H
