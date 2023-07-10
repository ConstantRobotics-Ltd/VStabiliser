#include "VStabiliser.h"
#include "VStabiliserVersion.h"



std::string cr::vstab::VStabiliser::getVersion()
{
    return VSTABILISER_VERSION;
}



cr::vstab::VStabiliserParams &cr::vstab::VStabiliserParams::operator= (const cr::vstab::VStabiliserParams &src)
{
    // Check yourself.
    if (this == &src)
        return *this;

    // Copy params.
    scaleFactor = src.scaleFactor;
    xOffsetLimit = src.xOffsetLimit;
    yOffsetLimit = src.yOffsetLimit;
    aOffsetLimit = src.aOffsetLimit;
    xFilterCoeff = src.xFilterCoeff;
    yFilterCoeff = src.yFilterCoeff;
    aFilterCoeff = src.aFilterCoeff;
    enable = src.enable;
    trasparentBorder = src.trasparentBorder;
    constXOffset = src.constXOffset;
    constYOffset = src.constYOffset;
    constAOffset = src.constAOffset;
    instantXOffset = src.instantXOffset;
    instantYOffset = src.instantYOffset;
    instantAOffset = src.instantAOffset;
    type = src.type;
    cutFrequencyHz = src.cutFrequencyHz;
    fps = src.fps;
    processingTimeMks = src.processingTimeMks;

    return *this;
}



