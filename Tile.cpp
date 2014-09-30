#include "Tile.h"
#include "Game.h"
#include <assert.h>
#include <string.h>

Tile::Tile(const Game& game, const vector2& pos)
    : mGame(game), mPosition(pos), mTileSize(mGame.getTileSize())
{
#ifndef RECT_MAP
    const auto datasize = mTileSize * mTileSize;
    mData = unique_ptr<bool[]>(new bool[datasize]);
    memset(mData.get(), 0, datasize);
#endif
}

Tile* Tile::duplicate() const
{
    Tile *newTile = new Tile(mGame, mPosition);

#ifdef RECT_MAP
    newTile->mActiveRects = mActiveRects;
#else
    const auto datasize = mTileSize * mTileSize;
    memcpy(newTile->mData.get(), mData.get(), datasize);
#endif

    return newTile;
}

void Tile::set(const vector2& pos)
{
#ifdef RECT_MAP
    mActiveRects[pos.toFlatInt()] = true;
#else
    mData[pos.X * mTileSize + pos.Y] = true;
#endif
}

void Tile::clear(const vector2& pos)
{
#ifdef RECT_MAP
    auto it = mActiveRects.find(pos.toFlatInt());

    if(it != mActiveRects.end())
        mActiveRects.erase(it);
#else
    mData[pos.X * mTileSize + pos.Y] = false;
#endif
}

bool Tile::hasActiveRects() const
{
#ifdef RECT_MAP
    return mActiveRects.size() > 0;
#else
    for(uint32_t i = 0; i < mTileSize * mTileSize; ++i)
    {
        if(mData[i])
            return true;
    }

    return false;
#endif
}

vector<vector2> Tile::getActiveRects() const
{
    vector<vector2> result;

#ifdef RECT_MAP
    for(auto it: mActiveRects)
    {
        result.push_back(vector2(it.first));
    }
#else
    for(uint32_t x = 0; x < mTileSize; ++x)
    {
        for(uint32_t y = 0; y < mTileSize; ++y)
        {
            if(get(vector2(x,y)))
                result.push_back(vector2(x,y));
        }
    }
#endif

    return result;
}

void Tile::update()
{
    for(uint32_t x = 0; x < mGame.getTileSize(); ++x)
    {
        for(uint32_t y = 0; y < mGame.getTileSize(); ++y)
        {
            updateRect(vector2(x,y));
        }
    }
}

void Tile::updateRect(const vector2& pos)
{
    uint32_t count = 0;
    const Tile* previous = nullptr;

    if(mGame.hasPreviousTile(mPosition))
        previous = &mGame.getPreviousTile(mPosition);

    for(int32_t i = -1; i <= 1; ++i)
    {
        for(int32_t j = -1; j <= 1; ++j)
        {
            if(i == 0 && j == 0)
                continue;

            vector2 cpos = vector2(i,j) + pos;
            if(inBoundaries(cpos))
            {
                if(previous)
                    count += previous->get(cpos);
            }
            else
                count += mGame.wasActive(getRectPos() + cpos);
        }
    }

    if(count < 2) {
        // Underpopulation
        clear(pos);
    } else if(count > 3) {
        // Overcrowding
        clear(pos);
    } else if(count == 3) {
        // Reproduction
        set(pos);
    }
}
