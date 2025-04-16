#include "VStabiliser.h"
#include "VStabiliserVersion.h"



/**
 * @brief Set or clear a bit in a byte.
 * @param byte Pointer to the byte.
 * @param cond Condition to set or clear the bit. True to set, false to clear.
 * @param bitPosition The bit position (0-7).
 */
static inline void setOrClearBit(uint8_t *byte, bool cond, uint8_t bitPosition) 
{
    if (cond)
        *byte |= (1 << bitPosition);     // Set the bit
    else
        *byte &= ~(1 << bitPosition);    // Clear the bit
}



/**
 * @brief Check if the bit is set in the byte.
 * @param byte The byte to check.
 * @param bitPosition The bit position to check (0-7).
 * @return TRUE if the bit is set, FALSE otherwise.
 */
static inline bool isBitSet(uint8_t byte, uint8_t bitPosition) 
{
    return (byte & (1 << bitPosition)) != 0;
}



cr::vstab::VStabiliser::~VStabiliser()
{

}



std::string cr::vstab::VStabiliser::getVersion()
{
    return VSTABILISER_VERSION;
}



bool cr::vstab::VStabiliserParams::encode(
        uint8_t* data, int bufferSize, int& size, cr::vstab::VStabiliserParamsMask* mask)
{
    // Check buffer size. Full parameters require 96 bytes.
    if (bufferSize < 96)
        return false;

    /// Format of the data:
    /// [0] - Header byte (0x02)
    /// [1] - major version of the library
    /// [2] - minor version of the library
    /// [3:5] - mask bytes that defines which params are included in the payload
    /// [6:end] - Payload data.

    // Encode version.
    int pos = 0;
    data[pos] = 0x02; pos += 1;
    data[pos] = VSTABILISER_MAJOR_VERSION; pos += 1;
    data[pos] = VSTABILISER_MINOR_VERSION; pos += 1;

    // Prepare mask.
    if (mask == nullptr)
    {
        // Prepare mask.
        data[pos] = 0xFF; pos += 1;
        data[pos] = 0xFF; pos += 1;
        data[pos] = 0xFF; pos += 1;

        // Encode data.
        memcpy(&data[pos], &scaleFactor, sizeof(scaleFactor)); pos += sizeof(scaleFactor);
        memcpy(&data[pos], &xOffsetLimit, sizeof(xOffsetLimit)); pos += sizeof(xOffsetLimit);
        memcpy(&data[pos], &yOffsetLimit, sizeof(yOffsetLimit)); pos += sizeof(yOffsetLimit);
        memcpy(&data[pos], &aOffsetLimit, sizeof(aOffsetLimit)); pos += sizeof(aOffsetLimit);
        memcpy(&data[pos], &xFilterCoeff, sizeof(xFilterCoeff)); pos += sizeof(xFilterCoeff);
        memcpy(&data[pos], &yFilterCoeff, sizeof(yFilterCoeff)); pos += sizeof(yFilterCoeff);
        memcpy(&data[pos], &aFilterCoeff, sizeof(aFilterCoeff)); pos += sizeof(aFilterCoeff);
        data[pos] = enable == true ? 0x01 : 0x00; pos += 1;
        data[pos] = transparentBorder == true ? 0x01 : 0x00; pos += 1;
        memcpy(&data[pos], &constXOffset, sizeof(constXOffset)); pos += sizeof(constXOffset);
        memcpy(&data[pos], &constYOffset, sizeof(constYOffset)); pos += sizeof(constYOffset);
        memcpy(&data[pos], &constAOffset, sizeof(constAOffset)); pos += sizeof(constAOffset);
        memcpy(&data[pos], &instantXOffset, sizeof(instantXOffset)); pos += sizeof(instantXOffset);
        memcpy(&data[pos], &instantYOffset, sizeof(instantYOffset)); pos += sizeof(instantYOffset);
        memcpy(&data[pos], &instantAOffset, sizeof(instantAOffset)); pos += sizeof(instantAOffset);
        memcpy(&data[pos], &type, sizeof(type)); pos += sizeof(type);
        memcpy(&data[pos], &cutFrequencyHz, sizeof(cutFrequencyHz)); pos += sizeof(cutFrequencyHz);
        memcpy(&data[pos], &fps, sizeof(fps)); pos += sizeof(fps);
        memcpy(&data[pos], &processingTimeMks, sizeof(processingTimeMks)); pos += sizeof(processingTimeMks);
        memcpy(&data[pos], &logMod, sizeof(logMod)); pos += sizeof(logMod);
        memcpy(&data[pos], &backend, sizeof(backend)); pos += sizeof(backend);
        memcpy(&data[pos], &custom1, sizeof(custom1)); pos += sizeof(custom1);
        memcpy(&data[pos], &custom2, sizeof(custom2)); pos += sizeof(custom2);
        memcpy(&data[pos], &custom3, sizeof(custom3)); pos += sizeof(custom3);

        size = pos;
        return true;
    }

    // First byte of mask.
    setOrClearBit(&data[pos], mask->scaleFactor, 7);
    setOrClearBit(&data[pos], mask->xOffsetLimit, 6);
    setOrClearBit(&data[pos], mask->yOffsetLimit, 5);
    setOrClearBit(&data[pos], mask->aOffsetLimit, 4);
    setOrClearBit(&data[pos], mask->xFilterCoeff, 3);
    setOrClearBit(&data[pos], mask->yFilterCoeff, 2);
    setOrClearBit(&data[pos], mask->aFilterCoeff, 1);
    setOrClearBit(&data[pos], mask->enable, 0);

    // Second byte of mask.
    pos += 1;
    setOrClearBit(&data[pos], mask->transparentBorder, 7);
    setOrClearBit(&data[pos], mask->constXOffset, 6);
    setOrClearBit(&data[pos], mask->constYOffset, 5);
    setOrClearBit(&data[pos], mask->constAOffset, 4);
    setOrClearBit(&data[pos], mask->instantXOffset, 3);
    setOrClearBit(&data[pos], mask->instantYOffset, 2);
    setOrClearBit(&data[pos], mask->instantAOffset, 1);
    setOrClearBit(&data[pos], mask->type, 0);

    // Third byte of mask.
    pos += 1;
    setOrClearBit(&data[pos], mask->cutFrequencyHz, 7);
    setOrClearBit(&data[pos], mask->fps, 6);
    setOrClearBit(&data[pos], mask->processingTimeMks, 5);
    setOrClearBit(&data[pos], mask->logMod, 4);
    setOrClearBit(&data[pos], mask->backend, 3);
    setOrClearBit(&data[pos], mask->custom1, 2);
    setOrClearBit(&data[pos], mask->custom2, 1);
    setOrClearBit(&data[pos], mask->custom3, 0);
    
    // Start encoding data.
    pos += 1;

    if (mask->scaleFactor)
    {
        memcpy(&data[pos], &scaleFactor, sizeof(scaleFactor)); 
        pos += sizeof(scaleFactor);
    }
    if (mask->xOffsetLimit)
    {
        memcpy(&data[pos], &xOffsetLimit, sizeof(xOffsetLimit)); 
        pos += sizeof(xOffsetLimit);
    }
    if (mask->yOffsetLimit)
    {
        memcpy(&data[pos], &yOffsetLimit, sizeof(yOffsetLimit)); 
        pos += sizeof(yOffsetLimit);
    }
    if (mask->aOffsetLimit)
    {
        memcpy(&data[pos], &aOffsetLimit, sizeof(aOffsetLimit)); 
        pos += sizeof(aOffsetLimit);
    }
    if (mask->xFilterCoeff)
    {
        memcpy(&data[pos], &xFilterCoeff, sizeof(xFilterCoeff)); 
        pos += sizeof(xFilterCoeff);
    }
    if (mask->yFilterCoeff)
    {
        memcpy(&data[pos], &yFilterCoeff, sizeof(yFilterCoeff)); 
        pos += sizeof(yFilterCoeff);
    }
    if (mask->aFilterCoeff)
    {
        memcpy(&data[pos], &aFilterCoeff, sizeof(aFilterCoeff)); 
        pos += sizeof(aFilterCoeff);
    }
    if (mask->enable)
    {
        data[pos] = enable == true ? 0x01 : 0x00; 
        pos += 1;
    }
    if (mask->transparentBorder)
    {
        data[pos] = transparentBorder == true ? 0x01 : 0x00; 
        pos += 1;
    }
    if (mask->constXOffset)
    {
        memcpy(&data[pos], &constXOffset, sizeof(constXOffset)); 
        pos += sizeof(constXOffset);
    }
    if (mask->constYOffset)
    {
        memcpy(&data[pos], &constYOffset, sizeof(constYOffset)); 
        pos += sizeof(constYOffset);
    }
    if (mask->constAOffset)
    {
        memcpy(&data[pos], &constAOffset, sizeof(constAOffset)); 
        pos += sizeof(constAOffset);
    }
    if (mask->instantXOffset)
    {
        memcpy(&data[pos], &instantXOffset, sizeof(instantXOffset)); 
        pos += sizeof(instantXOffset);
    }
    if (mask->instantYOffset)
    {
        memcpy(&data[pos], &instantYOffset, sizeof(instantYOffset)); 
        pos += sizeof(instantYOffset);
    }
    if (mask->instantAOffset)
    {
        memcpy(&data[pos], &instantAOffset, sizeof(instantAOffset)); 
        pos += sizeof(instantAOffset);
    }
    if (mask->type)
    {
        memcpy(&data[pos], &type, sizeof(type)); pos += sizeof(type);
    }
    if (mask->cutFrequencyHz)
    {
        memcpy(&data[pos], &cutFrequencyHz, sizeof(cutFrequencyHz)); 
        pos += sizeof(cutFrequencyHz);
    }
    if (mask->fps)
    {
        memcpy(&data[pos], &fps, sizeof(fps)); pos += sizeof(fps);
    }
    if (mask->processingTimeMks)
    {
        memcpy(&data[pos], &processingTimeMks, sizeof(processingTimeMks)); 
        pos += sizeof(processingTimeMks);
    }
    if (mask->logMod)
    {
        memcpy(&data[pos], &logMod, sizeof(logMod)); 
        pos += sizeof(logMod);
    }
    if (mask->backend)
    {
        memcpy(&data[pos], &backend, sizeof(backend)); 
        pos += sizeof(backend);
    }
    if (mask->custom1)
    {
        memcpy(&data[pos], &custom1, sizeof(custom1)); 
        pos += sizeof(custom1);
    }
    if (mask->custom2)
    {
        memcpy(&data[pos], &custom2, sizeof(custom2)); 
        pos += sizeof(custom2);
    }
    if (mask->custom3)
    {
        memcpy(&data[pos], &custom3, sizeof(custom3)); 
        pos += sizeof(custom3);
    }

    size = pos;

    return true;
}



