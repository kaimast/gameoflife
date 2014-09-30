#include "Game.h"

#include <chrono>
#include <thread>

void runGraphics(Game& game, Graphics& graphics)
{
    if(!graphics.init())
    {
        cout << "Failed to initialize graphics..." << endl;
        game.stop();
        return;
    }

    while(game.isOk())
    {
        graphics.handleInput();
        graphics.draw();

        chrono::milliseconds dur(50);
        this_thread::sleep_for(dur);
    }
}

Game::Game(bool benchmark, uint32_t tileSize, uint32_t initialSeed)
    : mTileUpdater(*this), mIsBenchmarking(benchmark), mTileSize(tileSize), mInitialSeed(initialSeed)
{
}

Game::~Game()
{
    mOk = false;

    // Check if graphics are enabled...
    if(mGraphics)
        mGraphicsThread.join();

    for(auto it: mTiles)
    {
        delete it.second;
    }
}

void Game::set(const vector2 &pos)
{
    auto mappos = toTilePosition(pos, mTileSize);
    auto relpos = toRelativePosition(pos, mTileSize);

    if(!hasTile(mappos))
        createTile(mappos);

    getTile(mappos).set(relpos);
}

void Game::increaseSpeed()
{
    mSpeed += mSpeedStep;
}

void Game::decreaseSpeed()
{
    if(mSpeed > 0)
        mSpeed -= mSpeedStep;
}

bool Game::isPaused() const
{
    return mPaused;
}

void Game::togglePaused()
{
    mPaused = !mPaused;
}

uint32_t Game::getSpeed() const
{
    return mSpeed;
}

void Game::createInitialSetup()
{
    cout << "Starting with a grid of " << mInitialSeed << "x" << mInitialSeed << " tiles" << endl;

    for(uint32_t i = 0; i < mInitialSeed; ++i)
    {
        for(uint32_t j = 0; j < mInitialSeed; ++j)
        {
            bool active = rand() % 2;

            if(active)
                set(vector2(20 + i, 20 + j));
        }
    }
}

bool Game::init(bool showGraphics)
{
    if(showGraphics)
    {
        mGraphics = unique_ptr<Graphics>{new Graphics(*this)};
        mGraphicsThread = std::thread(runGraphics, std::ref(*this), std::ref(*mGraphics));
    }

    createInitialSetup();

    return true;
}

void Game::createTile(const vector2 &pos)
{
    if(hasTile(pos))
        return;

    Tile* tile = new Tile(*this, pos);
    mTiles[pos.toFlatInt()] = tile;
}

bool Game::wasActive(const vector2& pos) const
{
    vector2 tilePos = toTilePosition(pos, mTileSize);
    vector2 relPos = toRelativePosition(pos, mTileSize);

    auto it = mOldTiles.find(tilePos.toFlatInt());

    if(it == mOldTiles.end())
        return false;

    return it->second->get(relPos);
}

bool Game::hasTile(const vector2& pos) const
{
    return mTiles.find(pos.toFlatInt()) != mTiles.end();
}

void Game::doGameLogic()
{
    mRound++;

    // set up old tiles
    for(auto it: mTiles)
    {
        mOldTiles[it.first] = it.second->duplicate();
    }

    // create border tlies
    for(auto it: mOldTiles)
    {
        auto pos = it.second->getPosition();

        createTile(pos + vector2(1,0));
        createTile(pos + vector2(1,1));
        createTile(pos + vector2(1,-1));
        createTile(pos + vector2(0,1));
        createTile(pos + vector2(0,-1));
        createTile(pos + vector2(-1,0));
        createTile(pos + vector2(-1,1));
        createTile(pos + vector2(-1,-1));
    }

    mTileUpdater.update(mTiles);

    //clear old tiles
    for(auto it: mOldTiles)
    {
        delete it.second;
    }

    // garbe collect empty tiles
    // prevent render thread from reading while we clean up
    mRendering.lock();
    for(auto it = mTiles.begin(); it != mTiles.end();)
    {
        if(!it->second->hasActiveRects())
            it = mTiles.erase(it);
        else
            ++it;
    }
    mRendering.unlock();

    mOldTiles.clear();
}

void Game::update()
{
    auto start = chrono::high_resolution_clock::now();

    if(!mPaused) {
        benchmark.generationStart();
        doGameLogic();
        benchmark.generationDone();
     }

    if(benchmark.isDone() && mIsBenchmarking) {
        this->stop();
        return;
    }

    auto diff = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start);
    auto sleeptime = mSpeed - diff.count();

    if(sleeptime > 0) {
        chrono::milliseconds dur(sleeptime);
        this_thread::sleep_for(dur);
    }
}

Tile& Game::getTile(const vector2& pos)
{
    auto key = pos.toFlatInt();
    auto it = mTiles.find(key);

    if(it == mTiles.end())
        throw std::runtime_error("No such tile");

    return *it->second;
}

const Tile& Game::getTile(const vector2& pos) const
{
    auto key = pos.toFlatInt();
    auto it = mTiles.find(key);

    if(it == mTiles.end())
        throw std::runtime_error("No such tile");

    return *it->second;
}

const Tile& Game::getPreviousTile(const vector2& pos) const
{
    auto key = pos.toFlatInt();
    auto it = mOldTiles.find(key);

    if(it == mOldTiles.end())
        throw std::runtime_error("No such tile");

    return *it->second;
}

