#include "VStabiliser.h"
#include "VStabiliserVersion.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>



namespace
{
// Encoded params and commands store int and float fields as raw 4 bytes
// values (the byte order of the host).
static_assert(sizeof(int) == 4, "int must be 4 bytes");
static_assert(sizeof(float) == 4, "float must be 4 bytes");
static_assert(std::numeric_limits<float>::is_iec559,
              "float must be IEEE 754 binary32");

/// Size of an int or float value in the encoded data, bytes.
constexpr int VALUE_SIZE{4};
/// Size of a bool value in the encoded data, bytes.
constexpr int FLAG_SIZE{1};

// Header of the encoded data (params and commands):
// [0] - data type, [1] - major version of the library, [2] - minor version of
// the library.

/// Position of the data type in the encoded data.
constexpr int DATA_TYPE_POS{0};
/// Position of the major version of the library in the encoded data.
constexpr int MAJOR_VERSION_POS{1};
/// Position of the minor version of the library in the encoded data.
constexpr int MINOR_VERSION_POS{2};
/// Data type of the encoded action command (COMMAND).
constexpr uint8_t COMMAND_DATA_TYPE{0x00};
/// Data type of the encoded set param command (SET_PARAM).
constexpr uint8_t SET_PARAM_DATA_TYPE{0x01};
/// Data type of the encoded params.
constexpr uint8_t PARAMS_DATA_TYPE{0x02};

// Encoded params: header, 3 bytes of the mask which defines the fields
// included in the payload, payload (values of the selected fields in the order
// of the mask bits, the most significant bit of the first mask byte is the
// first field).

/// Position of the first mask byte in the encoded params.
constexpr int PARAMS_MASK_POS{3};
/// Size of the mask in the encoded params, bytes.
constexpr int PARAMS_MASK_SIZE{3};
/// Size of the encoded params header: data type, version and mask, bytes.
constexpr int PARAMS_HEADER_SIZE{PARAMS_MASK_POS + PARAMS_MASK_SIZE};
/// Number of bits in a mask byte.
constexpr size_t MASK_BYTE_BITS{8};
/// The most significant bit of a mask byte: the bit of the first field of the
/// byte.
constexpr unsigned int MASK_FIRST_BIT{0x80U};
/// Number of fields (one mask bit per field).
constexpr size_t FIELDS_NUMBER{24};
static_assert(FIELDS_NUMBER == static_cast<size_t>(PARAMS_MASK_SIZE) *
              MASK_BYTE_BITS, "every mask bit must be used by a field");
/// Number of int and float fields.
constexpr int VALUES_NUMBER{22};
/// Number of bool fields.
constexpr int FLAGS_NUMBER{2};
/// Size of the encoded params with all fields included, bytes. A buffer of
/// this size is enough for any mask.
constexpr int PARAMS_MAX_SIZE{PARAMS_HEADER_SIZE +
                              VALUES_NUMBER * VALUE_SIZE +
                              FLAGS_NUMBER * FLAG_SIZE};
static_assert(PARAMS_MAX_SIZE == 96, "wrong size of encoded params");
static_assert(static_cast<size_t>(VALUES_NUMBER) +
              static_cast<size_t>(FLAGS_NUMBER) == FIELDS_NUMBER,
              "wrong number of fields");

// Encoded command: header, command or param ID (int), argument or param value
// (float).

/// Position of the command or param ID in the encoded command.
constexpr int COMMAND_ID_POS{3};
/// Position of the command argument or param value in the encoded command.
constexpr int COMMAND_VALUE_POS{7};
/// Size of the encoded command of both types, bytes.
constexpr int COMMAND_SIZE{11};
static_assert(COMMAND_VALUE_POS + VALUE_SIZE == COMMAND_SIZE,
              "wrong size of encoded command");

/// The lowest valid command ID.
constexpr int COMMAND_ID_MIN{
    static_cast<int>(rapidpixel::VStabiliserCommand::RESET)};
/// The highest valid command ID. An ID that is not in the range of the enum is
/// rejected: the implementation must never get an unknown ID from the decoder.
constexpr int COMMAND_ID_MAX{
    static_cast<int>(rapidpixel::VStabiliserCommand::HOLD_MSEC)};
/// The lowest valid param ID.
constexpr int PARAM_ID_MIN{
    static_cast<int>(rapidpixel::VStabiliserParam::SCALE_FACTOR)};
/// The highest valid param ID. An ID that is not in the range of the enum is
/// rejected.
constexpr int PARAM_ID_MAX{
    static_cast<int>(rapidpixel::VStabiliserParam::CUSTOM_3)};



/// Location of a VStabiliserParams field in the encoded data.
struct FieldRef
{
    /// Pointer to the field of a VStabiliserParams object.
    void* value{nullptr};
    /// Size of the field in the encoded data: VALUE_SIZE or FLAG_SIZE.
    int size{0};
};



/// Get the table of VStabiliserParams fields in the order they are stored in
/// the encoded data (the same order as the bits of the mask).
std::array<FieldRef, FIELDS_NUMBER> makeFieldTable(
        rapidpixel::VStabiliserParams& params)
{
    return {{
        {&params.scaleFactor, VALUE_SIZE},
        {&params.xOffsetLimit, VALUE_SIZE},
        {&params.yOffsetLimit, VALUE_SIZE},
        {&params.aOffsetLimit, VALUE_SIZE},
        {&params.xFilterCoeff, VALUE_SIZE},
        {&params.yFilterCoeff, VALUE_SIZE},
        {&params.aFilterCoeff, VALUE_SIZE},
        {&params.enable, FLAG_SIZE},
        {&params.transparentBorder, FLAG_SIZE},
        {&params.constXOffset, VALUE_SIZE},
        {&params.constYOffset, VALUE_SIZE},
        {&params.constAOffset, VALUE_SIZE},
        {&params.instantXOffset, VALUE_SIZE},
        {&params.instantYOffset, VALUE_SIZE},
        {&params.instantAOffset, VALUE_SIZE},
        {&params.type, VALUE_SIZE},
        {&params.cutFrequencyHz, VALUE_SIZE},
        {&params.fps, VALUE_SIZE},
        {&params.processingTimeMks, VALUE_SIZE},
        {&params.logMod, VALUE_SIZE},
        {&params.backend, VALUE_SIZE},
        {&params.custom1, VALUE_SIZE},
        {&params.custom2, VALUE_SIZE},
        {&params.custom3, VALUE_SIZE}
    }};
}



/// Get the flags of the params mask in the same order as the table of fields.
std::array<bool, FIELDS_NUMBER> makeMaskFlags(
        const rapidpixel::VStabiliserParamsMask& mask)
{
    return {{
        mask.scaleFactor,
        mask.xOffsetLimit,
        mask.yOffsetLimit,
        mask.aOffsetLimit,
        mask.xFilterCoeff,
        mask.yFilterCoeff,
        mask.aFilterCoeff,
        mask.enable,
        mask.transparentBorder,
        mask.constXOffset,
        mask.constYOffset,
        mask.constAOffset,
        mask.instantXOffset,
        mask.instantYOffset,
        mask.instantAOffset,
        mask.type,
        mask.cutFrequencyHz,
        mask.fps,
        mask.processingTimeMks,
        mask.logMod,
        mask.backend,
        mask.custom1,
        mask.custom2,
        mask.custom3
    }};
}



/// Get the index of the mask byte (the first mask byte has index 0) which
/// holds the bit of the field.
constexpr int maskByteIndex(size_t field)
{
    return static_cast<int>(field / MASK_BYTE_BITS);
}



/// Get the bit of the mask byte which belongs to the field. The first field of
/// a mask byte is the most significant bit.
constexpr uint8_t maskBit(size_t field)
{
    return static_cast<uint8_t>(MASK_FIRST_BIT >> (field % MASK_BYTE_BITS));
}



/// Write the header of the encoded data: data type and version of the library.
void encodeHeader(uint8_t* data, uint8_t dataType)
{
    data[DATA_TYPE_POS] = dataType;
    data[MAJOR_VERSION_POS] = VSTABILISER_MAJOR_VERSION;
    data[MINOR_VERSION_POS] = VSTABILISER_MINOR_VERSION;
}



/// Check that the encoded data was made by the library of the same major and
/// minor version. The data must contain at least the header.
bool isVersionSupported(const uint8_t* data)
{
    return data[MAJOR_VERSION_POS] == VSTABILISER_MAJOR_VERSION &&
           data[MINOR_VERSION_POS] == VSTABILISER_MINOR_VERSION;
}



/// Encode a command of any type. Nothing is written and the size is 0 if the
/// pointer is nullptr.
void encodeCommandData(
        uint8_t* data, int& size, uint8_t dataType, int id, float value)
{
    size = 0;
    if (data == nullptr)
    {
        return;
    }

    encodeHeader(data, dataType);
    std::memcpy(&data[COMMAND_ID_POS], &id, sizeof(id));
    std::memcpy(&data[COMMAND_VALUE_POS], &value, sizeof(value));
    size = COMMAND_SIZE;
}
}



