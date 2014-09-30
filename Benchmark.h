#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "defines.h"
#include <chrono>
#include <map>

class Benchmark
{
public:
    void generationStart();
    void generationDone();

    bool isDone() const {
        return mCenturyCount >= 10;
    }

private:
    uint32_t mGenerationCount = 0;
    uint32_t mCenturyCount = 0;

    chrono::system_clock::time_point mCenturyStart;
    chrono::system_clock::time_point mLastStart;

    map<uint32_t, chrono::milliseconds> mCenturyDurations;
};

#endif // BENCHMARK_H
