#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <deque>
#include <unordered_map>
#include "Settings.h"

using namespace std;

class FileReader {
public:
    static Settings loadSettings(const string& filename) {
        Settings mySettings;
        ifstream file(filename);
        string currentLine;

        if (!file.is_open()) {
            return mySettings;
        }

        while (getline(file, currentLine)) {
            if (currentLine.empty() || currentLine[0] == '#') continue;

            istringstream lineStream(currentLine);
            string settingName;

            if (getline(lineStream, settingName, ':')) {
                string settingValue;
                if (getline(lineStream, settingValue)) {
                    if (settingName == "RAM_SIZE") mySettings.ramMemorySize = stoul(settingValue);
                    else if (settingName == "PAGE_SIZE") mySettings.sizeOfOnePage = stoul(settingValue);
                    else if (settingName == "TLB_SIZE") mySettings.quickCacheSize = stoul(settingValue);
                    else if (settingName == "TLB_LATENCY") mySettings.quickCacheDelay = stod(settingValue);
                    else if (settingName == "RAM_LATENCY") mySettings.ramDelay = stod(settingValue);
                    else if (settingName == "DISK_LATENCY") mySettings.hardDriveDelay = stod(settingValue);
                }
            }
        }
        return mySettings;
    }

    static unordered_map<unsigned int, deque<int>> lookIntoFuture(string filename, unsigned int shiftAmount) {
        unordered_map<unsigned int, deque<int>> futureTimeline;
        ifstream file(filename);
        string line;
        int currentStep = 0;

        if (!file.is_open()) {
            return futureTimeline;
        }

        while (getline(file, line)) {
            if (line.empty()) continue;

            size_t spaceLocation = line.find(' ');
            if (spaceLocation == string::npos) continue;

            string addrStr = line.substr(0, spaceLocation);
            if (addrStr.length() > 2 && (addrStr.substr(0, 2) == "0x" || addrStr.substr(0, 2) == "0X")) {
                addrStr = addrStr.substr(2);
            }

            unsigned int address = stoul(addrStr, nullptr, 16);
            unsigned int pageNumber = address >> shiftAmount;

            futureTimeline[pageNumber].push_back(currentStep);
            currentStep++;
        }
        return futureTimeline;
    }
};
