#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stopwatch.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
private slots:
    void RsvToggleSig();
    void RsvResetSig();
    void RsvRecLapTimeSig();
    void RsvUpdateSig();
    void on_pb_start_stop_toggled(bool checked);

    void on_pb_reset_clicked();

    void on_pb_lap_clicked();

private:
    Ui::MainWindow *ui;
    Stopwatch *stopwatch;
};
#endif // MAINWINDOW_H
