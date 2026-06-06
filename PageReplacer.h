#pragma once
#include <list>
#include <deque>
#include <vector>
#include <unordered_map>

using namespace std;

class PageReplacer {
public:
    virtual unsigned int kickOutVictim() = 0;
    virtual void markAsUsed(unsigned int pageNumber) = 0;
    virtual ~PageReplacer() {}
};

class SimpleFIFO : public PageReplacer {
    deque<unsigned int> lineOfPages;
public:
    void markAsUsed(unsigned int pageNumber) override {
        bool alreadyInLine = false;
        for (unsigned int page : lineOfPages) {
            if (page == pageNumber) {
                alreadyInLine = true;
                break;
            }
        }

        if (!alreadyInLine) {
            lineOfPages.push_back(pageNumber);
        }
    }

    unsigned int kickOutVictim() override {
        unsigned int victim = lineOfPages.front();
        lineOfPages.pop_front();
        return victim;
    }
};

class SimpleLRU : public PageReplacer {
    list<unsigned int> historyList;
    unordered_map<unsigned int, list<unsigned int>::iterator> memoryMap;
public:
    void markAsUsed(unsigned int pageNumber) override {
        if (memoryMap.count(pageNumber)) {
            historyList.erase(memoryMap[pageNumber]);
        }

        historyList.push_front(pageNumber);
        memoryMap[pageNumber] = historyList.begin();
    }

    unsigned int kickOutVictim() override {
        unsigned int victim = historyList.back();
        historyList.pop_back();
        memoryMap.erase(victim);
        return victim;
    }
};

class FutureOPT : public PageReplacer {
    unordered_map<unsigned int, deque<int>>& futureTimeline;
    vector<unsigned int> pagesInRam;
public:
    FutureOPT(unordered_map<unsigned int, deque<int>>& timeline) : futureTimeline(timeline) {}

    void markAsUsed(unsigned int pageNumber) override {
        if (!futureTimeline[pageNumber].empty()) {
            futureTimeline[pageNumber].pop_front();
        }

        bool alreadyInRam = false;
        for (unsigned int page : pagesInRam) {
            if (page == pageNumber) {
                alreadyInRam = true;
                break;
            }
        }

        if (!alreadyInRam) {
            pagesInRam.push_back(pageNumber);
        }
    }

    unsigned int kickOutVictim() override {
        unsigned int ultimateVictim = 0;
        int furthestTime = -1;
        int targetIndex = -1;

        for (int i = 0; i < pagesInRam.size(); i++) {
            unsigned int page = pagesInRam[i];

            if (futureTimeline[page].empty()) {
                ultimateVictim = page;
                targetIndex = i;
                break;
            }

            int nextUse = futureTimeline[page].front();

            if (nextUse > furthestTime) {
                furthestTime = nextUse;
                ultimateVictim = page;
                targetIndex = i;
            }
        }

        if (targetIndex != -1) {
            pagesInRam.erase(pagesInRam.begin() + targetIndex);
        }

        return ultimateVictim;
    }
};
