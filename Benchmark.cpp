#include "Benchmark.h"

void Benchmark::generationStart()
{
    mLastStart = chrono::system_clock::now();

    if(mGenerationCount % 100 == 1) {
        mCenturyStart = mLastStart;
    }
}

void Benchmark::generationDone()
{
    mGenerationCount++;

    if(mGenerationCount % 100 == 0) {

        auto centuryDur = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now() - mCenturyStart);
        auto count = centuryDur.count();

        cout << mCenturyCount+1 << ". century took " << count << " ms" << endl;
        cout << "Avg Generation Speed: " << (count/100.0f) << " ms" << endl;

        mCenturyDurations[mCenturyCount] = centuryDur;
        mCenturyCount++;

        if(mCenturyCount == 10) {
            uint32_t sum = 0;

            for(auto dur : mCenturyDurations) {
                sum += dur.second.count();
            }

            cout << "Overall Average: " << (sum / (100.0f * mCenturyCount)) << "ms" << endl;
        }
    }
}
