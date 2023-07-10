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



void cr::vstab::VStabiliserParams::encode(uint8_t* data, int& size)
{
    // Encode version.
    int pos = 0;
    data[pos] = 0x02; pos += 1;
    data[pos] = VSTABILISER_MAJOR_VERSION; pos += 1;
    data[pos] = VSTABILISER_MINOR_VERSION; pos += 1;

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

    // Decode data.
    int pos = 3;
    memcpy(&scaleFactor, &data[pos], 4); pos += 4;
    memcpy(&xOffsetLimit, &data[pos], 4); pos += 4;
    memcpy(&yOffsetLimit, &data[pos], 4); pos += 4;
    memcpy(&aOffsetLimit, &data[pos], 4); pos += 4;
    memcpy(&xFilterCoeff, &data[pos], 4); pos += 4;
    memcpy(&yFilterCoeff, &data[pos], 4); pos += 4;
    memcpy(&aFilterCoeff, &data[pos], 4); pos += 4;
    enable = data[pos] == 0x00 ? false : true; pos += 1;
    trasparentBorder = data[pos] == 0x00 ? false : true; pos += 1;
    memcpy(&constXOffset, &data[pos], 4); pos += 4;
    memcpy(&constYOffset, &data[pos], 4); pos += 4;
    memcpy(&constAOffset, &data[pos], 4); pos += 4;
    memcpy(&instantXOffset, &data[pos], 4); pos += 4;
    memcpy(&instantYOffset, &data[pos], 4); pos += 4;
    memcpy(&instantAOffset, &data[pos], 4); pos += 4;
    memcpy(&type, &data[pos], 4); pos += 4;
    memcpy(&cutFrequencyHz, &data[pos], 4); pos += 4;
    memcpy(&fps, &data[pos], 4); pos += 4;
    memcpy(&processingTimeMks, &data[pos], 4);

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