bool cr::vstab::VStabiliserParams::decode(uint8_t* data, int dataSize)
{
    // Check data size.
    if (dataSize < 6)
        return false;

    /// Format of the data:
    /// [0] - Header byte (0x02)
    /// [1] - major version of the library
    /// [2] - minor version of the library
    /// [3:5] - mask bytes that defines which params are included in the payload
    /// [6:end] - Payload data.

    // Check header.
    if (data[0] != 0x02)
        return false;

    // Check version.
    if (data[1] != VSTABILISER_MAJOR_VERSION ||
        data[2] != VSTABILISER_MINOR_VERSION)
        return false;

    // Reset the params.
    scaleFactor = 0;
    xOffsetLimit = 0;
    yOffsetLimit = 0;
    aOffsetLimit = 0.0f;
    xFilterCoeff = 0.0f;
    yFilterCoeff = 0.0f;
    aFilterCoeff = 0.0f;
    enable = false;
    transparentBorder = false;
    constXOffset = 0;
    constYOffset = 0;
    constAOffset = 0.0f;
    instantXOffset = 0;
    instantYOffset = 0;
    instantAOffset = 0.0f;
    type = 0;
    cutFrequencyHz = 0.0f;
    fps = 0.0f;
    processingTimeMks = 0;
    logMod = 0;
    backend = 0;
    custom1 = 0.0f;
    custom2 = 0.0f;
    custom3 = 0.0f;

    // Start of payload.
    int pos = 6;

    if (isBitSet(data[3], 7))
    {
        if (dataSize < pos + sizeof(scaleFactor))
            return false;
        memcpy(&scaleFactor, &data[pos], sizeof(scaleFactor)); 
        pos += sizeof(scaleFactor);
    }

    if (isBitSet(data[3], 6))
    {
        if (dataSize < pos + sizeof(xOffsetLimit))
            return false;
        memcpy(&xOffsetLimit, &data[pos], sizeof(xOffsetLimit)); 
        pos += sizeof(xOffsetLimit);
    }

    if (isBitSet(data[3], 5))
    {
        if (dataSize < pos + sizeof(yOffsetLimit))
            return false;
        memcpy(&yOffsetLimit, &data[pos], sizeof(yOffsetLimit)); 
        pos += sizeof(yOffsetLimit);
    }

    if (isBitSet(data[3], 4))
    {
        if (dataSize < pos + sizeof(aOffsetLimit))
            return false;
        memcpy(&aOffsetLimit, &data[pos], sizeof(aOffsetLimit)); 
        pos += sizeof(aOffsetLimit);
    }

    if (isBitSet(data[3], 3))
    {
        if (dataSize < pos + sizeof(xFilterCoeff))
            return false;
        memcpy(&xFilterCoeff, &data[pos], sizeof(xFilterCoeff)); 
        pos += sizeof(xFilterCoeff);
    }

    if (isBitSet(data[3], 2))
    {
        if (dataSize < pos + sizeof(yFilterCoeff))
            return false;
        memcpy(&yFilterCoeff, &data[pos], sizeof(yFilterCoeff)); 
        pos += sizeof(yFilterCoeff);
    }

    if (isBitSet(data[3], 1))
    {
        if (dataSize < pos + sizeof(aFilterCoeff))
            return false;
        memcpy(&aFilterCoeff, &data[pos], sizeof(aFilterCoeff)); 
        pos += sizeof(aFilterCoeff);
    }

    if (isBitSet(data[3], 0))
    {
        if (dataSize < pos + 1)
            return false;
        enable = data[pos] == 0x00 ? false : true; pos += 1;
    }

    if (isBitSet(data[4], 7))
    {
        if (dataSize < pos + 1)
            return false;
        transparentBorder = data[pos] == 0x00 ? false : true; pos += 1;
    }

    if (isBitSet(data[4], 6))
    {
        if (dataSize < pos + sizeof(constXOffset))
            return false;
        memcpy(&constXOffset, &data[pos], sizeof(constXOffset)); 
        pos += sizeof(constXOffset);
    }

    if (isBitSet(data[4], 5))
    {
        if (dataSize < pos + sizeof(constYOffset))
            return false;
        memcpy(&constYOffset, &data[pos], sizeof(constYOffset)); 
        pos += sizeof(constYOffset);
    }

    if (isBitSet(data[4], 4))
    {
        if (dataSize < pos + sizeof(constAOffset))
            return false;
        memcpy(&constAOffset, &data[pos], sizeof(constAOffset)); 
        pos += sizeof(constAOffset);
    }

    if (isBitSet(data[4], 3))
    {
        if (dataSize < pos + sizeof(instantXOffset))
            return false;
        memcpy(&instantXOffset, &data[pos], sizeof(instantXOffset)); 
        pos += sizeof(instantXOffset);
    }

    if (isBitSet(data[4], 2))
    {
        if (dataSize < pos + sizeof(instantYOffset))
            return false;
        memcpy(&instantYOffset, &data[pos], sizeof(instantYOffset)); 
        pos += sizeof(instantYOffset);
    }

    if (isBitSet(data[4], 1))
    {
        if (dataSize < pos + sizeof(instantAOffset))
            return false;
        memcpy(&instantAOffset, &data[pos], sizeof(instantAOffset)); 
        pos += sizeof(instantAOffset);
    }

    if (isBitSet(data[4], 0))
    {
        if (dataSize < pos + sizeof(type))
            return false;
        memcpy(&type, &data[pos], sizeof(type)); 
        pos += sizeof(type);
    }

    if (isBitSet(data[5], 7))
    {
        if (dataSize < pos + sizeof(cutFrequencyHz))
            return false;
        memcpy(&cutFrequencyHz, &data[pos], sizeof(cutFrequencyHz)); 
        pos += sizeof(cutFrequencyHz);
    }

    if (isBitSet(data[5], 6))
    {
        if (dataSize < pos + sizeof(fps))
            return false;
        memcpy(&fps, &data[pos], sizeof(fps)); 
        pos += sizeof(fps);
    }

    if (isBitSet(data[5], 5))
    {
        if (dataSize < pos + sizeof(processingTimeMks))
            return false;
        memcpy(&processingTimeMks, &data[pos], sizeof(processingTimeMks)); 
        pos += sizeof(processingTimeMks);
    }

    if (isBitSet(data[5], 4))
    {
        if (dataSize < pos + sizeof(logMod))
            return false;
        memcpy(&logMod, &data[pos], sizeof(logMod));
        pos += sizeof(logMod);
    }

    if (isBitSet(data[5], 3))
    {
        if (dataSize < pos + sizeof(backend))
            return false;
        memcpy(&backend, &data[pos], sizeof(backend)); 
        pos += sizeof(backend);
    }

    if (isBitSet(data[5], 2))
    {
        if (dataSize < pos + sizeof(custom1))
            return false;
        memcpy(&custom1, &data[pos], sizeof(custom1)); 
        pos += sizeof(custom1);
    }

    if (isBitSet(data[5], 1))
    {
        if (dataSize < pos + sizeof(custom2))
            return false;
        memcpy(&custom2, &data[pos], sizeof(custom2)); 
        pos += sizeof(custom2);
    }

    if (isBitSet(data[5], 0))
    {
        if (dataSize < pos + sizeof(custom3))
            return false;
        memcpy(&custom3, &data[pos], sizeof(custom3)); 
        pos += sizeof(custom3);
    }

    return true;
}



