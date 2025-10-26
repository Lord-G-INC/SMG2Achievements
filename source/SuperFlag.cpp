#pragma once

#include "SuperFlag.h"

namespace sf
{
    void *gSuperFlagTbl = pt::loadArcAndFile("/SystemData/SuperFlag.arc", "/SuperFlag.bcsv", 0);

    bool isFlagActive(s32 index)
    {
        JMapInfo JMapTable = JMapInfo();
        JMapTable.attach(gSuperFlagTbl);
        // OSReport("Num data %d\n", JMapTable.end().mIndex);
        if (index >= JMapTable.end().mIndex)
            return false;
        else if (index == -1)
            return true;
        JMapInfoIter iter = JMapInfoIter(&JMapTable, index);
        return isFlagActive(iter);
    }

    bool isFlagActive(JMapInfoIter iter)
    {
        const char *pFlagType;
        iter.getValue<const char *>("FlagType", &pFlagType);
        const ConditionEntry *entry = &gConditionTable[0];

        for (s32 i = 0; i < gConditionTableEntries; i++)
        {
            entry = &gConditionTable[i];

            if (MR::isEqualString(pFlagType, entry->flagType))
            {
                return (entry->func)(iter);
                break;
            }
        }

        return 0;
    }

    bool isFlagActive(const char *pFlagName)
    {
        // OSReport("Checking flag %s\n", pFlagName);
        if (MR::isEqualString(pFlagName, "True"))
            return true;
        JMapInfo JMapTable = JMapInfo();
        JMapTable.attach(gSuperFlagTbl);

        JMapInfoIter iter = JMapTable.findElement<const char *>("FlagName", pFlagName, 0);
        if (iter != JMapTable.end())
        {
            return isFlagActive(iter);
        }
        else
        {
            return false;
        }
    }

    /**
     * UTILITY FUNCTIONS
     */

    const char *getParam00Str(JMapInfoIter iter)
    {
        const char *pParam00Str;
        iter.getValue<const char *>("Param00Str", &pParam00Str);
        return pParam00Str;
    }

    const char *getGalaxyName(JMapInfoIter iter)
    {
        const char *pParam00Str = getParam00Str(iter);
        if (MR::isEqualString(pParam00Str, "$CurrentGalaxy"))
            return MR::getCurrentStageName();
        else
            return pParam00Str;
    }

    const char *getParamStrNumbered(JMapInfoIter iter, int index)
    {
        char pFieldName[10];
        sprintf(pFieldName, "Param%02dStr", index);
        const char *pValue;
        iter.getValue<const char *>(pFieldName, &pValue);
        return pValue;
    }

    // No, I don't think I will.
    s32 getLife()
    {
        // Note: This crashes on file select.
        MarioHolder *pHolder = MR::getMarioHolder();
        return pHolder->getMarioActor()->mLifeNum;
    }

    GalaxyStatusAccessor getGalaxyStatusAccessor(JMapInfoIter iter)
    {
        const char *pGalaxyName = getGalaxyName(iter);
        return MR::makeGalaxyStatusAccessor(pGalaxyName);
    }

    /**
     * TYPE FUNCTIONS
     * Checks specific flag types to see if they return true or false.
     */

    bool handleTypeAnd(JMapInfoIter iter)
    {
        for (int i = 0; i < 100; i++)
        {
            const char *pValue = getParamStrNumbered(iter, i);
            if (MR::isNullOrEmptyString(pValue))
                break;
            else if (!isFlagActive(pValue))
                return false;
        }
        return true;
    }

    bool handleTypeOr(JMapInfoIter iter)
    {
        for (int i = 0; i < 100; i++)
        {
            const char *pValue = getParamStrNumbered(iter, i);
            if (MR::isNullOrEmptyString(pValue))
                break;
            else if (isFlagActive(pValue))
                return true;
        }
        return false;
    }

    bool handleTypeNot(JMapInfoIter iter)
    {
        const char *pSign = getParam00Str(iter);
        return !isFlagActive(pSign);
    }

    bool handleTypeGalaxyName(JMapInfoIter iter)
    {
        for (int i = 0; i < 100; i++)
        {
            const char *pValue = getParamStrNumbered(iter, i);
            if (MR::isNullOrEmptyString(pValue))
                break;
            else if (MR::isEqualStageName(pValue))
                return true;
        }
        return false;
    }

    /**
     * Comparison type utility functions
     */

    Comparator getComparatorType(const char *pString)
    {
        for (int i = 0; i < sizeof(sComparators) / sizeof(const char *); i++)
        {
            if (MR::isEqualString(sComparators[i], pString))
            {
                return static_cast<Comparator>(i);
            }
        }
        return InvalidComparator;
    }

    bool compareValues(Comparator comp, s32 val1, s32 val2)
    {
        switch (comp)
        {
        case (Equal):
            return val1 == val2;
        case (Greater):
            return val1 > val2;
        case (GreaterEqual):
            return val1 >= val2;
        case (Less):
            return val1 < val2;
        case (LessEqual):
            return val1 <= val2;
        case (NotEqual):
            return val1 != val2;
        default:
            return false;
        }
    }

