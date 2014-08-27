#include "Game.h"

#include <chrono>
#include <thread>

Game::~Game()
{
    for(auto it: mTiles)
    {
        delete it.second;
    }
}

bool Game::init()
{
    // Create Subsystems
    mGraphics = unique_ptr<Graphics>{new Graphics(*this)};

    if(!mGraphics->init())
        return false;

    // Initial Setup
    Tile& initialTile = createTile(vector2(0,0));

    initialTile.set(vector2(10,10));
    initialTile.set(vector2(10,11));
    initialTile.set(vector2(10,12));
    initialTile.set(vector2(10,13));
    initialTile.set(vector2(10,14));

    initialTile.set(vector2(11,10));
    initialTile.set(vector2(11,12));
    initialTile.set(vector2(11,14));

    initialTile.set(vector2(12,10));
    initialTile.set(vector2(12,11));
    initialTile.set(vector2(12,12));
    initialTile.set(vector2(12,13));
    initialTile.set(vector2(12,14));


    initialTile.set(vector2(13,10));
    initialTile.set(vector2(14,10));
    initialTile.set(vector2(15,10));
    initialTile.set(vector2(16,10));
    initialTile.set(vector2(16,11));\

    return true;
}

Tile& Game::createTile(const vector2 &pos)
{
    Tile* tile = new Tile(*this, pos);

    mTiles[toMapPos(pos)] = tile;
    return getTile(pos);
}

bool Game::wasActive(const vector2& pos) const
{
    vector2 tilePos = toTilePosition(pos);
    vector2 relPos = toRelativePosition(pos);

    auto mappos = toMapPos(tilePos);
    auto it = mOldTiles.find(mappos);

    if(it == mOldTiles.end())
        return false;

    return it->second->get(relPos);
}

bool Game::hasTile(const vector2& pos) const
{
    auto mappos = toMapPos(pos);

    return mTiles.find(mappos) != mTiles.end();
}

void Game::update()
{
    const auto speed = 50;

    auto start = chrono::high_resolution_clock::now();
    mRound++;

    cout << "Round " << mRound << endl;
    mGraphics->handleInput();
    mGraphics->draw();

    // set up old tiles
    for(auto it: mTiles)
    {
        mOldTiles[it.first] = it.second->duplicate();
    }

    for(auto it: mTiles)
    {
        it.second->update();
    }

    //clear old tiles
    for(auto it: mOldTiles)
    {
        delete it.second;
    }

    mOldTiles.clear();

    auto diff = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start);
    auto sleeptime = diff.count() - speed;

    if(sleeptime > 0) {
        chrono::milliseconds dur(sleeptime);
        this_thread::sleep_for(dur);
    }
}

Tile& Game::getTile(const vector2& pos)
{
    return *mTiles[toMapPos(pos)];
}

const Tile& Game::getTile(const vector2& pos) const
{
    auto key = toMapPos(pos);
    auto it = mTiles.find(key);

    if(it == mTiles.end())
        throw std::runtime_error("No such tile");

    return *it->second;
}

