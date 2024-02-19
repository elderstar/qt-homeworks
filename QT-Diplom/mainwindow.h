#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include <QTimer>
#include <QVariant>

#include <QDate>
#include <QCalendar>

#include "DB.h"

#define MAINWINDOW_CONNECTION "MAINWINDOW_CONNECTION"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void slot_receiveConnectionStatus(bool status);
    void slot_receiveDataFromDB(QSqlQueryModel *query_model, int request_type);
    void slot_initCalendar(const QDate &_min_date, const QDate &_max_date);
    void slot_Timer();

private slots:

    void on_cb_airports_currentIndexChanged(int index);
    void on_pb_load_flights_clicked();
    void on_cb_destinations_currentIndexChanged(int index);
    void on_pb_show_load_clicked();

private:
    void resizeEvent(QResizeEvent*);
    void tryToConnect();
    void toggleConnStatus();
    void initCalendar(const QDate date);

    Ui::MainWindow *ui;

    QMap <QString,QString> DB_data = {
        {"host","981757-ca08998.tmweb.ru"},
        {"DB_name","demo"},
        {"user","netology_usr_cpp"},
        {"pass","CppNeto3"},
        {"port","5432"},
    };

    QDate           min_date;
    QDate           max_date;
    QMessageBox     *msg_box;
    DB              *db;
    QTimer          *timer;
    QString         selected_airport_code;
    QString         selected_airport_name;
    int             selected_direction;
    QDate           selected_date;
    QDialog*        show_load_dialog;
    int             cols_in_last_query;
};
#endif // MAINWINDOW_H
