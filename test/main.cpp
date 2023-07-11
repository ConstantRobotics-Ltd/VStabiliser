#include <iostream>
#include "VStabiliser.h"



/// Link namesapces.
using namespace cr::vstab;
using namespace std;



/// Copy test.
bool copyTest();

/// Encode/decode test.
bool encodeDecodeTest();

/// Encode/decode test with params mask.
bool encodeDecodeWithMaskTest();

/// Encode/decode commands test.
bool encodeDecodeCommandsTest();

/// JSON read/write test.
bool jsonReadWriteTest();



// Entry point.
int main(void)
{
    cout << "#####################################" << endl;
    cout << "#                                   #" << endl;
    cout << "# VStabiliserParams test            #" << endl;
    cout << "#                                   #" << endl;
    cout << "#####################################" << endl;
    cout << endl;

    cout << "Copy test:" << endl;
    if (copyTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode test:" << endl;
    if (encodeDecodeTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode test with params mask:" << endl;
    if (encodeDecodeWithMaskTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "Encode/Decode commands test:" << endl;
    if (encodeDecodeCommandsTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    cout << "JSON read/write test:" << endl;
    if (jsonReadWriteTest())
        cout << "OK" << endl;
    else
        cout << "ERROR" << endl;
    cout << endl;

    return 1;
}



/// Copy test.
bool copyTest()
{
    // Prepare random params.
    VStabiliserParams in;
    in.scaleFactor = rand() % 255;
    in.xOffsetLimit = rand() % 255;
    in.yOffsetLimit = rand() % 255;
    in.aOffsetLimit = rand() % 255;
    in.xFilterCoeff = rand() % 255;
    in.yFilterCoeff = rand() % 255;
    in.aFilterCoeff = rand() % 255;
    in.enable = false;
    in.trasparentBorder = true;
    in.constXOffset = rand() % 255;
    in.constYOffset = rand() % 255;
    in.constAOffset = rand() % 255;
    in.instantXOffset = rand() % 255;
    in.instantYOffset = rand() % 255;
    in.instantAOffset = rand() % 255;
    in.type = rand() % 255;
    in.cutFrequencyHz = rand() % 255;
    in.fps = rand() % 255;
    in.processingTimeMks = rand() % 255;
    in.logMod = rand() % 255;

    // Copy params.
    VStabiliserParams out = in;

    // Compare params.
    if (in.scaleFactor != out.scaleFactor)
    {
        cout << "in.scaleFactor" << endl;
        return false;
    }
    if (in.xOffsetLimit != out.xOffsetLimit)
    {
        cout << "in.xOffsetLimit" << endl;
        return false;
    }
    if (in.yOffsetLimit != out.yOffsetLimit)
    {
        cout << "in.yOffsetLimit" << endl;
        return false;
    }
    if (in.aOffsetLimit != out.aOffsetLimit)
    {
        cout << "in.aOffsetLimit" << endl;
        return false;
    }
    if (in.xFilterCoeff != out.xFilterCoeff)
    {
        cout << "in.xFilterCoeff" << endl;
        return false;
    }
    if (in.yFilterCoeff != out.yFilterCoeff)
    {
        cout << "in.yFilterCoeff" << endl;
        return false;
    }
    if (in.aFilterCoeff != out.aFilterCoeff)
    {
        cout << "in.aFilterCoeff" << endl;
        return false;
    }
    if (in.enable != out.enable)
    {
        cout << "in.enable" << endl;
        return false;
    }
    if (in.trasparentBorder != out.trasparentBorder)
    {
        cout << "in.trasparentBorder" << endl;
        return false;
    }
    if (in.constXOffset != out.constXOffset)
    {
        cout << "in.constXOffset" << endl;
        return false;
    }
    if (in.constYOffset != out.constYOffset)
    {
        cout << "in.constYOffset" << endl;
        return false;
    }
    if (in.constAOffset != out.constAOffset)
    {
        cout << "in.constAOffset" << endl;
        return false;
    }
    if (in.instantXOffset != out.instantXOffset)
    {
        cout << "in.instantXOffset" << endl;
        return false;
    }
    if (in.instantYOffset != out.instantYOffset)
    {
        cout << "in.instantYOffset" << endl;
        return false;
    }
    if (in.instantAOffset != out.instantAOffset)
    {
        cout << "in.instantAOffset" << endl;
        return false;
    }
    if (in.type != out.type)
    {
        cout << "in.type" << endl;
        return false;
    }
    if (in.cutFrequencyHz != out.cutFrequencyHz)
    {
        cout << "in.cutFrequencyHz" << endl;
        return false;
    }
    if (in.fps != out.fps)
    {
        cout << "in.fps" << endl;
        return false;
    }
    if (in.processingTimeMks != out.processingTimeMks)
    {
        cout << "in.processingTimeMks" << endl;
        return false;
    }
    if (in.logMod != out.logMod)
    {
        cout << "in.logMod" << endl;
        return false;
    }

    return true;
}



/// Encode/decode test.
bool encodeDecodeTest()
{
    // Prepare random params.
    VStabiliserParams in;
    in.scaleFactor = rand() % 255;
    in.xOffsetLimit = rand() % 255;
    in.yOffsetLimit = rand() % 255;
    in.aOffsetLimit = rand() % 255;
    in.xFilterCoeff = rand() % 255;
    in.yFilterCoeff = rand() % 255;
    in.aFilterCoeff = rand() % 255;
    in.enable = false;
    in.trasparentBorder = true;
    in.constXOffset = rand() % 255;
    in.constYOffset = rand() % 255;
    in.constAOffset = rand() % 255;
    in.instantXOffset = rand() % 255;
    in.instantYOffset = rand() % 255;
    in.instantAOffset = rand() % 255;
    in.type = rand() % 255;
    in.cutFrequencyHz = rand() % 255;
    in.fps = rand() % 255;
    in.processingTimeMks = rand() % 255;
    in.logMod = rand() % 255;

    // Encode data.
    uint8_t data[1024];
    int size = 0;
    in.encode(data, size);

    cout << "Encoded data size: " << size << " bytes" << endl;

    // Decode data.
    VStabiliserParams out;
    if (!out.decode(data))
    {
        cout << "Can't decode data" << endl;
        return false;
    }

    // Compare params.
    if (in.scaleFactor != out.scaleFactor)
    {
        cout << "in.scaleFactor" << endl;
        return false;
    }
    if (in.xOffsetLimit != out.xOffsetLimit)
    {
        cout << "in.xOffsetLimit" << endl;
        return false;
    }
    if (in.yOffsetLimit != out.yOffsetLimit)
    {
        cout << "in.yOffsetLimit" << endl;
        return false;
    }
    if (in.aOffsetLimit != out.aOffsetLimit)
    {
        cout << "in.aOffsetLimit" << endl;
        return false;
    }
    if (in.xFilterCoeff != out.xFilterCoeff)
    {
        cout << "in.xFilterCoeff" << endl;
        return false;
    }
    if (in.yFilterCoeff != out.yFilterCoeff)
    {
        cout << "in.yFilterCoeff" << endl;
        return false;
    }
    if (in.aFilterCoeff != out.aFilterCoeff)
    {
        cout << "in.aFilterCoeff" << endl;
        return false;
    }
    if (in.enable != out.enable)
    {
        cout << "in.enable" << endl;
        return false;
    }
    if (in.trasparentBorder != out.trasparentBorder)
    {
        cout << "in.trasparentBorder" << endl;
        return false;
    }
    if (in.constXOffset != out.constXOffset)
    {
        cout << "in.constXOffset" << endl;
        return false;
    }
    if (in.constYOffset != out.constYOffset)
    {
        cout << "in.constYOffset" << endl;
        return false;
    }
    if (in.constAOffset != out.constAOffset)
    {
        cout << "in.constAOffset" << endl;
        return false;
    }
    if (in.instantXOffset != out.instantXOffset)
    {
        cout << "in.instantXOffset" << endl;
        return false;
    }
    if (in.instantYOffset != out.instantYOffset)
    {
        cout << "in.instantYOffset" << endl;
        return false;
    }
    if (in.instantAOffset != out.instantAOffset)
    {
        cout << "in.instantAOffset" << endl;
        return false;
    }
    if (in.type != out.type)
    {
        cout << "in.type" << endl;
        return false;
    }
    if (in.cutFrequencyHz != out.cutFrequencyHz)
    {
        cout << "in.cutFrequencyHz" << endl;
        return false;
    }
    if (in.fps != out.fps)
    {
        cout << "in.fps" << endl;
        return false;
    }
    if (in.processingTimeMks != out.processingTimeMks)
    {
        cout << "in.processingTimeMks" << endl;
        return false;
    }
    if (in.logMod != out.logMod)
    {
        cout << "in.logMod" << endl;
        return false;
    }

    return true;
}



/// Encode/decode test with params mask.
bool encodeDecodeWithMaskTest()
{
    // Prepare random params.
    VStabiliserParams in;
    in.scaleFactor = rand() % 255;
    in.xOffsetLimit = rand() % 255;
    in.yOffsetLimit = rand() % 255;
    in.aOffsetLimit = rand() % 255;
    in.xFilterCoeff = rand() % 255;
    in.yFilterCoeff = rand() % 255;
    in.aFilterCoeff = rand() % 255;
    in.enable = false;
    in.trasparentBorder = true;
    in.constXOffset = rand() % 255;
    in.constYOffset = rand() % 255;
    in.constAOffset = rand() % 255;
    in.instantXOffset = rand() % 255;
    in.instantYOffset = rand() % 255;
    in.instantAOffset = rand() % 255;
    in.type = rand() % 255;
    in.cutFrequencyHz = rand() % 255;
    in.fps = rand() % 255;
    in.processingTimeMks = rand() % 255;
    in.logMod = rand() % 255;

    // Prepare mask.
    VStabiliserParamsMask mask;
    mask.scaleFactor = true;
    mask.xOffsetLimit = false;
    mask.yOffsetLimit = true;
    mask.aOffsetLimit = false;
    mask.xFilterCoeff = true;
    mask.yFilterCoeff = false;
    mask.aFilterCoeff = true;
    mask.enable = false;
    mask.trasparentBorder = true;
    mask.constXOffset = false;
    mask.constYOffset = true;
    mask.constAOffset = false;
    mask.instantXOffset = true;
    mask.instantYOffset = false;
    mask.instantAOffset = true;
    mask.type = false;
    mask.cutFrequencyHz = true;
    mask.fps = false;
    mask.processingTimeMks = true;
    mask.logMod = false;

    // Encode data.
    uint8_t data[1024];
    int size = 0;
    in.encode(data, size, &mask);

    cout << "Encoded data size: " << size << " bytes" << endl;

    // Decode data.
    VStabiliserParams out;
    if (!out.decode(data))
    {
        cout << "Can't decode data" << endl;
        return false;
    }

    // Compare params.
    if (in.scaleFactor != out.scaleFactor)
    {
        cout << "in.scaleFactor" << endl;
        return false;
    }
    if (out.xOffsetLimit != 0)
    {
        cout << "in.xOffsetLimit" << endl;
        return false;
    }
    if (in.yOffsetLimit != out.yOffsetLimit)
    {
        cout << "in.yOffsetLimit" << endl;
        return false;
    }
    if (out.aOffsetLimit != 0)
    {
        cout << "in.aOffsetLimit" << endl;
        return false;
    }
    if (in.xFilterCoeff != out.xFilterCoeff)
    {
        cout << "in.xFilterCoeff" << endl;
        return false;
    }
    if (out.yFilterCoeff != 0)
    {
        cout << "in.yFilterCoeff" << endl;
        return false;
    }
    if (in.aFilterCoeff != out.aFilterCoeff)
    {
        cout << "in.aFilterCoeff" << endl;
        return false;
    }
    if (out.enable != false)
    {
        cout << "in.enable" << endl;
        return false;
    }
    if (in.trasparentBorder != out.trasparentBorder)
    {
        cout << "in.trasparentBorder" << endl;
        return false;
    }
    if (out.constXOffset != 0)
    {
        cout << "in.constXOffset" << endl;
        return false;
    }
    if (in.constYOffset != out.constYOffset)
    {
        cout << "in.constYOffset" << endl;
        return false;
    }
    if (out.constAOffset != 0)
    {
        cout << "in.constAOffset" << endl;
        return false;
    }
    if (in.instantXOffset != out.instantXOffset)
    {
        cout << "in.instantXOffset" << endl;
        return false;
    }
    if (out.instantYOffset != 0)
    {
        cout << "in.instantYOffset" << endl;
        return false;
    }
    if (in.instantAOffset != out.instantAOffset)
    {
        cout << "in.instantAOffset" << endl;
        return false;
    }
    if (out.type != 0)
    {
        cout << "in.type" << endl;
        return false;
    }
    if (in.cutFrequencyHz != out.cutFrequencyHz)
    {
        cout << "in.cutFrequencyHz" << endl;
        return false;
    }
    if (out.fps != 0)
    {
        cout << "in.fps" << endl;
        return false;
    }
    if (in.processingTimeMks != out.processingTimeMks)
    {
        cout << "in.processingTimeMks" << endl;
        return false;
    }
    if (out.logMod != 0)
    {
        cout << "in.logMod" << endl;
        return false;
    }

    return true;
}



/// Encode/decode commands test.
bool encodeDecodeCommandsTest()
{
    // Encode command.
    uint8_t data[1024];
    int size = 0;
    float outValue = (float)(rand() % 20);
    VStabiliser::encodeCommand(data, size, VStabiliserCommand::RESET);

    // Decode command.
    VStabiliserCommand commandId;
    VStabiliserParam paramId;
    float value = 0.0f;
    if (VStabiliser::decodeCommand(data, size, paramId, commandId, value) != 0)
    {
        cout << "Command not decoded" << endl;
        return false;
    }

    // Checkk ID and value.
    if (commandId != VStabiliserCommand::RESET)
    {
        cout << "not a VStabiliserCommand::RESER" << endl;
        return false;
    }

    // Encode param.
    outValue = (float)(rand() % 20);
    VStabiliser::encodeSetParamCommand(
                data, size, VStabiliserParam::X_OFFSET_LIMIT, outValue);

    // Decode command.
    value = 0.0f;
    if (VStabiliser::decodeCommand(data, size, paramId, commandId, value) != 1)
    {
        cout << "Set param command not decoded" << endl;
        return false;
    }

    // Checkk ID and value.
    if (paramId != VStabiliserParam::X_OFFSET_LIMIT)
    {
        cout << "not a VStabiliserParam::X_OFFSET_LIMIT" << endl;
        return false;
    }
    if (value != outValue)
    {
        cout << "not equal value" << endl;
        return false;
    }

    return true;
}



/// JSON read/write test.
bool jsonReadWriteTest()
{
    // Prepare random params.
    VStabiliserParams in;
    in.scaleFactor = rand() % 255;
    in.xOffsetLimit = rand() % 255;
    in.yOffsetLimit = rand() % 255;
    in.aOffsetLimit = rand() % 255;
    in.xFilterCoeff = rand() % 255;
    in.yFilterCoeff = rand() % 255;
    in.aFilterCoeff = rand() % 255;
    in.enable = false;
    in.trasparentBorder = true;
    in.constXOffset = rand() % 255;
    in.constYOffset = rand() % 255;
    in.constAOffset = rand() % 255;
    in.instantXOffset = rand() % 255;
    in.instantYOffset = rand() % 255;
    in.instantAOffset = rand() % 255;
    in.type = rand() % 255;
    in.cutFrequencyHz = rand() % 255;
    in.fps = rand() % 255;
    in.processingTimeMks = rand() % 255;
    in.logMod = rand() % 255;

    // Write params to file.
    cr::utils::ConfigReader inConfig;
    inConfig.set(in, "VStabiliserParams");
    inConfig.writeToFile("TestVStabiliserParams.json");

    // Read params from file.
    cr::utils::ConfigReader outConfig;
    if(!outConfig.readFromFile("TestVStabiliserParams.json"))
    {
        cout << "Can't open config file" << endl;
        return false;
    }

    VStabiliserParams out;
    if(!outConfig.get(out, "VStabiliserParams"))
    {
        cout << "Can't read params from file" << endl;
        return false;
    }

    // Compare params.
    if (in.scaleFactor != out.scaleFactor)
    {
        cout << "in.scaleFactor" << endl;
        return false;
    }
    if (in.xOffsetLimit != out.xOffsetLimit)
    {
        cout << "in.xOffsetLimit" << endl;
        return false;
    }
    if (in.yOffsetLimit != out.yOffsetLimit)
    {
        cout << "in.yOffsetLimit" << endl;
        return false;
    }
    if (in.aOffsetLimit != out.aOffsetLimit)
    {
        cout << "in.aOffsetLimit" << endl;
        return false;
    }
    if (in.xFilterCoeff != out.xFilterCoeff)
    {
        cout << "in.xFilterCoeff" << endl;
        return false;
    }
    if (in.yFilterCoeff != out.yFilterCoeff)
    {
        cout << "in.yFilterCoeff" << endl;
        return false;
    }
    if (in.aFilterCoeff != out.aFilterCoeff)
    {
        cout << "in.aFilterCoeff" << endl;
        return false;
    }
    if (in.enable != out.enable)
    {
        cout << "in.enable" << endl;
        return false;
    }
    if (in.trasparentBorder != out.trasparentBorder)
    {
        cout << "in.trasparentBorder" << endl;
        return false;
    }
    if (in.constXOffset != out.constXOffset)
    {
        cout << "in.constXOffset" << endl;
        return false;
    }
    if (in.constYOffset != out.constYOffset)
    {
        cout << "in.constYOffset" << endl;
        return false;
    }
    if (in.constAOffset != out.constAOffset)
    {
        cout << "in.constAOffset" << endl;
        return false;
    }
    if (in.type != out.type)
    {
        cout << "in.type" << endl;
        return false;
    }
    if (in.cutFrequencyHz != out.cutFrequencyHz)
    {
        cout << "in.cutFrequencyHz" << endl;
        return false;
    }
    if (in.fps != out.fps)
    {
        cout << "in.fps" << endl;
        return false;
    }

    return true;
}














