#include "Achievements.h"
#include "SuperFlag.h"
#include "Game/Util/HashUtil.h"

namespace sf {
    extern void *gSuperFlagTbl;
}

AchievementChunk::AchievementChunk() : mArray(0) {
    initializeData();
}

u32 AchievementChunk::getSignature() const {
    return 'ACH1';
}

u32 AchievementChunk::makeHeaderHashCode() const {
    return MR::getHashCode("AchievementChunk");
}

u32 AchievementChunk::serialize(u8 *pPosition, u32) const {
    u16* pCursor = (u16*)pPosition;
    JMapInfo info = JMapInfo();
    info.attach(sf::gSuperFlagTbl);
    for (int i = 0; i < mCount; i++) {
        JMapInfoIter iter = JMapInfoIter(&info, i);
        const char* name;
        iter.getValue<const char*>("FlagName", &name);
        if (mArray.isOn(i)) {
            *pCursor++ = MR::getHashCode(name) & 0xFFFF;
        }
    }
    return 0;
}

u32 AchievementChunk::deserialize(const u8 *pPosition, u32) {
    const u16* pCursor = (const u16*)pPosition;
    JMapInfo info = JMapInfo();
    info.attach(sf::gSuperFlagTbl);
    for (int i = 0; i < mCount; i++) {
        JMapInfoIter iter = JMapInfoIter(&info, i);
        const char* name;
        iter.getValue<const char*>("FlagName", &name);
        u16 code = MR::getHashCode(name) & 0xFFFF;
        if (*pCursor++ == code) {
            mArray.set(i, true);
        } else {
            mArray.set(i, false);
        }
    }
    return 0;
}

void AchievementChunk::initializeData() {
    JMapInfo info = JMapInfo();
    info.attach(sf::gSuperFlagTbl);
    mArray = MR::BitArray(info.mData->mNumEntries);
    mCount = info.mData->mNumEntries;
}

extern const unsigned char start__9GameSceneFv;