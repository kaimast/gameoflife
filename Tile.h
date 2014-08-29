#ifndef TILE_H
#define TILE_H

#include "defines.h"
#include "vector2.h"
#include <vector>
#include <map>

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

    bool inBoundaries(const vector2& pos) const;

private:
    void updateRect(const vector2& pos);

#ifdef PACK_TILE_CONTENT
    unique_ptr<uint8_t[]> mData;
#else
    unique_ptr<bool[]> mData;
#endif

#ifdef RECT_MAP
    //TODO find a nicer datastructure for this
    // value is not really needed...
    map<uint64_t, bool> mActiveRects;
#endif

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

inline bool Tile::inBoundaries(const vector2 &pos) const
{
    return (pos.X >= 0 && pos.X < static_cast<int32_t>(TILE_SIZE)) && (pos.Y >= 0 && pos.Y < static_cast<int32_t>(TILE_SIZE));
}

inline bool Tile::get(const vector2& pos) const
{
#ifdef PACK_TILE_CONTENT
    uint32_t byteY = pos.Y / 8;
    uint32_t offY = pos.Y % 8;

    uint8_t val = mData[pos.X * TILE_BYTE_SIZE + byteY] & (1 << offY);
    return val != 0;
#else
    return mData[pos.X * TILE_SIZE + pos.Y];
#endif
}


#endif // TILE_H
