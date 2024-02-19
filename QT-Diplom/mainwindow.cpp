#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "load_dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Инспектор аэропортов");

    db          =   new DB(this);
    db->addDB(POSTGRE_DRIVER, MAINWINDOW_CONNECTION);
    timer       =   new QTimer(this);
    msg_box     =   new QMessageBox(this);

    toggleConnStatus();

    ui->pb_load_flights->setText("Cписок рейсов");
    ui->pb_load_flights->setDisabled("true");
    ui->pb_show_load->setText("Загруженность");
    ui->pb_show_load->setDisabled("true");

    for(auto const & key:destination.keys())
    {
        QVariant var_key(destination.value(key));
        ui->cb_destinations->addItem(destination.value(key), var_key);
    }

    connect(timer, &QTimer::timeout, this, &MainWindow::slot_Timer);
    connect(db, &DB::sig_returnDates, this, &MainWindow::slot_initCalendar);
    connect(db, &DB::sig_sendConnectionStatus, this, &MainWindow::slot_receiveConnectionStatus);
    connect(db, &DB::sig_sendDataFromDB, this, &MainWindow::slot_receiveDataFromDB);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slot_receiveConnectionStatus(bool status)
{
    toggleConnStatus();
    if(db->isOpen())
    {
        db->getAirports();
        auto get_dates_task = [&](){
            return db->getDates();
        };
        auto dates_result = QtConcurrent::run(get_dates_task);
    }else{
        tryToConnect();
    }
}

void MainWindow::slot_receiveDataFromDB(QSqlQueryModel *query_model, int request_type)
{

    switch (request_type) {
        case request_types::GET_AIRPORTS:
        {
            QString airport_name = "";
            for(int i = 0; i < query_model->rowCount(); ++i)
            {
                airport_name = query_model->record(i).value("airport_name").toString();
                QVariant airport_code(query_model->record(i).value("airport_code").toString());
                ui->cb_airports->addItem(airport_name, airport_code);

            }
            ui->pb_load_flights->setEnabled("true");
            ui->pb_show_load->setEnabled("true");
            break;
        }
        case request_types::GET_FLIGHTS:
        {
            cols_in_last_query = query_model->columnCount();
            int rows = query_model->rowCount();
            int tb_width = ui->tableWidget->width()-18;
            ui->tableWidget->setRowCount(rows);
            ui->tableWidget->setColumnCount(cols_in_last_query);
            ui->tableWidget->setHorizontalHeaderLabels({
                "Номер рейса",
                "Время вылета",
                query_model->headerData(2, Qt::Horizontal, 0).toString()});

            for(int i = 0; i < cols_in_last_query; ++i)
            {
                ui->tableWidget->setColumnWidth(i, tb_width/cols_in_last_query);
            }

            for(int row_idx = 0; row_idx < rows; ++row_idx)
            {
                for(int col_idx=0; col_idx < cols_in_last_query; ++col_idx)
                {
                    ui->tableWidget->setItem(
                        row_idx,
                        col_idx,
                        new QTableWidgetItem(query_model->record(row_idx).value(col_idx).toString()));
                }
            }
            break;
        }
        default:
            break;
    }
}

void MainWindow::tryToConnect()
{
    auto conn = [&](){

        db->connect(DB_data);
    };
    auto result = QtConcurrent::run(conn);
}

void MainWindow::slot_Timer()
{
    timer->stop();
    tryToConnect();
}

void MainWindow::toggleConnStatus()
{
    if(db->isOpen())
    {
        ui->pb_load_flights->setDisabled(false);
        ui->lb_conn_status->setStyleSheet("color:green");
        ui->lb_conn_status->setText("Подключено");
    }else{
        ui->pb_load_flights->setDisabled(true);
        ui->lb_conn_status->setStyleSheet("color:red");
        ui->lb_conn_status->setText("Отключен");
        auto db_err_text = db->getLastError().text();

        if(db_err_text != "")
        {
            db_err_text += "\n Пробую соединиться снова...";
            msg_box->setText(db_err_text);
            msg_box->exec();
        }

        if (!msg_box->isVisible()){
            timer->start(RECONNECT_INTERVAL * 1000);
        }
    }
}

void MainWindow::slot_initCalendar(const QDate &_min_date, const QDate &_max_date)
{
    min_date = _min_date;
    max_date = _max_date;
    ui->calendarWidget->setMinimumDate(min_date);
    ui->calendarWidget->setMaximumDate(max_date);
    ui->calendarWidget->setSelectedDate(min_date);
}

void MainWindow::on_cb_airports_currentIndexChanged(int index)
{
    selected_airport_code = ui->cb_airports->currentData().toString();
    selected_airport_name = ui->cb_airports->currentText();
}

void MainWindow::on_cb_destinations_currentIndexChanged(int index)
{
    selected_direction = ui->cb_destinations->currentIndex();
}

void MainWindow::on_pb_load_flights_clicked()
{
    if(db->isOpen())
    {
        db->getFlights(ui->calendarWidget->selectedDate().toString(), selected_airport_code, selected_direction);
    }else{
        tryToConnect();
    }
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    int tb_width = ui->tableWidget->width()-18;
    for(int i = 0; i < cols_in_last_query; ++i)
    {
        ui->tableWidget->setColumnWidth(i, tb_width/cols_in_last_query);
    }
}

void MainWindow::on_pb_show_load_clicked()
{
    show_load_dialog    =   new Dialog(
                                        this,
                                        db,
                                        selected_airport_name,
                                        selected_airport_code,
                                        ui->calendarWidget->selectedDate().year()
                                    );
    show_load_dialog->exec();
}