    bool handleTypeGroupComparisonS32(JMapInfoIter iter, s32 val)
    {
        const char *pSign = getParam00Str(iter);
        const Comparator comparator = getComparatorType(pSign);
        if (comparator == InvalidComparator)
        {
            OSReport("Invalid sign %s\n", pSign);
            return false;
        }

        s32 compVal = 0;
        iter.getValue<s32>("Param00Int", &compVal);

        bool condition = compareValues(comparator, val, compVal);
        // OSReport("Condition %d %s %d. %s\n", val, sComparators[comparator], compVal, condition ? "True" : "False");
        return condition;
    }

    bool handleTypeScenarioNo(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, MR::getCurrentScenarioNo());
    }

    bool handleTypeSelectedScenarioNo(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, MR::getCurrentSelectedScenarioNo());
    }

    bool handleTypePowerStarNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, MR::getPowerStarNum());
    }

    bool handleTypeStockedCoinNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, GameDataFunction::getStockedCoinNum());
    }

    bool handleTypeStockedStarPieceNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, GameDataFunction::getStockedStarPieceNum());
    }

    bool handleTypeStarPieceNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, MR::getStarPieceNum());
    }

    bool handleTypeCoinNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, MR::getCoinNum());
    }

    bool handleTypePurpleCoinNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, MR::getPurpleCoinNum());
    }

    bool handleTypeClearTime(JMapInfoIter iter)
    {
        const char *pGalaxyName;
        iter.getValue<const char *>("Param01Str", &pGalaxyName);
        if (MR::isEqualString(pGalaxyName, "$CurrentGalaxy"))
            pGalaxyName = MR::getCurrentStageName();
        s32 scenarioNum = 0;
        iter.getValue<s32>("Param01Int", &scenarioNum);
        if (scenarioNum <= 0)
            scenarioNum = MR::getCurrentSelectedScenarioNo();
        return handleTypeGroupComparisonS32(iter, GameDataFunction::getStageSavedBestTime(pGalaxyName, scenarioNum));
    }

    bool handleTypeTicoCoinNum(JMapInfoIter iter)
    {
        int offset = 0;
        // count the current comet medal
        if (GameSequenceFunction::isCollectTicoCoinInStage() && !GameDataFunction::isOnGalaxyFlagTicoCoin(MR::getCurrentStageName()))
            offset = 1;
        return handleTypeGroupComparisonS32(iter, GameDataFunction::calcTicoCoinNum() + offset);
    }

    bool handleTypeLivesNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, GameDataFunction::getPlayerLeft());
    }

    bool handleTypeLifeNum(JMapInfoIter iter)
    {
        return handleTypeGroupComparisonS32(iter, getLife());
    }

    bool handleTypeRaceTime(JMapInfoIter iter)
    {
        s32 param01int = 0;
        iter.getValue<s32>("Param01Int", &param01int);
        return handleTypeGroupComparisonS32(iter, MR::getRaceBestTime(param01int));
    }

    bool handleTypeBronzeStarCollected(JMapInfoIter iter)
    {
        s32 scenarioNum = 0;
        iter.getValue<s32>("Param00Int", &scenarioNum);
        if (scenarioNum <= 0)
            scenarioNum = MR::getCurrentSelectedScenarioNo();
        return GameDataFunction::hasBronzeStar(getGalaxyName(iter), scenarioNum);
    }

    bool handleTypeStarCollected(JMapInfoIter iter)
    {
        GalaxyStatusAccessor pAccessor = getGalaxyStatusAccessor(iter);
        s32 scenarioNum = 0;
        iter.getValue<s32>("Param00Int", &scenarioNum);
        if (scenarioNum <= 0)
            scenarioNum = MR::getCurrentSelectedScenarioNo();
        return pAccessor.hasPowerStar(scenarioNum);
    }

    bool handleTypeTicoCoinCollected(JMapInfoIter iter)
    {
        const char *pGalaxyName = getGalaxyName(iter);
        if (MR::isEqualStageName(pGalaxyName))
            return GameSequenceFunction::isCollectTicoCoinInStage();
        return GameDataFunction::isOnGalaxyFlagTicoCoin(pGalaxyName);
    }

    bool handleTypeGameEventFlag(JMapInfoIter iter)
    {
        const char *pEventFlagName = getParam00Str(iter);
        if (GameEventFlagTable::isExist(pEventFlagName))
            return GameDataFunction::isOnGameEventFlag(pEventFlagName);
        else
        {
            OSReport("SuperFlag: Event Flag \"%s\" does not seem to exist\n", pEventFlagName);
            return false;
        }
    }

    bool handleTypeGalaxyCompletedNoGreen(JMapInfoIter iter)
    {
        const char *pGalaxyName = getGalaxyName(iter);
        return MR::isGalaxyCompletedNoGreen(pGalaxyName);
    }

    bool handleTypeGalaxyCompletedWithGreen(JMapInfoIter iter)
    {
        const char *pGalaxyName = getGalaxyName(iter);
        return MR::isGalaxyCompletedWithGreen(pGalaxyName);
    }

}