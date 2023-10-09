#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Games.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void changeProgress(int val);
private slots:

    void on_pushButton_toggled(bool checked);

    void on_rb_for_me_toggled(bool checked);

    void on_comboBox_currentIndexChanged(int index);

    void on_pb_install_clicked();

    void on_rb_all_users_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    Games *games;
};
#endif // MAINWINDOW_H
