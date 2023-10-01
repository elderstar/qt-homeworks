#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

struct DBDetais{
    QString db_host = "undefined";
    uint db_port = 0;
    QString db_name = "undefined";
    QString db_user = "undefined";
    QString db_pass = "undefined";
};

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    DBDetais getDBDetails();
    bool isFormFilled();
private slots:
    void on_buttonBox_accepted();
    void toggleOkButton();
private:
    Ui::Dialog *ui;
    DBDetais db_details;
};

#endif // DIALOG_H
