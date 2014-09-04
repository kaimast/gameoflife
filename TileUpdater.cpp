#include "TileUpdater.h"
#include "Game.h"

TileUpdater::TileUpdater(const Game &game) : mGame(game)
{

}

void TileUpdater::update(const unordered_map<int64_t, Tile*>& tiles)
{
   for(auto it: tiles)
   {
       mWorkStack.push(it.second);
   }

   std::thread t1(&TileUpdater::work, this);
   std::thread t2(&TileUpdater::work, this);
   std::thread t3(&TileUpdater::work, this);
   std::thread t4(&TileUpdater::work, this);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void TileUpdater::work()
{
    bool ok = true;

    while(ok)
    {
        auto next = getNextTile();

        if(next)
        {
            next->update();
        }
        else
        {
            ok = false;
        }
    }
}

Tile* TileUpdater::getNextTile()
{
    mStackLock.lock();
    Tile* result = nullptr;

    if(!mWorkStack.empty()) {
        result = mWorkStack.top();
        mWorkStack.pop();
    }

    mStackLock.unlock();\
    return result;
}
