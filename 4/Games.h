#ifndef GAMES_H
#define GAMES_H

#include <QString>
#include <QVector>

class Games{
 public:
    Games();
    ~Games() = default;
    QVector<QString> getGames()const;
    void fetchAvailableGames();
private:
    QVector<QString> games_avail;
};
#endif // GAMES_H
