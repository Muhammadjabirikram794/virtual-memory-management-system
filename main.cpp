#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <unordered_map>

#include "Settings.h"
#include "ScoreBoard.h"
#include "FileReader.h"
#include "PageReplacer.h"
#include "MemoryManager.h"

using namespace std;

int main() {
    Settings appSettings = FileReader::loadSettings("config.txt");

    cout << "\n--- Current System Configuration ---\n";
    cout << "RAM Size: " << appSettings.ramMemorySize << " bytes\n";
    cout << "Page Size: " << appSettings.sizeOfOnePage << " bytes\n";
    cout << "Total RAM Frames: " << appSettings.getTotalRamFrames() << "\n";
    cout << "TLB Size: " << appSettings.quickCacheSize << " entries\n";
    cout << "TLB Latency: " << appSettings.quickCacheDelay << " ns\n";
    cout << "RAM Latency: " << appSettings.ramDelay << " ns\n";
    cout << "Disk Latency: " << appSettings.hardDriveDelay << " ns\n";
    cout << "------------------------------------\n\n";

    ScoreBoard myScoreboard;
    unordered_map<unsigned int, deque<int>> futureTimeline;

    int chosenAlgorithm;
    cout << "Select Algorithm (0: FIFO, 1: LRU, 2: OPT): ";
    if (!(cin >> chosenAlgorithm)) {
        chosenAlgorithm = 2;
    }

    PageReplacer* activeAlgorithm = nullptr;

    if (chosenAlgorithm == 0) {
        activeAlgorithm = new SimpleFIFO();
    }
    else if (chosenAlgorithm == 1) {
        activeAlgorithm = new SimpleLRU();
    }
    else if (chosenAlgorithm == 2) {
        cout << "Pre-scanning trace.txt for OPT algorithm...\n";
        futureTimeline = FileReader::lookIntoFuture("trace.txt", appSettings.getShiftAmount());
        activeAlgorithm = new FutureOPT(futureTimeline);
    }
    else {
        cout << "Invalid choice. Exiting.\n";
        return 1;
    }

    MemoryManager mmu(appSettings, myScoreboard, activeAlgorithm);

    ifstream traceFile("trace.txt");
    string line;

    if (!traceFile.is_open()) {
        cout << "Error: Could not open trace.txt!\n";
        delete activeAlgorithm;
        return 1;
    }

    cout << "Running simulation... Please wait.\n";

    while (getline(traceFile, line)) {
        if (line.empty()) continue;

        try {
            size_t spaceLocation = line.find(' ');
            if (spaceLocation == string::npos) continue;

            string addrStr = line.substr(0, spaceLocation);
            if (addrStr.length() > 2 && (addrStr.substr(0, 2) == "0x" || addrStr.substr(0, 2) == "0X")) {
                addrStr = addrStr.substr(2);
            }

            unsigned int address = stoul(addrStr, nullptr, 16);
            bool isWriteOperation = (line.substr(spaceLocation + 1, 1) == "W");

            mmu.readOrWrite(address, isWriteOperation);
        }
        catch (...) {
            continue;
        }
    }

    myScoreboard.showFinalScore();
    delete activeAlgorithm;

    return 0;
}