rapidpixel::VStabiliser::~VStabiliser() = default;



std::string rapidpixel::VStabiliser::getVersion()
{
    return VSTABILISER_VERSION;
}



bool rapidpixel::VStabiliserParams::encode(
        uint8_t* data, int bufferSize, int& size,
        rapidpixel::VStabiliserParamsMask* mask)
{
    // The size is 0 if params are not encoded.
    size = 0;

    // Check the pointer and the buffer size. The buffer must be able to hold
    // params with all fields even if the mask excludes some of them.
    if (data == nullptr || bufferSize < PARAMS_MAX_SIZE)
    {
        return false;
    }

    // Encode header.
    encodeHeader(data, PARAMS_DATA_TYPE);

    // Encode mask. All fields are encoded if the mask is not set.
    std::array<bool, FIELDS_NUMBER> selected{};
    if (mask == nullptr)
    {
        selected.fill(true);
    }
    else
    {
        selected = makeMaskFlags(*mask);
    }
    std::memset(&data[PARAMS_MASK_POS], 0, PARAMS_MASK_SIZE);
    for (size_t i = 0; i < FIELDS_NUMBER; ++i)
    {
        if (selected[i])
        {
            data[PARAMS_MASK_POS + maskByteIndex(i)] |= maskBit(i);
        }
    }

    // Encode selected fields.
    const auto fields = makeFieldTable(*this);
    int pos = PARAMS_HEADER_SIZE;
    for (size_t i = 0; i < FIELDS_NUMBER; ++i)
    {
        if (!selected[i])
        {
            continue;
        }

        const FieldRef& field = fields[i];
        if (field.size == FLAG_SIZE)
        {
            data[pos] = *static_cast<const bool*>(field.value) ? 0x01 : 0x00;
        }
        else
        {
            std::memcpy(&data[pos], field.value, VALUE_SIZE);
        }
        pos += field.size;
    }

    size = pos;

    return true;
}



