#include "Tile.h"
#include "Game.h"
#include <assert.h>
#include <string.h>

Tile::Tile(const Game& game, const vector2& pos)
    : mGame(game), mPosition(pos)
{
    const auto datasize = TILE_SIZE * TILE_BYTE_SIZE;

    mData = unique_ptr<uint8_t[]>(new uint8_t[datasize]);
    memset(mData.get(), 0, datasize);
}

Tile* Tile::duplicate() const
{
    Tile *newTile = new Tile(mGame, mPosition);
    const auto datasize = TILE_SIZE * TILE_BYTE_SIZE;

    memcpy(newTile->mData.get(), mData.get(), datasize);

    return newTile;
}

void Tile::set(const vector2& pos)
{
    uint32_t byteY = pos.Y / 8;
    uint32_t offY = pos.Y % 8;

    mData[pos.X * TILE_BYTE_SIZE + byteY] |= (1 << offY);
}

void Tile::clear(const vector2& pos)
{
    uint32_t byteY = pos.Y / 8;
    uint32_t offY = pos.Y % 8;

    mData[pos.X * TILE_BYTE_SIZE + byteY] &= ~(1 << offY);
}


bool Tile::get(const vector2& pos) const
{
    uint32_t byteY = pos.Y / 8;
    uint32_t offY = pos.Y % 8;

    uint8_t val = mData[pos.X * TILE_BYTE_SIZE + byteY] & (1 << offY);
    return val != 0;
}

bool Tile::hasActiveRects() const
{
    for(uint32_t i = 0; i < TILE_SIZE * TILE_BYTE_SIZE; ++i)
    {
        if(mData[i] != 0)
            return true;
    }

    return false;
}

vector<vector2> Tile::getActiveRects() const
{
    vector<vector2> result;

    if(!hasActiveRects())
    {
        return result;
    }

    for(uint32_t x = 0; x < TILE_SIZE; ++x)
    {
        for(uint32_t y = 0; y < TILE_SIZE; ++y)
        {
            if(get(vector2(x,y)))
                result.push_back(vector2(x,y));
        }
    }

    return result;
}

void Tile::update()
{
    for(uint32_t x = 0; x < TILE_SIZE; ++x)
    {
        for(uint32_t y = 0; y < TILE_SIZE; ++y)
        {
            uint32_t count = 0;
            vector2 root = getRectPos() + vector2(x,y);

            count += mGame.wasActive(root + vector2(-1,0));
            count += mGame.wasActive(root + vector2(1,0));
            count += mGame.wasActive(root + vector2(0,1));
            count += mGame.wasActive(root + vector2(0,-1));

            count += mGame.wasActive(root + vector2(-1,-1));
            count += mGame.wasActive(root + vector2(-1,1));
            count += mGame.wasActive(root + vector2(1,1));
            count += mGame.wasActive(root + vector2(1,-1));

            if(count < 2) {
                // Underpopulation
                clear(vector2(x,y));
            } else if(count > 3) {
                // Overcrowding
                clear(vector2(x,y));
            } else if(count == 3) {
                // Reproduction
                set(vector2(x,y));
            }
        }
    }
}
