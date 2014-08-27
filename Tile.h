#ifndef TILE_H
#define TILE_H

#include "defines.h"
#include "vector2.h"
#include <vector>

class Game;

class Tile
{
public:
    Tile(const Game& game, const vector2& pos);

    void set(const vector2& pos);
    void clear(const vector2& pos);
    bool get(const vector2& pos) const;

    const vector2 getPosition() const;
    const vector2 getRectPos() const;

    bool hasActiveRects() const;
    vector<vector2> getActiveRects() const;

    void update();

    Tile* duplicate() const;

private:
    unique_ptr<uint8_t[]> mData;

    const Game& mGame;
    const vector2 mPosition;
};

inline const vector2 Tile::getPosition() const
{
    return mPosition;
}

inline const vector2 Tile::getRectPos() const
{
    return vector2(mPosition.X * TILE_SIZE, mPosition.Y * TILE_SIZE);
}

#endif // TILE_H
