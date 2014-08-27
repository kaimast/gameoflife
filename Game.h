#ifndef GAME_H
#define GAME_H

#include <map>
#include <vector>
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

    Tile& createTile(const vector2& pos);
    Tile& getTile(const vector2& pos);
    const Tile& getTile(const vector2& pos) const;
    bool hasTile(const vector2& pos) const;

    vector<const Tile*> getActiveTiles() const;

    //! Was a rectangle active in the last round?
    bool wasActive(const vector2& pos) const;

    uint32_t getRoundNumber() const;

private:
    int64_t toMapPos(const vector2& pos) const;

    bool mOk = true;
    uint32_t mRound = 0;

    map<int64_t, Tile*> mTiles, mOldTiles;
    unique_ptr<Graphics> mGraphics;
};

inline bool Game::isOk() const
{
    return mOk;
}

inline void Game::stop()
{
    mOk = false;
}

inline int64_t Game::toMapPos(const vector2& pos) const
{
    return (static_cast<uint64_t>(pos.X) << sizeof(uint32_t)*8) + pos.Y;
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
