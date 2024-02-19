#include "DB.h"
#include "qdatetime.h"

DB::DB(QObject *parent)
{
   data_base    =   new QSqlDatabase();
   query_model  =   new QSqlQueryModel();
}

DB::~DB()
{
    delete data_base;
    delete query_model;
}

void DB::addDB(QString driver, QString conn_name)
{
    *data_base  =   QSqlDatabase::addDatabase(driver, conn_name);
}

bool DB::isOpen()
{
    return data_base->isOpen();
}

void DB::connect(QMap<QString, QString> data)
{
    data_base->setHostName(data["host"]);
    data_base->setDatabaseName(data["DB_name"]);
    data_base->setUserName(data["user"]);
    data_base->setPassword(data["pass"]);
    data_base->setPort(data["port"].toInt());

    bool status;
    status  =   data_base->open();
    emit sig_sendConnectionStatus(status);
}

void DB::getFlights(QString date, QString airport_code, int direction)
{
    query_model->clear();
    QString query = "";

    const char* airport_role = "";

    if(direction == 0)
    {
        airport_role = "Аэропорт отправления";
        query = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' "
                "FROM bookings.flights f JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport "
                "WHERE f.departure_airport = '" + airport_code + "' "
                "AND CAST(scheduled_departure AS DATE) = '" + date + "' ORDER BY flight_no ASC";
    }else if(direction == 1)
    {
        airport_role = "Аэропорт назначения";
        query = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' "
                "FROM bookings.flights f JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport "
                "WHERE f.arrival_airport = '" + airport_code + "' "
                "AND CAST(scheduled_arrival AS DATE) = '" + date + "' ORDER BY flight_no ASC";
    }

    query_model->setQuery(query, *data_base);
    query_model->setHeaderData(2, Qt::Horizontal, tr(airport_role));

    emit sig_sendDataFromDB(query_model, request_types::GET_FLIGHTS);
}

void DB::getAnnualLoad(int year, QString airport_code)
{
    query_model->clear();
    QString query = "";

    query = "SELECT count(flight_no), date_trunc('month', scheduled_departure) as \"Month\" from bookings.flights f "
    "WHERE EXTRACT(YEAR FROM scheduled_departure) = '" + QString::number(year) + "' "
    "AND ( departure_airport = '" + airport_code + "' or arrival_airport = '" + airport_code + "')"
    "GROUP BY \"Month\"";

    query_model->setQuery(query, *data_base);

    emit sig_sendDataFromDB(query_model, request_types::GET_ANNUAL_LOAD);
}

void DB::getMonthlyLoad(int month, int year, QString airport_code)
{
    query_model->clear();
    QString query = "";

    query = "SELECT count(flight_no), date_trunc('day', scheduled_departure) as \"Day\" from bookings.flights f "
    "WHERE EXTRACT(MONTH FROM scheduled_departure) = " + QString::number(month) + " "
    "AND EXTRACT(YEAR FROM scheduled_departure) = " + QString::number(year) + " "
    "AND ( departure_airport = '" + airport_code + "' or arrival_airport = '" + airport_code + "')"
    "GROUP BY \"Day\"";

    query_model->setQuery(query, *data_base);

    emit sig_sendDataFromDB(query_model, request_types::GET_MONTHLY_LOAD);
}

void DB::getDates()
{
    QString query = "SELECT MIN(scheduled_departure), MAX(scheduled_departure) FROM bookings.flights";
    query_model->setQuery(query, *data_base);

    QDate min_date = query_model->record(0).value(0).toDate();
    QDate max_date = query_model->record(0).value(1).toDate();

    emit sig_returnDates(min_date, max_date);
}

void DB::getAirports()
{
    query_model->clear();
    query_model->setQuery("SELECT airport_code, airport_name->>'ru' as airport_name FROM bookings.airports_data ORDER BY airport_code ASC", *data_base);

    emit sig_sendDataFromDB(query_model, request_types::GET_AIRPORTS);
}

void DB::disconnect(QString conn_name)
{
    *data_base  =   QSqlDatabase::database(conn_name);
    data_base->close();
    bool status;
    status  =   data_base->isOpen();
    emit sig_sendConnectionStatus(status);
}

QSqlError DB::getLastError()
{
    return data_base->lastError();
}
