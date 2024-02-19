#ifndef DB_H
#define DB_H

#include <QMainWindow>
#include <QObject>
#include <QTableView>
#include <QMap>

#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlRecord>

#define POSTGRE_DRIVER      "QPSQL"
#define RECONNECT_INTERVAL  5

enum request_types{
    GET_AIRPORTS,
    GET_FLIGHTS,
    GET_ANNUAL_LOAD,
    GET_MONTHLY_LOAD,
};

const QMap<int, QString> destination = {
    {0, "Вылет"},
    {1, "Прилет"}
};

class DB:   public QObject
{
    Q_OBJECT
public:
    explicit DB(QObject *parent =   nullptr);
    ~DB();

    void addDB(QString driver, QString conn_name    =   "");
    void disconnect(QString conn_name   =   "");
    void sendRequest(QString request, QTableView *view, int request_type);
    QSqlError getLastError(void);
    void connect(QMap<QString,QString> DB_data);
    bool isOpen();
    void getAirports();
    void getFlights(QString date, QString airport_code, int direction);
    void getAnnualLoad(int year, QString airport_code);
    void getMonthlyLoad(int month, int year, QString airport_code);
    void getDates();
signals:
    void sig_returnDates(const QDate &min_date, const QDate &max_date);
    void sig_sendConnectionStatus(bool status);
    void sig_sendDataFromDB(QSqlQueryModel *query_model, int request_type);
private:

    QSqlDatabase *data_base;
    QSqlQueryModel *query_model;
};

#endif // DB_H