void cr::vstab::VStabiliser::encodeSetParamCommand(
        uint8_t* data, int& size, cr::vstab::VStabiliserParam id, float value)
{
    // Fill header.
    data[0] = 0x01;
    data[1] = VSTABILISER_MAJOR_VERSION;
    data[2] = VSTABILISER_MINOR_VERSION;

    // Fill data.
    int paramId = (int)id;
    memcpy(&data[3], &paramId, 4);
    memcpy(&data[7], &value, 4);
    size = 11;
}



void cr::vstab::VStabiliser::encodeCommand(
        uint8_t* data, int& size, cr::vstab::VStabiliserCommand id, float value)
{
    // Fill header.
    data[0] = 0x00;
    data[1] = VSTABILISER_MAJOR_VERSION;
    data[2] = VSTABILISER_MINOR_VERSION;

    // Fill data.
    int commandId = (int)id;
    memcpy(&data[3], &commandId, 4);
    memcpy(&data[7], &value, 4);
    size = 11;
}



int cr::vstab::VStabiliser::decodeCommand(
        uint8_t* data, int size, cr::vstab::VStabiliserParam& paramId,
        cr::vstab::VStabiliserCommand& commandId, float& value)
{
    // Check size.
    if (size < 11)
        return -1;

    // Check version.
    if (data[1] != VSTABILISER_MAJOR_VERSION ||
        data[2] != VSTABILISER_MINOR_VERSION)
        return -1;

    // Extract data.
    int id = 0;
    memcpy(&id, &data[3], 4);
    memcpy(&value, &data[7], 4);

    // Check command type.
    if (data[0] == 0x00)
    {
        commandId = (VStabiliserCommand)id;
        return 0;
    }
    else if (data[0] == 0x01)
    {
        // Check size.
        if (size != 11)
            return false;

        paramId = (VStabiliserParam)id;
        return 1;
    }

    return -1;
}