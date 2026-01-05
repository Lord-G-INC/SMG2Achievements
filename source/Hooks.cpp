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
    JMapInfo info = JMapInfo();
    info.attach(sf::gSuperFlagTbl);
    for (int i = 0; i < info.mData->mNumEntries; i++) {
        JMapInfoIter iter = JMapInfoIter(&info, i);
        const char* type;
        iter.getValue<const char*>("FieldType", &type);
        for (int j = 0; j < startNamesSize; i++) {
            if (MR::isEqualString(type, startNames[j])) {
                chunk->mArray.set(i, sf::isFlagActive(iter));
            }
        }
    }
}