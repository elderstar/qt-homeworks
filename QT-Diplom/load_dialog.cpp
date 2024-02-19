#include "load_dialog.h"
#include "./ui_load_dialog.h"

Dialog::Dialog(QWidget *parent, DB* db, const QString airport_name,
               const QString airport_code, const int year) :
    QDialog(parent),
    ui(new Ui::Dialog),
    _db(db),
    _airport_name(airport_name),
    _airport_code(airport_code),
    _year(year)
{
    ui->setupUi(this);
    this->setWindowTitle("Статистика загруженности aэропорта");
    ui->lb_airport_name->setText(_airport_name + " " + QString::number(_year));

    for(int i = 0; i < months.count(); ++i)
    {
        ui->cb_months->addItem(months[i+1], i+1);
    }

    chart_annual        =   new QChart();
    chart_view_annual   =   new QChartView(chart_annual);
    series_annual       =   new QBarSeries(this);

    chart_monthly       =   new QChart();
    chart_view_monthly  =   new QChartView(chart_monthly);
    series_monthly      =   new QLineSeries(this);

    QGridLayout* layout_annual = new QGridLayout();
    QGridLayout* layout_monthly = new QGridLayout();
    layout_annual->addWidget(chart_view_annual);
    layout_monthly->addWidget(ui->cb_months, 0, 0, Qt::AlignTop);
    layout_monthly->addWidget(chart_view_monthly, 1, 0,  Qt::AlignTop);

    ui->pb_close_graphs->setText("Закрыть окно");
    ui->tw_graphs->setTabText(0, "Статистика за год");
    ui->tw_graphs->setTabText(1, "Статистика за месяц");
    ui->tab->setLayout(layout_annual);
    ui->tab_2->setLayout(layout_monthly);

    connect(db, &DB::sig_sendDataFromDB, this, &Dialog::slot_receiveDataFromDB);

    _db->getAnnualLoad(_year, _airport_code);
}

Dialog::~Dialog()
{
    delete ui;
    delete chart_annual;
    delete chart_view_annual;
    delete chart_monthly;
    delete chart_view_monthly;
    delete axisXm;
    delete axisYm;
}

template<typename T> void Dialog::clearGraph(QChart* chart, T* series)
{
    if(!chart->series().isEmpty())
    {
        series->clear();
        chart->removeSeries(series);
    }
}

void Dialog::on_pb_close_graphs_clicked()
{
    this->close();
}

void Dialog::slot_receiveDataFromDB(QSqlQueryModel *query_model, int request_type)
{
    switch (request_type) {
        case request_types::GET_ANNUAL_LOAD:
        {
            clearGraph(chart_annual, series_annual);
            QStringList months_axis;
            QBarCategoryAxis* axisX = new QBarCategoryAxis();
            QValueAxis* axisY = new QValueAxis();
            QBarSet* set = new QBarSet("Месяцы");
            int m=0;
            for(int i = 0; i < months.count(); ++i)
            {
                months_axis << months[i+1];
                int month_idx = query_model->record(m).value("Month").toDate().month();
                if(month_idx == i+1)
                {
                    *set << query_model->record(m++).value("count").toInt();
                }else{
                    *set << 0;
                }
            }

            series_annual->append(set);
            chart_annual->addSeries(series_annual);

            axisX->append(months_axis);
            chart_annual->addAxis(axisX, Qt::AlignBottom);
            chart_annual->addAxis(axisY, Qt::AlignLeft);

            series_annual->attachAxis(axisX);
            series_annual->attachAxis(axisY);

            chart_annual->legend()->setVisible(true);
            chart_annual->legend()->setAlignment(Qt::AlignBottom);

            chart_view_annual->resize(ui->tw_graphs->width(), ui->tw_graphs->height());
            chart_view_annual->show();
            break;
        }
        case request_types::GET_MONTHLY_LOAD:
        {
            clearGraph(chart_monthly, series_monthly);

            if(axisXm != nullptr)
            {
                chart_monthly->removeAxis(axisXm);
                chart_monthly->removeAxis(axisYm);
                delete axisXm;
                delete axisYm;
                axisXm    = nullptr;
                axisYm    = nullptr;
            }
            int Y_max = 0;
            int Y_min = 0;

            for(int i = 0; i < query_model->rowCount(); ++i)
            {
                int count = query_model->record(i).value(0).toInt();

                count > Y_max ? Y_max = count : false ;
                count < Y_min || i == 0 ? Y_min = count : false ;

                series_monthly->append(
                                query_model->record(i).value(1).toDate().day(),
                                count
                            );
            }

            // если график загруженности - горизонтальная прямая, то его не видно на графике, расширяю интервал по оси Y
            Y_min > 0 ? --Y_min : false ;
            ++Y_max;
            chart_monthly->addSeries(series_monthly);
            axisXm              =   new QValueAxis();
            axisYm              =   new QValueAxis();
            axisXm->setTickCount(query_model->rowCount());
            axisYm->setRange(Y_min, Y_max);
            axisXm->setLabelFormat("%i");
            axisYm->setLabelFormat("%i");
            chart_monthly->addAxis(axisXm, Qt::AlignBottom);
            chart_monthly->addAxis(axisYm, Qt::AlignLeft);
            series_monthly->attachAxis(axisXm);
            series_monthly->attachAxis(axisYm);

            chart_monthly->legend()->hide();
            chart_monthly->resize(ui->tw_graphs->width(), ui->tw_graphs->height() - ui->cb_months->height()*2);
            chart_monthly->show();
            break;
        }
        default:
            break;
    }
}

void Dialog::resizeEvent(QResizeEvent*)
{
    chart_view_annual->resize(ui->tw_graphs->width(), ui->tw_graphs->height());
    chart_monthly->resize(ui->tw_graphs->width(), ui->tw_graphs->height() - ui->cb_months->height()*2);

    int width = this->width()/2-11;
    ui->tw_graphs->setStyleSheet("QTabBar::tab {width:" + QString::number(width) + "px;}");
}

void Dialog::on_tw_graphs_currentChanged(int index)
{
    if(index == 1)
    {
        getMonthlyLoad();
    }
}

void Dialog::on_cb_months_currentIndexChanged(int index)
{
    getMonthlyLoad();
}

void Dialog::getMonthlyLoad()
{
    month_idx = ui->cb_months->currentData().toInt();
    _db->getMonthlyLoad(month_idx, _year, _airport_code);
}

