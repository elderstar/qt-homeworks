#ifndef LOAD_DIALOG_H
#define LOAD_DIALOG_H

#include <QDialog>
#include <QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>

#include <QtConcurrent/QtConcurrent>

#include "DB.h"

#define STAT_CONNECTION "STAT_CONNECTION"

// в БД месяцы с 1 нумеруются, поэтому я в QMap пронумеровал с 1 и в ComboBox нумерую i+1
const QMap<int, QString> months = {
    {1, "Январь"},
    {2, "Февраль"},
    {3, "Март"},
    {4, "Апрель"},
    {5, "Май"},
    {6, "Июнь"},
    {7, "Июль"},
    {8, "Август"},
    {9, "Сентябрь"},
    {10, "Октябрь"},
    {11, "Ноябрь"},
    {12, "Декабрь"}
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
signals:
    void sig_chart_annual_ready();
private slots:
    void on_pb_close_graphs_clicked();
    void slot_receiveDataFromDB(QSqlQueryModel *query_model, int request_type);
    void on_tw_graphs_currentChanged(int index);
    void on_cb_months_currentIndexChanged(int index);

public:
    explicit Dialog(QWidget *parent = nullptr, DB* db = nullptr, const QString selected_airport_name = "",
                    const QString selected_airport_code = "", const int selected_year = 1900);
    ~Dialog();
    template<typename T> void clearGraph(QChart* chart, T* series);
    void prepareGraphs();
private:
    void resizeEvent(QResizeEvent*);
    void getMonthlyLoad();

    Ui::Dialog* ui;
    DB*         _db;
    QString     _airport_name, _airport_code;
    int         _year;
    int         month_idx = 1;

    QBarSeries* series_annual;
    QChart*     chart_annual;
    QChartView* chart_view_annual;

    QLineSeries* series_monthly;
    QChart*     chart_monthly;
    QChartView* chart_view_monthly;
    QValueAxis* axisXm = nullptr;
    QValueAxis* axisYm = nullptr;
    QMessageBox* msg_box = nullptr;

};

#endif // LOAD_DIALOG_H
