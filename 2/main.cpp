#include <QCoreApplication>
#include <QLocale>
#include <QSqlDatabase>
#include <QHostAddress>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase sql;
    QHostAddress host;
    sql.setHostName("localhost");
    sql.setDatabaseName("db_name");
    sql.setUserName("root");
    sql.setPassword("password");
    host.setAddress("192.168.1.101");

    return a.exec();
}
