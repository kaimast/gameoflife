#ifndef GAME_H
#define GAME_H

#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "defines.h"
#include "vector2.h"
#include "Tile.h"
#include "Graphics.h"

class Game
{
public:
    ~Game();

    bool init();

    void update();
    bool isOk() const;

    void stop();

    // Set a random tile active
    void set(const vector2& pos);

    void createTile(const vector2& pos);
    Tile& getTile(const vector2& pos);
    const Tile& getTile(const vector2& pos) const;
    bool hasTile(const vector2& pos) const;

    const Tile& getPreviousTile(const vector2& pos) const;
    bool hasPreviousTile(const vector2& pos) const;

    void increaseSpeed();
    void decreaseSpeed();

    bool isPaused() const;
    void togglePaused();

    uint32_t getSpeed() const;

    vector<const Tile*> getActiveTiles() const;

    //! Was a rectangle active in the last round?
    bool wasActive(const vector2& pos) const;

    uint32_t getRoundNumber() const;

    void notifyRenderStart();
    void notifyRenderStop();

private:
    void doGameLogic();
    void createInitialSetup();

    bool mOk = true;
    uint32_t mRound = 0;

    unordered_map<int64_t, Tile*> mTiles, mOldTiles;
    unique_ptr<Graphics> mGraphics;

    std::thread mGraphicsThread;

    const uint32_t mSpeedStep = 50;
    uint32_t mSpeed = 0 * mSpeedStep;

    bool mPaused = false;

    std::mutex mRendering;
};

inline bool Game::isOk() const
{
    return mOk;
}

inline void Game::stop()
{
    mOk = false;
}

inline bool Game::hasPreviousTile(const vector2 &pos) const
{
    auto key = pos.toFlatInt();
    auto it = mOldTiles.find(key);

    return (it != mOldTiles.end());
}

inline void Game::notifyRenderStart()
{
    mRendering.lock();
}

inline void Game::notifyRenderStop()
{
    mRendering.unlock();
}

inline vector<const Tile*> Game::getActiveTiles() const
{
    vector<const Tile*> result;

    for(auto it: mTiles)
    {
        result.push_back(it.second);
    }

    return result;
}

inline uint32_t Game::getRoundNumber() const
{
    return mRound;
}

#endif // GAME_H
