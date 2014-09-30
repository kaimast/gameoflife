#ifndef TILEUPDATER_H
#define TILEUPDATER_H

#include "Tile.h"
#include <stack>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_map>

class Game;

class TileUpdater
{
public:
    TileUpdater(const Game& game);
    ~TileUpdater();

    void update(const unordered_map<int64_t, Tile*>& tiles);

    bool running() const;

private:
    void work();
    Tile* getNextTile();

    const Game& mGame;

    // This mutex shall be used to lock mWorkStac
    mutex mStackLock;

    // Remaining tiles to be processed
    stack<Tile*> mWorkStack;

    // Amount of thread that are currently working
    std::atomic<unsigned int> mWorking;


    stack<thread> mThreads;
};

#endif // TILEUPDATER_H
