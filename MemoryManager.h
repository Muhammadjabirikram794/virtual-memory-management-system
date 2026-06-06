#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include "Settings.h"
#include "ScoreBoard.h"
#include "PageReplacer.h"

using namespace std;

struct PageData {
    unsigned int frameBoxNumber;
    bool isValid = false;
    bool needsSaving = false;
};

class QuickCache {
    struct CacheBox { unsigned int pageNumber; unsigned int frameBox; bool filled = false; };
    vector<CacheBox> boxes;
    unsigned int maxBoxes;
public:
    QuickCache(unsigned int size) : maxBoxes(size) {
        if (size > 0) boxes.resize(size);
    }

    int checkCache(unsigned int pageNumber) {
        if (maxBoxes == 0) return -1;
        for (auto& box : boxes) {
            if (box.filled && box.pageNumber == pageNumber) return box.frameBox;
        }
        return -1;
    }

    void saveToCache(unsigned int pageNumber, unsigned int frameBox) {
        if (maxBoxes == 0) return;
        static int nextSlot = 0;
        boxes[nextSlot] = { pageNumber, frameBox, true };
        nextSlot = (nextSlot + 1) % maxBoxes;
    }

    void deleteFromCache(unsigned int pageNumber) {
        if (maxBoxes == 0) return;
        for (auto& box : boxes) {
            if (box.filled && box.pageNumber == pageNumber) {
                box.filled = false;
            }
        }
    }
};



class MemoryManager {
    Settings mySettings;
    ScoreBoard& myScoreboard;
    QuickCache myCache;
    unordered_map<unsigned int, PageData> ramTable;
    PageReplacer* myReplacer;
    list<unsigned int> emptyRamBoxes;

    // Fixed version of the function from image_1e93b7.png
    unsigned int findPageInFrame(unsigned int frameNumber) {
        for (auto it = ramTable.begin(); it != ramTable.end(); ++it) {
            // it->first is the Page Number, it->second is the PageData
            if (it->second.isValid && it->second.frameBoxNumber == frameNumber) {
                return it->first;
            }
        }
        return 0;
    }

    void fixMissingPage(unsigned int pageNumber) {
        myScoreboard.misses++;
        myScoreboard.totalTimePassed += mySettings.hardDriveDelay;

        unsigned int chosenRamBox;

        if (!emptyRamBoxes.empty()) {
            chosenRamBox = emptyRamBoxes.front();
            emptyRamBoxes.pop_front();
        }
        else {
            /*
               FIXED-PRIORITY (STATIC VICTIM)
               Hard-code the victim to Frame 0 every time.
            */
            chosenRamBox = 0;

            // Identify which page is currently sitting in Frame 0
            unsigned int victimPage = findPageInFrame(chosenRamBox);

            // Handle Write-Back if the page in Frame 0 was modified
            if (ramTable.count(victimPage) && ramTable[victimPage].needsSaving) {
                myScoreboard.hardDriveWrites++;
                myScoreboard.totalTimePassed += mySettings.hardDriveDelay;
            }

            // Evict the victim
            ramTable[victimPage].isValid = false;
            myCache.deleteFromCache(victimPage);
        }

        // Load the new page into Frame 0
        ramTable[pageNumber] = { chosenRamBox, true, false };
    }

public:
    MemoryManager(Settings settings, ScoreBoard& board, PageReplacer* replacer)
        : mySettings(settings), myScoreboard(board), myCache(settings.quickCacheSize), myReplacer(replacer)
    {
        for (unsigned int i = 0; i < mySettings.getTotalRamFrames(); ++i) {
            emptyRamBoxes.push_back(i);
        }
    }

    void readOrWrite(unsigned int memoryAddress, bool isWritingData) {
        myScoreboard.totalOperations++;
        unsigned int pageNumber = memoryAddress >> mySettings.getShiftAmount();

        myScoreboard.totalTimePassed += mySettings.quickCacheDelay;
        int ramBox = myCache.checkCache(pageNumber);

        if (ramBox != -1) {
            myScoreboard.quickCacheHits++;
        }
        else {
            myScoreboard.totalTimePassed += mySettings.ramDelay;

            if (ramTable.count(pageNumber) && ramTable[pageNumber].isValid) {
                myScoreboard.ramHits++;
                ramBox = ramTable[pageNumber].frameBoxNumber;
                myCache.saveToCache(pageNumber, ramBox);
            }
            else {
                fixMissingPage(pageNumber);
                ramBox = ramTable[pageNumber].frameBoxNumber;
                myCache.saveToCache(pageNumber, ramBox);
            }
        }

        if (isWritingData) {
            ramTable[pageNumber].needsSaving = true;
        }

        // We still call this to keep the replacer object updated, 
        // though fixMissingPage now ignores its 'kickOut' logic.
        myReplacer->markAsUsed(pageNumber);
    }
};