#include "Games.h"

Games::Games()
{
    fetchAvailableGames();
};

void Games::fetchAvailableGames()
{
    games_avail = {"Warcraft III: Reign of Chaos",
                   "Warcraft III: The Frozen Throne",
                   "Star Craft II"};
    // список доступных для установки игр, полученный якобы из базы.
}

QVector<QString> Games::getGames() const
{
    return games_avail;
}
