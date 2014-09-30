#include "TileUpdater.h"
#include "Game.h"

TileUpdater::TileUpdater(const Game &game) : mGame(game), mWorking(0)
{
    // Initialize all threads in the beginning
    unsigned int numCPU = std::thread::hardware_concurrency();
    auto numThreads = max<unsigned int>(1, numCPU); // might return 0...

    cout << "Detected " << numCPU << " (virtual) cores. Starting " << numThreads << " threads." << endl;

    for(unsigned int i = 0; i < numThreads; ++i) {
        mThreads.push(std::thread(&TileUpdater::work, this));
    }
}

void TileUpdater::update(const unordered_map<int64_t, Tile*>& tiles)
{
   for(auto it: tiles)
   {
       mStackLock.lock();
       mWorkStack.push(it.second);
       mStackLock.unlock();
   }

   // block until work is done
   bool done = false;

   while(!done)
   {
       chrono::milliseconds dur(1);
       this_thread::sleep_for(dur);

       mStackLock.lock();
       done = mWorkStack.empty() && (mWorking == 0);
       mStackLock.unlock();
   }
}

bool TileUpdater::running() const
{
    return mGame.isOk();
}

TileUpdater::~TileUpdater()
{
   while(!mThreads.empty())
   {
        mThreads.top().join();
        mThreads.pop();
   }
}

void TileUpdater::work()
{
    while(this->running())
    {
        bool hasWork = true;

        while(hasWork)
        {
            auto next = getNextTile();

            if(next)
            {
                next->update();
                mWorking--;
            }
            else
            {
                hasWork = false;
            }
        }

        // Wait a little befor continuing with next set of work
        chrono::milliseconds dur(1);
        this_thread::sleep_for(dur);
    }
}

Tile* TileUpdater::getNextTile()
{
    mStackLock.lock();
    Tile* result = nullptr;

    if(!mWorkStack.empty()) {
        result = mWorkStack.top();
        mWorkStack.pop();
        mWorking++;
    }

    mStackLock.unlock();
    return result;
}
