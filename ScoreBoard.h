#pragma once
#include <iostream>

using namespace std;

class ScoreBoard {
public:
    long totalOperations = 0;
    long quickCacheHits = 0;
    long ramHits = 0;
    long misses = 0;
    long hardDriveWrites = 0;
    long long totalTimePassed = 0;

    void showFinalScore() {
        cout << endl;
        cout << "=== Final Performance Scoreboard ===" << endl;
        cout << "Total Memory Operations: " << totalOperations << endl;
        cout << "Total Faults (Page Faults): " << misses << endl;
        cout << "Quick Cache (TLB) Hit Rate: " << (double)quickCacheHits / totalOperations * 100 << endl;
        cout << "Total Disk Writes: " << hardDriveWrites << endl;
        cout << "Total Simulated Time: " << totalTimePassed << endl;

        if (totalOperations > 0) {
            cout << "Average Time Per Operation (EAT): " << totalTimePassed / totalOperations << endl;
        }
    }
};
