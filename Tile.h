#ifndef TILE_H
#define TILE_H

#include "defines.h"
#include "vector2.h"
#include <vector>
#include <unordered_map>

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

#ifdef RECT_MAP
    //TODO find a nicer datastructure for this
    // value is not really needed...
    unordered_map<uint64_t, bool> mActiveRects;
#else
#ifdef PACK_TILE_CONTENT
    unique_ptr<uint8_t[]> mData;
#else
    unique_ptr<bool[]> mData;
#endif
#endif

    const Game& mGame;
    const vector2 mPosition;

    const uint32_t mTileSize;
};

inline const vector2 Tile::getPosition() const
{
    return mPosition;
}

inline const vector2 Tile::getRectPos() const
{
    return vector2(mPosition.X * mTileSize, mPosition.Y * mTileSize);
}

inline bool Tile::inBoundaries(const vector2 &pos) const
{
    return (pos.X >= 0 && pos.X < static_cast<int32_t>(mTileSize)) && (pos.Y >= 0 && pos.Y < static_cast<int32_t>(mTileSize));
}

inline bool Tile::get(const vector2& pos) const
{
#ifdef RECT_MAP
    return mActiveRects.find(pos.toFlatInt()) != mActiveRects.end();
#else
    return mData[pos.X * mTileSize + pos.Y];
#endif
}


#endif // TILE_H