bool rapidpixel::VStabiliserParams::decode(uint8_t* data, int dataSize)
{
    // Check the pointer and the size (the header must be complete).
    if (data == nullptr || dataSize < PARAMS_HEADER_SIZE)
    {
        return false;
    }

    // Check data type.
    if (data[DATA_TYPE_POS] != PARAMS_DATA_TYPE)
    {
        return false;
    }

    // Check version.
    if (!isVersionSupported(data))
    {
        return false;
    }

    // Decode into a temporary object: this object is changed only if the
    // whole data is valid. Fields not selected by the mask are set to zero.
    VStabiliserParams result;
    const auto fields = makeFieldTable(result);
    int pos = PARAMS_HEADER_SIZE;
    for (size_t i = 0; i < FIELDS_NUMBER; ++i)
    {
        const FieldRef& field = fields[i];
        const bool encoded =
                (data[PARAMS_MASK_POS + maskByteIndex(i)] & maskBit(i)) != 0;
        if (!encoded)
        {
            std::memset(field.value, 0, static_cast<size_t>(field.size));
            continue;
        }

        // Check that the field is inside the data (pos <= dataSize here).
        if (dataSize - pos < field.size)
        {
            return false;
        }
        if (field.size == FLAG_SIZE)
        {
            *static_cast<bool*>(field.value) = data[pos] != 0x00;
        }
        else
        {
            std::memcpy(field.value, &data[pos], VALUE_SIZE);
        }
        pos += field.size;
    }

    *this = result;

    return true;
}



void rapidpixel::VStabiliser::encodeSetParamCommand(
        uint8_t* data, int& size, rapidpixel::VStabiliserParam id, float value)
{
    encodeCommandData(data, size, SET_PARAM_DATA_TYPE,
                      static_cast<int>(id), value);
}



void rapidpixel::VStabiliser::encodeCommand(
        uint8_t* data, int& size, rapidpixel::VStabiliserCommand id,
        float value)
{
    encodeCommandData(data, size, COMMAND_DATA_TYPE,
                      static_cast<int>(id), value);
}



int rapidpixel::VStabiliser::decodeCommand(
        uint8_t* data, int size, rapidpixel::VStabiliserParam& paramId,
        rapidpixel::VStabiliserCommand& commandId, float& value)
{
    // Check the pointer and the size. Commands of both types have the same
    // fixed size, so any other size is an error.
    if (data == nullptr || size != COMMAND_SIZE)
    {
        return -1;
    }

    // Check version.
    if (!isVersionSupported(data))
    {
        return -1;
    }

    // Extract data.
    int rawId = 0;
    float argument = 0.0f;
    std::memcpy(&rawId, &data[COMMAND_ID_POS], sizeof(rawId));
    std::memcpy(&argument, &data[COMMAND_VALUE_POS], sizeof(argument));

    // Check command type and the range of the ID. The output arguments are
    // changed only if the command is valid.
    if (data[DATA_TYPE_POS] == COMMAND_DATA_TYPE)
    {
        if (rawId < COMMAND_ID_MIN || rawId > COMMAND_ID_MAX)
        {
            return -1;
        }
        commandId = static_cast<VStabiliserCommand>(rawId);
        value = argument;
        return 0;
    }
    if (data[DATA_TYPE_POS] == SET_PARAM_DATA_TYPE)
    {
        if (rawId < PARAM_ID_MIN || rawId > PARAM_ID_MAX)
        {
            return -1;
        }
        paramId = static_cast<VStabiliserParam>(rawId);
        value = argument;
        return 1;
    }

    return -1;
}
