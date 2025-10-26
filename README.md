# SuperFlag
SuperFlag is an API for other modules to check conditionals (booleans).

Made by me and Evanbowl. Big thanks to Evan for the help!

This module requires [PTUtils](https://github.com/Lord-G-INC/Modular-PTD/tree/main/PTD/PTUtils) to function.

Basic programming knowledge will help in understanding this module, but it is not required.

# How to use - for modders
First, download [the template](disc/SystemData/SuperFlag.arc). Put it in your mod's `SystemData` folder.

Super Flags are essentially booleans and have two values: true or false.

## FlagName
Name this whatever you want*. If a module's BCSV file has a FlagName and indicates that it's for SuperFlag, put the name you have here in that field.

*The flag name "True" is reserved and always returns true.

## FlagType

What you choose to check will dictate what the rest of the fields are.

Speaking of, the last two fields in this bcsv are `Param01Str` and `Param01Int`.

Some flag types have multiple aliases, and are indicated with a /.

### Comparison Type

If a type is marked as this, `Param00Str` and `Param00Int` will ALWAYS be the same thing.

`Param00Str`: The sign to use. Supports >=, ==, >, <, <=, !=. If you don't know what these symbols mean, you can look [here](https://www.geeksforgeeks.org/comparison-operators-in-programming/).

`Param00Int`: What the value is compared to.

Example: 
![The 2nd row included in the SuperFlag.arc template](superflag-template.png)

It looks like an average programming comparison. If the CoinNum is greater than or equal to 100 when this flag is checked, it returns true. Otherwise, it returns false.

### FlagTypes List
#### And
Checks if all of the flags provided are true.

Param00Str is the first flag it will check. If true, it goes to Param01Str. If false, it returns false. If blank, it assumes the previous flag was the last one. 

This will go on up to Param99Str, but you will have to add those fields.

#### Or
Checks if any of the flags provided are active.

Param00Str is the first flag it will check. If true, it returns true. If false, it goes on to Param01Str. If it's blank, it assumes the previous flag was the last one and return false.

This goes on up to Param99Str, but likewise with And, you'll have to add those fields.

#### Not
Checks if the flag in Param00Str is true. If it is, it returns false. If it isn't, it returns true.

#### GalaxyName
Checks if any of the galaxy names provided are the current galaxy.

Checks Param00Str. If that's the current galaxy's name, returns true. If false, it goes on to Param01Str. If that's blank, it assumes the previous flag was the last one and returns false.

Like `Or`, goes on up to Param99Str (but you will have to add these fields).

#### ScenarioNo / ScenarioNum
[Comparison Type](#comparison-type). Compares the current scenario number.

#### SelectedScenarioNo / SelectedScenarioNum
[Comparison Type](#comparison-type). Compares the currently selected scenario number.
Green Star 1 in Flip Swap would have a SelectedScenarioNo of 3, and a ScenarioNo of 1 because it takes place in Scenario 1, but you *select* the 3rd star.

#### PowerStarNum
[Comparison Type](#comparison-type). Compares the current amount of collected stars in the current save file.

#### StockedCoinNum
[Comparison Type](#comparison-type). Compares the current amount of stocked coins in the current save file. Stocked coins are what you see as the coin counter in Starship Mario.

#### StockedStarPieceNum / StockedStarBitNum
[Comparison Type](#comparison-type). Compares the current amount of stocked starbits in the current save file. Stocked starbits are what you see as the starbit counter in Starship Mario.

#### StarPieceNum / StarBitNum
[Comparison Type](#comparison-type). Compares the current amount of collected Starbits in the current level.

#### CoinNum
[Comparison Type](#comparison-type). Compares the current amount of collected Coins in the current level.

#### PurpleCoinNum

[Comparison Type](#comparison-type). Compares the current amount of collected Purple Coins in the current level.

#### ClearTime

[Comparison Type](#comparison-type). Compares the clear time (in frames) of a certain galaxy in the current save file.

Param01Str: The name of the galaxy to check. Put `$CurrentGalaxy` to check the current galaxy.

Param01Int: The scenario number to check. 0 or less checks the current selected scenario.

#### TicoCoinNum / CometMedalNum
[Comparison Type](#comparison-type). Compares the current comet medal count. Includes the comet medal in the current galaxy. So, it's like the number in the pause menu.

#### MarioLeftNum / LivesNum
[Comparison Type](#comparison-type). Compares the current number of lives, also known as the number in the bottom left.

#### HealthLeftNum / LifeMeterNum
[Comparison Type](#comparison-type). Compares the current count in the life meter. In other words, this:

![Life Meter](life-meter.png)

#### RaceTime
[Comparison Type](#comparison-type). Compares the current race time based on a race id.

Param01Int: The Race ID to check. In the vanilla game, 0 is Wild Glide Galaxy and 1 is Fleet Glide Galaxy.

#### BronzeStarCollected
Checks if a bronze star in a galaxy has been collected in the current save file.

Param00Str: The name of the galaxy to check. Put `$CurrentGalaxy` to check the current galaxy.

Param00Int: The scenario number to check. 0 or less checks the current selected scenario.


#### StarCollected
Checks if a star in a galaxy has been collected in the current save file.

Param00Str: The name of the galaxy to check. Put `$CurrentGalaxy` to check the current galaxy.

Param00Int: The scenario number to check. 0 or less checks the current selected scenario.

#### TicoCoinCollected / CometMedalCollected
Checks if the comet medal in a galaxy has been collected in the current save file.

Param00Str: The name of the galaxy to check. Put `$CurrentGalaxy` to check the current galaxy. If you collect the comet medal, this will return true even if you haven't saved yet. In other words, it is collected when the icon on the UI shows it's collected.

#### GameEventFlag
Checks if the game event flag is active in the current save file. A list of those can be found [here](https://www.lumasworkshop.com/wiki/Game_Flags). 

Param00Str: Game Event Flag name

#### GalaxyCompletedSilver / GalaxyCompletedNoGreen
Checks if the galaxy has a silver crown in the current save file.

Param00Str: The name of the galaxy to check. Put `$CurrentGalaxy` to check the current galaxy.

#### GalaxyCompletedGold / GalaxyCompletedWithGreen
Checks if the galaxy has a gold crown in the current save file.

Param00Str: The name of the galaxy to check. Put `$CurrentGalaxy` to check the current galaxy.


### Future Support
1. Support for specifying which save file to check may be added in the future.

# How to use - for developers
Using SuperFlag in your module allows users to specify conditions for activating features in your module.

## Checking Flags

1. Put `#include "SuperFlag.h"` at the top of the .cpp file
2. Add SuperFlag as a Module Dependency to your `ModuleInfo.json`:
```json
"ModuleDependancies": [
    "SuperFlag"
]
```
(yes that is how dependencies is spelled, no it is not the right way)

3. Call it using:
```cpp
const char *name = "Your Flag Name Here";
bool result = sf::isFlagActive(name);
```
or
```cpp
int index = 0;
bool result = sf::isFlagActive(index);
```

## Adding Custom Flag Types

1. Add the following to ModuleInfo.json's ModuleData:
```json
  "ModuleData": [
    {
      "SuperFlagCustomFlags": [
        {
          "Name": "CoinFlip",
          "FunctionName": "handleTypeCoinFlip"
        }
      ]
    }
  ]
```
The Name is what users will put as the FlagType.

The function name is what SuperFlag will call. It must match the parameters of other types (`handleTypeCoinFlip(JMapInfoIter iter)`).

2. Define the function. By default SuperFlag will look for the type under the `sf` namespace.
```cpp
namespace sf {
    bool handleTypeCoinFlip(JMapInfoIter iter)
    {
        return MR::isHalfProbability();
    }
}
```
And you're done!
