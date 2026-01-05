#pragma once
#include "Game/System/BinaryDataChunkHolder.h"
#include "Game/Util/BitArray.h"

class AchievementChunk : public BinaryDataChunkBase {
    public:
    AchievementChunk();

    // Returns a second identifier for this chunk.
    // Note that this value should be static, and should not be calculated based on data values.
    virtual u32 makeHeaderHashCode() const;

    // Returns the signature of this chunk.
    // Should be a four character code.
	virtual u32 getSignature() const;

    // Used for writing data to the save file.
    // The `pPosition` argument is a pointer to the
    // start of the data in the save file.
    // Unsure of what the 2nd argument is used for.
    // Return value is the size of the data written.
    // Note that you don't need to write the signature and hash yourself.
	virtual u32 serialize(u8 *pPosition, u32) const;

    // Used for reading data from the save file.
    // The `pPosition` argument is a pointer to the
    // start of the data in the save file.
    // The `size` argument is the size of this chunk.
    // Return value appears to be a return code. 0 = success.
	virtual u32 deserialize(const u8 *pPosition, u32 size);

    // Used for initializing this chunk's member variables.
	virtual void initializeData();

    MR::BitArray mArray;
    s32 mCount;
};