void cr::vstab::VStabiliserParams::encode(
        uint8_t* data, int& size, cr::vstab::VStabiliserParamsMask* mask)
{
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
        memcpy(&data[pos], &scaleFactor, 4); pos += 4;
        memcpy(&data[pos], &xOffsetLimit, 4); pos += 4;
        memcpy(&data[pos], &yOffsetLimit, 4); pos += 4;
        memcpy(&data[pos], &aOffsetLimit, 4); pos += 4;
        memcpy(&data[pos], &xFilterCoeff, 4); pos += 4;
        memcpy(&data[pos], &yFilterCoeff, 4); pos += 4;
        memcpy(&data[pos], &aFilterCoeff, 4); pos += 4;
        data[pos] = enable == true ? 0x01 : 0x00; pos += 1;
        data[pos] = trasparentBorder == true ? 0x01 : 0x00; pos += 1;
        memcpy(&data[pos], &constXOffset, 4); pos += 4;
        memcpy(&data[pos], &constYOffset, 4); pos += 4;
        memcpy(&data[pos], &constAOffset, 4); pos += 4;
        memcpy(&data[pos], &instantXOffset, 4); pos += 4;
        memcpy(&data[pos], &instantYOffset, 4); pos += 4;
        memcpy(&data[pos], &instantAOffset, 4); pos += 4;
        memcpy(&data[pos], &type, 4); pos += 4;
        memcpy(&data[pos], &cutFrequencyHz, 4); pos += 4;
        memcpy(&data[pos], &fps, 4); pos += 4;
        memcpy(&data[pos], &processingTimeMks, 4); pos += 4;

        size = pos;

        return;
    }

    // Prepare mask.
    data[pos] = 0;
    data[pos] = data[pos] | (mask->scaleFactor ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->xOffsetLimit ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->yOffsetLimit ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->aOffsetLimit ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->xFilterCoeff ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->yFilterCoeff ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->aFilterCoeff ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->enable ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->trasparentBorder ? (uint8_t)128 :(uint8_t)0);
    data[pos] = data[pos] | (mask->constXOffset ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->constYOffset ? (uint8_t)32 : (uint8_t)0);
    data[pos] = data[pos] | (mask->constAOffset ? (uint8_t)16 : (uint8_t)0);
    data[pos] = data[pos] | (mask->instantXOffset ? (uint8_t)8 : (uint8_t)0);
    data[pos] = data[pos] | (mask->instantYOffset ? (uint8_t)4 : (uint8_t)0);
    data[pos] = data[pos] | (mask->instantAOffset ? (uint8_t)2 : (uint8_t)0);
    data[pos] = data[pos] | (mask->type ? (uint8_t)1 : (uint8_t)0);
    pos += 1;
    data[pos] = 0;
    data[pos] = data[pos] | (mask->cutFrequencyHz ? (uint8_t)128 : (uint8_t)0);
    data[pos] = data[pos] | (mask->fps ? (uint8_t)64 : (uint8_t)0);
    data[pos] = data[pos] | (mask->processingTimeMks ? (uint8_t)32 :(uint8_t)0);
    pos += 1;

    if (mask->scaleFactor)
    {
        memcpy(&data[pos], &scaleFactor, 4); pos += 4;
    }
    if (mask->xOffsetLimit)
    {
        memcpy(&data[pos], &xOffsetLimit, 4); pos += 4;
    }
    if (mask->yOffsetLimit)
    {
        memcpy(&data[pos], &yOffsetLimit, 4); pos += 4;
    }
    if (mask->aOffsetLimit)
    {
        memcpy(&data[pos], &aOffsetLimit, 4); pos += 4;
    }
    if (mask->xFilterCoeff)
    {
        memcpy(&data[pos], &xFilterCoeff, 4); pos += 4;
    }
    if (mask->yFilterCoeff)
    {
        memcpy(&data[pos], &yFilterCoeff, 4); pos += 4;
    }
    if (mask->aFilterCoeff)
    {
        memcpy(&data[pos], &aFilterCoeff, 4); pos += 4;
    }
    if (mask->enable)
    {
        data[pos] = enable == true ? 0x01 : 0x00; pos += 1;
    }
    if (mask->trasparentBorder)
    {
        data[pos] = trasparentBorder == true ? 0x01 : 0x00; pos += 1;
    }
    if (mask->constXOffset)
    {
        memcpy(&data[pos], &constXOffset, 4); pos += 4;
    }
    if (mask->constYOffset)
    {
        memcpy(&data[pos], &constYOffset, 4); pos += 4;
    }
    if (mask->constAOffset)
    {
        memcpy(&data[pos], &constAOffset, 4); pos += 4;
    }
    if (mask->instantXOffset)
    {
        memcpy(&data[pos], &instantXOffset, 4); pos += 4;
    }
    if (mask->instantYOffset)
    {
        memcpy(&data[pos], &instantYOffset, 4); pos += 4;
    }
    if (mask->instantAOffset)
    {
        memcpy(&data[pos], &instantAOffset, 4); pos += 4;
    }
    if (mask->type)
    {
        memcpy(&data[pos], &type, 4); pos += 4;
    }
    if (mask->cutFrequencyHz)
    {
        memcpy(&data[pos], &cutFrequencyHz, 4); pos += 4;
    }
    if (mask->fps)
    {
        memcpy(&data[pos], &fps, 4); pos += 4;
    }
    if (mask->processingTimeMks)
    {
        memcpy(&data[pos], &processingTimeMks, 4); pos += 4;
    }

    size = pos;
}



