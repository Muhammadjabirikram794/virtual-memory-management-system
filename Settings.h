#pragma once
#include <cmath>
#include <string>

using namespace std;

struct Settings {
    unsigned int ramMemorySize = 0;
    unsigned int sizeOfOnePage = 1;
    unsigned int quickCacheSize = 0;

    double quickCacheDelay = 0.0;
    double ramDelay = 0.0;
    double hardDriveDelay = 0.0;

    unsigned int getShiftAmount() const {
        return log2(sizeOfOnePage);
    }

    unsigned int getOffsetMask() const {
        return sizeOfOnePage - 1;
    }

    unsigned int getTotalRamFrames() const {
        return ramMemorySize / sizeOfOnePage;
    }
};
