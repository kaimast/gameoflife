#ifndef TILEUPDATER_H
#define TILEUPDATER_H

#include "Tile.h"
#include <stack>
#include <thread>
#include <mutex>
#include <unordered_map>

class Game;

class TileUpdater
{
public:
    TileUpdater(const Game& game);

    void update(const unordered_map<int64_t, Tile*>& tiles);

private:
    void work();

    Tile* getNextTile();

    stack<Tile*> mWorkStack;
    mutex mStackLock;

    const Game& mGame;
};

#endif // TILEUPDATER_H