bool cr::vstab::VStabiliserParams::decode(uint8_t* data)
{
    // Check header.
    if (data[0] != 0x02)
        return false;

    // Check version.
    if (data[1] != VSTABILISER_MAJOR_VERSION ||
        data[2] != VSTABILISER_MINOR_VERSION)
        return false;

    int pos = 6;
    if ((data[3] & (uint8_t)128) == (uint8_t)128)
    {
        memcpy(&scaleFactor, &data[pos], 4); pos += 4;
    }
    else
    {
        scaleFactor = 0;
    }
    if ((data[3] & (uint8_t)64) == (uint8_t)64)
    {
        memcpy(&xOffsetLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        xOffsetLimit = 0;
    }
    if ((data[3] & (uint8_t)32) == (uint8_t)32)
    {
        memcpy(&yOffsetLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        yOffsetLimit = 0;
    }
    if ((data[3] & (uint8_t)16) == (uint8_t)16)
    {
        memcpy(&aOffsetLimit, &data[pos], 4); pos += 4;
    }
    else
    {
        aOffsetLimit = 0.0f;
    }
    if ((data[3] & (uint8_t)8) == (uint8_t)8)
    {
        memcpy(&xFilterCoeff, &data[pos], 4); pos += 4;
    }
    else
    {
        xFilterCoeff = 0.0f;
    }
    if ((data[3] & (uint8_t)4) == (uint8_t)4)
    {
        memcpy(&yFilterCoeff, &data[pos], 4); pos += 4;
    }
    else
    {
        yFilterCoeff = 0.0f;
    }
    if ((data[3] & (uint8_t)2) == (uint8_t)2)
    {
        memcpy(&aFilterCoeff, &data[pos], 4); pos += 4;
    }
    else
    {
        aFilterCoeff = 0.0f;
    }
    if ((data[3] & (uint8_t)1) == (uint8_t)1)
    {
        enable = data[pos] == 0x00 ? false : true; pos += 1;
    }
    else
    {
        enable = false;
    }


    if ((data[4] & (uint8_t)128) == (uint8_t)128)
    {
        trasparentBorder = data[pos] == 0x00 ? false : true; pos += 1;
    }
    else
    {
        trasparentBorder = false;
    }
    if ((data[4] & (uint8_t)64) == (uint8_t)64)
    {
        memcpy(&constXOffset, &data[pos], 4); pos += 4;
    }
    else
    {
        constXOffset = 0;
    }
    if ((data[4] & (uint8_t)32) == (uint8_t)32)
    {
        memcpy(&constYOffset, &data[pos], 4); pos += 4;
    }
    else
    {
        constYOffset = 0;
    }
    if ((data[4] & (uint8_t)16) == (uint8_t)16)
    {
        memcpy(&constAOffset, &data[pos], 4); pos += 4;
    }
    else
    {
        constAOffset = 0.0f;
    }
    if ((data[4] & (uint8_t)8) == (uint8_t)8)
    {
        memcpy(&instantXOffset, &data[pos], 4); pos += 4;
    }
    else
    {
        instantXOffset = 0;
    }
    if ((data[4] & (uint8_t)4) == (uint8_t)4)
    {
        memcpy(&instantYOffset, &data[pos], 4); pos += 4;
    }
    else
    {
        instantYOffset = 0;
    }
    if ((data[4] & (uint8_t)2) == (uint8_t)2)
    {
        memcpy(&instantAOffset, &data[pos], 4); pos += 4;
    }
    else
    {
        instantAOffset = 0.0f;
    }
    if ((data[4] & (uint8_t)1) == (uint8_t)1)
    {
        memcpy(&type, &data[pos], 4); pos += 4;
    }
    else
    {
        type = 0;
    }


    if ((data[4] & (uint8_t)128) == (uint8_t)128)
    {
        memcpy(&cutFrequencyHz, &data[pos], 4); pos += 4;
    }
    else
    {
        cutFrequencyHz = 0.0f;
    }
    if ((data[4] & (uint8_t)64) == (uint8_t)64)
    {
        memcpy(&fps, &data[pos], 4); pos += 4;
    }
    else
    {
        fps = 0.0f;
    }
    if ((data[4] & (uint8_t)32) == (uint8_t)32)
    {
        memcpy(&processingTimeMks, &data[pos], 4);
    }
    else
    {
        processingTimeMks = 0;
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
        uint8_t* data, int& size, cr::vstab::VStabiliserCommand id)
{
    // Fill header.
    data[0] = 0x00;
    data[1] = VSTABILISER_MAJOR_VERSION;
    data[2] = VSTABILISER_MINOR_VERSION;

    // Fill data.
    int commandId = (int)id;
    memcpy(&data[3], &commandId, 4);
    size = 7;
}



int cr::vstab::VStabiliser::decodeCommand(
        uint8_t* data, int size, cr::vstab::VStabiliserParam& paramId,
        cr::vstab::VStabiliserCommand& commandId, float& value)
{
    // Check size.
    if (size < 7)
        return -1;

    // Check version.
    if (data[1] != VSTABILISER_MAJOR_VERSION ||
        data[2] != VSTABILISER_MINOR_VERSION)
        return -1;

    // Extract data.
    int id = 0;
    memcpy(&id, &data[3], 4);
    value = 0.0f;

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
        memcpy(&value, &data[7], 4);
        return 1;
    }

    return -1;
}












