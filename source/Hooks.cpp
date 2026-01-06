#include "syati.h"
#include "ExtGameDataUtil.h"
#include "SuperFlag.h"

namespace sf {
    extern void *gSuperFlagTbl;
}

namespace {
    AchievementChunk* getCurrentChunk() {
        ExtGameDataHolder *pHolder = ExtGameDataUtil::getCurrentGameDataHolder();

        if (pHolder) {
            return pHolder->mAchievementChunk;
        }

        return NULL;
    }
    class Info {
        public:
        JMapInfo* info;
        Info() {
            info = new JMapInfo();
            info->attach(sf::gSuperFlagTbl);
        }
        ~Info() {delete info;}
    };
    // Hack to globally store the SuperFlag bcsv. 
    // JMapInfo does not write to the void* ever.
    Info gInfo = Info();
}

const char* startNames[] = {
    "GalaxyName",
    "ScenarioNo",
    "ScenarioNum",
    "SelectedScenarioNo",
    "SelectedScenarioNum",
};

const int startNamesSize = sizeof(startNames) / sizeof(char*);

void onSceneStart() {
    AchievementChunk* chunk = getCurrentChunk();
    if (!chunk)
        return;
    const JMapInfo* info = gInfo.info;
    for (int i = 0; i < info->mData->mNumEntries; i++) {
        JMapInfoIter iter = JMapInfoIter(info, i);
        const char* type;
        iter.getValue<const char*>("FieldType", &type);
        for (int j = 0; j < startNamesSize; j++) {
            if (MR::isEqualString(type, startNames[j])) {
                chunk->mArray.set(i, sf::isFlagActive(iter));
            }
        }
    }
}

const char* loopNames[] = {
    "StarPieceNum",
    "StarBitNum",
    "CoinNum",
    "PurpleCoinNum"
};

const int loopSize = sizeof(loopNames) / sizeof(char*);

void onSceneLoop() {
    AchievementChunk* chunk = getCurrentChunk();
    if (!chunk)
        return;
    const JMapInfo* info = gInfo.info;
    for (int i = 0; i < info->mData->mNumEntries; i++) {
        JMapInfoIter iter = JMapInfoIter(info, i);
        const char* type;
        iter.getValue<const char*>("FieldType", &type);
        for (int j = 0; j < loopSize; j++) {
            if (MR::isEqualString(type, loopNames[i])) {
                chunk->mArray.set(i, sf::isFlagActive(iter));
            }
        }
    }
}