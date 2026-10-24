#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
#include "ConfigReader.h"
#include "VStabiliser.h"
#include "VStabiliserVersion.h"



namespace
{
/// Number of fields of the VStabiliserParams class which are serialized.
constexpr size_t FIELDS_NUMBER{24};
/// Size of the encoded params with all fields, bytes.
constexpr int FULL_SIZE{96};
/// Size of the encoded params header (data type, version, mask), bytes.
constexpr int HEADER_SIZE{6};
/// Size of the encoded command of any type, bytes.
constexpr int COMMAND_SIZE{11};
/// The first param ID.
constexpr int FIRST_PARAM_ID{1};
/// The last param ID.
constexpr int LAST_PARAM_ID{24};
static_assert(LAST_PARAM_ID ==
              static_cast<int>(rapidpixel::VStabiliserParam::CUSTOM_3),
              "the test must cover all param IDs");
/// The first command ID.
constexpr int FIRST_COMMAND_ID{1};
/// The last command ID.
constexpr int LAST_COMMAND_ID{4};
static_assert(LAST_COMMAND_ID ==
              static_cast<int>(rapidpixel::VStabiliserCommand::HOLD_MSEC),
              "the test must cover all command IDs");
/// Byte which fills buffers to detect writes outside of the expected area.
constexpr uint8_t SENTINEL{0xA5};



/// State of the test random generator. The tests use own deterministic
/// generator: results must be reproducible on all platforms.
uint32_t g_randomState{20240915U};



/// Get next random number of the generator.
uint32_t nextRandom()
{
    g_randomState = g_randomState * 1664525U + 1013904223U;
    return g_randomState;
}



/// Get random number in [0, limit).
int randomInt(int limit)
{
    return static_cast<int>((nextRandom() >> 8) % static_cast<uint32_t>(limit));
}



/// Get random 32 bits.
uint32_t randomBits()
{
    const uint32_t high = nextRandom() & 0xFFFF0000U;
    return high | (nextRandom() >> 16);
}



/// Get random float with any bit pattern (NaN and infinity included).
float randomFloat()
{
    const uint32_t bits = randomBits();
    float value{0.0f};
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}



/// Get random finite float with a short decimal representation.
float randomFiniteFloat()
{
    return static_cast<float>(randomInt(2000001) - 1000000) / 64.0f;
}



/// Get bits of a float: floats are compared by their bits, never with ==.
uint32_t bitsOf(float value)
{
    uint32_t bits{0};
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}



/// Get bits of an int.
uint32_t bitsOf(int value)
{
    return static_cast<uint32_t>(value);
}



/// Get bits of a bool.
uint32_t bitsOf(bool value)
{
    return value ? 1U : 0U;
}



/// Compare a field: the output must be equal to the input if the field is
/// selected and equal to zero if not. The number of mismatches is incremented
/// for each wrong field.
template <class T>
void checkField(const char* name, T in, T out, bool selected, int& errors)
{
    const T expected = selected ? in : T{};
    if (bitsOf(expected) != bitsOf(out))
    {
        std::cout << "Mismatch in field " << name << std::endl;
        ++errors;
    }
}



/// Get the flags of the params mask in the order of the fields.
std::array<bool, FIELDS_NUMBER> maskFlags(
        const rapidpixel::VStabiliserParamsMask& m)
{
    return {{m.scaleFactor, m.xOffsetLimit, m.yOffsetLimit, m.aOffsetLimit,
             m.xFilterCoeff, m.yFilterCoeff, m.aFilterCoeff, m.enable,
             m.transparentBorder, m.constXOffset, m.constYOffset,
             m.constAOffset, m.instantXOffset, m.instantYOffset,
             m.instantAOffset, m.type, m.cutFrequencyHz, m.fps,
             m.processingTimeMks, m.logMod, m.backend, m.custom1, m.custom2,
             m.custom3}};
}



/// Set the mask from the flags in the order of the fields.
rapidpixel::VStabiliserParamsMask makeMask(
        const std::array<bool, FIELDS_NUMBER>& f)
{
    rapidpixel::VStabiliserParamsMask m;
    m.scaleFactor = f[0];
    m.xOffsetLimit = f[1];
    m.yOffsetLimit = f[2];
    m.aOffsetLimit = f[3];
    m.xFilterCoeff = f[4];
    m.yFilterCoeff = f[5];
    m.aFilterCoeff = f[6];
    m.enable = f[7];
    m.transparentBorder = f[8];
    m.constXOffset = f[9];
    m.constYOffset = f[10];
    m.constAOffset = f[11];
    m.instantXOffset = f[12];
    m.instantYOffset = f[13];
    m.instantAOffset = f[14];
    m.type = f[15];
    m.cutFrequencyHz = f[16];
    m.fps = f[17];
    m.processingTimeMks = f[18];
    m.logMod = f[19];
    m.backend = f[20];
    m.custom1 = f[21];
    m.custom2 = f[22];
    m.custom3 = f[23];
    return m;
}



/// Get the mask with all flags set.
rapidpixel::VStabiliserParamsMask makeFullMask()
{
    std::array<bool, FIELDS_NUMBER> flags{};
    flags.fill(true);
    return makeMask(flags);
}



/// Get random mask.
rapidpixel::VStabiliserParamsMask makeRandomMask()
{
    std::array<bool, FIELDS_NUMBER> flags{};
    for (bool& flag : flags)
    {
        flag = randomInt(2) == 1;
    }
    return makeMask(flags);
}



/// Get the size of the encoded params for the mask, bytes. The fields enable
/// and transparentBorder (the 8th and 9th) are 1 byte, all others are 4 bytes.
int expectedSize(const rapidpixel::VStabiliserParamsMask& mask)
{
    const auto flags = maskFlags(mask);
    int size = HEADER_SIZE;
    for (size_t i = 0; i < FIELDS_NUMBER; ++i)
    {
        if (flags[i])
        {
            size += (i == 7 || i == 8) ? 1 : 4;
        }
    }
    return size;
}



/// Fill all fields with random bit patterns (NaN and infinity included).
void fillRandom(rapidpixel::VStabiliserParams& p)
{
    p.scaleFactor = static_cast<int>(randomBits());
    p.xOffsetLimit = static_cast<int>(randomBits());
    p.yOffsetLimit = static_cast<int>(randomBits());
    p.aOffsetLimit = randomFloat();
    p.xFilterCoeff = randomFloat();
    p.yFilterCoeff = randomFloat();
    p.aFilterCoeff = randomFloat();
    p.enable = randomInt(2) == 1;
    p.transparentBorder = randomInt(2) == 1;
    p.constXOffset = static_cast<int>(randomBits());
    p.constYOffset = static_cast<int>(randomBits());
    p.constAOffset = randomFloat();
    p.instantXOffset = static_cast<int>(randomBits());
    p.instantYOffset = static_cast<int>(randomBits());
    p.instantAOffset = randomFloat();
    p.type = static_cast<int>(randomBits());
    p.cutFrequencyHz = randomFloat();
    p.fps = randomFloat();
    p.processingTimeMks = static_cast<int>(randomBits());
    p.logMod = static_cast<int>(randomBits());
    p.backend = static_cast<int>(randomBits());
    p.custom1 = randomFloat();
    p.custom2 = randomFloat();
    p.custom3 = randomFloat();
}



/// Fill the fields with random finite values which survive a conversion to
/// text and back.
void fillRandomFinite(rapidpixel::VStabiliserParams& p)
{
    fillRandom(p);
    p.aOffsetLimit = randomFiniteFloat();
    p.xFilterCoeff = randomFiniteFloat();
    p.yFilterCoeff = randomFiniteFloat();
    p.aFilterCoeff = randomFiniteFloat();
    p.constAOffset = randomFiniteFloat();
    p.instantAOffset = randomFiniteFloat();
    p.cutFrequencyHz = randomFiniteFloat();
    p.fps = randomFiniteFloat();
    p.custom1 = randomFiniteFloat();
    p.custom2 = randomFiniteFloat();
    p.custom3 = randomFiniteFloat();
}



/// Check all fields of the output params object. A field must be equal to the
/// field of the input if the mask flag is set, otherwise it must be zero.
/// Returns the number of wrong fields.
int compareParams(const rapidpixel::VStabiliserParams& in,
                  const rapidpixel::VStabiliserParams& out,
                  const rapidpixel::VStabiliserParamsMask& mask)
{
    int errors = 0;
    checkField("scaleFactor", in.scaleFactor, out.scaleFactor,
               mask.scaleFactor, errors);
    checkField("xOffsetLimit", in.xOffsetLimit, out.xOffsetLimit,
               mask.xOffsetLimit, errors);
    checkField("yOffsetLimit", in.yOffsetLimit, out.yOffsetLimit,
               mask.yOffsetLimit, errors);
    checkField("aOffsetLimit", in.aOffsetLimit, out.aOffsetLimit,
               mask.aOffsetLimit, errors);
    checkField("xFilterCoeff", in.xFilterCoeff, out.xFilterCoeff,
               mask.xFilterCoeff, errors);
    checkField("yFilterCoeff", in.yFilterCoeff, out.yFilterCoeff,
               mask.yFilterCoeff, errors);
    checkField("aFilterCoeff", in.aFilterCoeff, out.aFilterCoeff,
               mask.aFilterCoeff, errors);
    checkField("enable", in.enable, out.enable, mask.enable, errors);
    checkField("transparentBorder", in.transparentBorder,
               out.transparentBorder, mask.transparentBorder, errors);
    checkField("constXOffset", in.constXOffset, out.constXOffset,
               mask.constXOffset, errors);
    checkField("constYOffset", in.constYOffset, out.constYOffset,
               mask.constYOffset, errors);
    checkField("constAOffset", in.constAOffset, out.constAOffset,
               mask.constAOffset, errors);
    checkField("instantXOffset", in.instantXOffset, out.instantXOffset,
               mask.instantXOffset, errors);
    checkField("instantYOffset", in.instantYOffset, out.instantYOffset,
               mask.instantYOffset, errors);
    checkField("instantAOffset", in.instantAOffset, out.instantAOffset,
               mask.instantAOffset, errors);
    checkField("type", in.type, out.type, mask.type, errors);
    checkField("cutFrequencyHz", in.cutFrequencyHz, out.cutFrequencyHz,
               mask.cutFrequencyHz, errors);
    checkField("fps", in.fps, out.fps, mask.fps, errors);
    checkField("processingTimeMks", in.processingTimeMks,
               out.processingTimeMks, mask.processingTimeMks, errors);
    checkField("logMod", in.logMod, out.logMod, mask.logMod, errors);
    checkField("backend", in.backend, out.backend, mask.backend, errors);
    checkField("custom1", in.custom1, out.custom1, mask.custom1, errors);
    checkField("custom2", in.custom2, out.custom2, mask.custom2, errors);
    checkField("custom3", in.custom3, out.custom3, mask.custom3, errors);
    return errors;
}



/// Compare two params objects field by field (all fields).
bool sameParams(const rapidpixel::VStabiliserParams& a,
                const rapidpixel::VStabiliserParams& b)
{
    return compareParams(a, b, makeFullMask()) == 0;
}



/// Print a failed check and return false.
bool fail(const char* message)
{
    std::cout << message << std::endl;
    return false;
}



/// Run one test and print the result.
bool runTest(const char* title, bool (*test)())
{
    std::cout << title << ":" << std::endl;
    const bool passed = test();
    std::cout << (passed ? "OK" : "ERROR") << std::endl << std::endl;
    return passed;
}



/// File which exists only while the object exists: the file is removed in the
/// destructor, so a test that stops early does not leave it. A failed removal
/// is reported.
class TempFile
{
public:
    /// Class constructor.
    explicit TempFile(const char* name) : m_name(name)
    {
    }

    /// Class destructor. Removes the file if it was not removed yet.
    ~TempFile()
    {
        if (!m_removed)
        {
            remove();
        }
    }

    TempFile(const TempFile&) = delete;
    TempFile(TempFile&&) = delete;
    TempFile& operator= (const TempFile&) = delete;
    TempFile& operator= (TempFile&&) = delete;

    /// Get the file name.
    const std::string& name() const
    {
        return m_name;
    }

    /// Remove the file. Returns true on success.
    bool remove()
    {
        m_removed = true;
        const bool removed = std::remove(m_name.c_str()) == 0;
        if (!removed)
        {
            std::cout << "Can't remove file " << m_name << std::endl;
        }
        return removed;
    }

private:

    /// Name of the file.
    std::string m_name;
    /// Flag: the removal of the file was done.
    bool m_removed{false};
};



/// Copy test: the copy constructor and the assignment copy all fields.
bool copyTest()
{
    g_randomState = 1U;
    rapidpixel::VStabiliserParams in;
    fillRandom(in);

    // Copy constructor.
    const rapidpixel::VStabiliserParams copy = in;
    if (compareParams(in, copy, makeFullMask()) != 0)
    {
        return fail("Copy constructor");
    }

    // Copy assignment.
    rapidpixel::VStabiliserParams assigned;
    assigned = in;
    if (compareParams(in, assigned, makeFullMask()) != 0)
    {
        return fail("Copy assignment");
    }

    return true;
}



/// Layout test: the byte layout of the encoded params is a part of the
/// protocol and must not change. Offsets and values are written here
/// independently of the library code.
bool layoutTest()
{
    // Params with a distinct value in each field.
    rapidpixel::VStabiliserParams p;
    p.scaleFactor = 0x01020304;
    p.xOffsetLimit = 0x11121314;
    p.yOffsetLimit = 0x21222324;
    p.aOffsetLimit = 1.5f;
    p.xFilterCoeff = 2.5f;
    p.yFilterCoeff = 3.5f;
    p.aFilterCoeff = 4.5f;
    p.enable = true;
    p.transparentBorder = false;
    p.constXOffset = 0x31323334;
    p.constYOffset = 0x41424344;
    p.constAOffset = 5.5f;
    p.instantXOffset = 0x51525354;
    p.instantYOffset = 0x61626364;
    p.instantAOffset = 6.5f;
    p.type = 0x71727374;
    p.cutFrequencyHz = 7.5f;
    p.fps = 8.5f;
    p.processingTimeMks = 0x01110213;
    p.logMod = 0x01110214;
    p.backend = 0x01110215;
    p.custom1 = 9.5f;
    p.custom2 = 10.5f;
    p.custom3 = 11.5f;

    // Encode all fields.
    std::array<uint8_t, FULL_SIZE> data{};
    int size = 0;
    if (!p.encode(data.data(), FULL_SIZE, size) || size != FULL_SIZE)
    {
        return fail("Can't encode all fields");
    }

    // Header: data type, major version, minor version, mask (24 bits set).
    if (data[0] != 0x02 || data[1] != VSTABILISER_MAJOR_VERSION ||
        data[2] != VSTABILISER_MINOR_VERSION)
    {
        return fail("Wrong header");
    }
    if (data[3] != 0xFF || data[4] != 0xFF || data[5] != 0xFF)
    {
        return fail("Wrong mask");
    }

    // Offsets of the 4 bytes fields in the encoded data.
    const struct
    {
        const char* name;
        size_t offset;
        uint32_t bits;
    } fields[] = {
        {"scaleFactor", 6, bitsOf(p.scaleFactor)},
        {"xOffsetLimit", 10, bitsOf(p.xOffsetLimit)},
        {"yOffsetLimit", 14, bitsOf(p.yOffsetLimit)},
        {"aOffsetLimit", 18, bitsOf(p.aOffsetLimit)},
        {"xFilterCoeff", 22, bitsOf(p.xFilterCoeff)},
        {"yFilterCoeff", 26, bitsOf(p.yFilterCoeff)},
        {"aFilterCoeff", 30, bitsOf(p.aFilterCoeff)},
        {"constXOffset", 36, bitsOf(p.constXOffset)},
        {"constYOffset", 40, bitsOf(p.constYOffset)},
        {"constAOffset", 44, bitsOf(p.constAOffset)},
        {"instantXOffset", 48, bitsOf(p.instantXOffset)},
        {"instantYOffset", 52, bitsOf(p.instantYOffset)},
        {"instantAOffset", 56, bitsOf(p.instantAOffset)},
        {"type", 60, bitsOf(p.type)},
        {"cutFrequencyHz", 64, bitsOf(p.cutFrequencyHz)},
        {"fps", 68, bitsOf(p.fps)},
        {"processingTimeMks", 72, bitsOf(p.processingTimeMks)},
        {"logMod", 76, bitsOf(p.logMod)},
        {"backend", 80, bitsOf(p.backend)},
        {"custom1", 84, bitsOf(p.custom1)},
        {"custom2", 88, bitsOf(p.custom2)},
        {"custom3", 92, bitsOf(p.custom3)}};
    for (const auto& field : fields)
    {
        uint32_t bits{0};
        std::memcpy(&bits, &data[field.offset], sizeof(bits));
        if (bits != field.bits)
        {
            std::cout << "Wrong position of field " << field.name << std::endl;
            return false;
        }
    }

    // Bool fields are 1 byte: 0x01 and 0x00.
    if (data[34] != 0x01 || data[35] != 0x00)
    {
        return fail("Wrong bool fields");
    }

    // The default mask excludes instant offsets: mask bytes are 0xFF, 0xF1,
    // 0xFF and the data is 12 bytes shorter.
    rapidpixel::VStabiliserParamsMask defaultMask;
    std::array<uint8_t, FULL_SIZE> masked{};
    if (!p.encode(masked.data(), FULL_SIZE, size, &defaultMask) ||
        size != FULL_SIZE - 12)
    {
        return fail("Can't encode with the default mask");
    }
    if (masked[3] != 0xFF || masked[4] != 0xF1 || masked[5] != 0xFF)
    {
        return fail("Wrong default mask");
    }

    return true;
}



/// Encode/decode test: all fields are encoded and decoded.
bool encodeDecodeTest()
{
    g_randomState = 2U;
    for (int i = 0; i < 2000; ++i)
    {
        rapidpixel::VStabiliserParams in;
        fillRandom(in);

        // Encode data without mask and with the mask with all flags set: the
        // data must be the same.
        std::array<uint8_t, 1024> data{};
        std::array<uint8_t, 1024> dataWithMask{};
        int size = 0;
        int sizeWithMask = 0;
        auto fullMask = makeFullMask();
        if (!in.encode(data.data(), 1024, size) || size != FULL_SIZE)
        {
            return fail("Can't encode data");
        }
        if (!in.encode(dataWithMask.data(), 1024, sizeWithMask, &fullMask) ||
            sizeWithMask != size ||
            std::memcmp(data.data(), dataWithMask.data(),
                        static_cast<size_t>(size)) != 0)
        {
            return fail("Different data with the full mask");
        }

        // Decode data.
        rapidpixel::VStabiliserParams out;
        if (!out.decode(data.data(), size))
        {
            return fail("Can't decode data");
        }
        if (compareParams(in, out, fullMask) != 0)
        {
            return false;
        }
    }

    return true;
}



/// Encode/decode test with params mask.
bool encodeDecodeWithMaskTest()
{
    g_randomState = 3U;

    // Random masks.
    for (int i = 0; i < 5000; ++i)
    {
        rapidpixel::VStabiliserParams in;
        fillRandom(in);
        auto mask = makeRandomMask();

        std::array<uint8_t, FULL_SIZE> data{};
        int size = 0;
        if (!in.encode(data.data(), FULL_SIZE, size, &mask))
        {
            return fail("Can't encode data with mask");
        }
        if (size != expectedSize(mask))
        {
            std::cout << "Wrong size: " << size << " instead of "
                      << expectedSize(mask) << std::endl;
            return false;
        }

        // Fields which are not in the mask must be zero in the result: the
        // output object is prepared with random values to check this.
        rapidpixel::VStabiliserParams out;
        fillRandom(out);
        if (!out.decode(data.data(), size))
        {
            return fail("Can't decode data with mask");
        }
        if (compareParams(in, out, mask) != 0)
        {
            return false;
        }
    }

    // Every single field.
    rapidpixel::VStabiliserParams in;
    fillRandom(in);
    for (size_t field = 0; field < FIELDS_NUMBER; ++field)
    {
        std::array<bool, FIELDS_NUMBER> flags{};
        flags[field] = true;
        auto mask = makeMask(flags);
        std::array<uint8_t, FULL_SIZE> data{};
        int size = 0;
        rapidpixel::VStabiliserParams out;
        if (!in.encode(data.data(), FULL_SIZE, size, &mask) ||
            size != expectedSize(mask) || !out.decode(data.data(), size) ||
            compareParams(in, out, mask) != 0)
        {
            std::cout << "Wrong single field mask, field " << field
                      << std::endl;
            return false;
        }
    }

    // Empty mask: only the header is encoded.
    auto emptyMask = makeMask(std::array<bool, FIELDS_NUMBER>{});
    std::array<uint8_t, FULL_SIZE> data{};
    int size = 0;
    rapidpixel::VStabiliserParams out;
    fillRandom(out);
    if (!in.encode(data.data(), FULL_SIZE, size, &emptyMask) ||
        size != HEADER_SIZE || !out.decode(data.data(), size) ||
        compareParams(in, out, emptyMask) != 0)
    {
        return fail("Wrong empty mask");
    }

    return true;
}



/// Buffer boundaries test: nothing is written outside of the encoded data and
/// nothing is written if the call is rejected.
bool bufferBoundariesTest()
{
    g_randomState = 4U;
    rapidpixel::VStabiliserParams in;
    fillRandom(in);

    // Null pointer: rejected, the size is zero.
    int size = 1234;
    if (in.encode(nullptr, FULL_SIZE, size) || size != 0)
    {
        return fail("nullptr buffer must be rejected");
    }

    // Buffers which are too small, with and without mask. The buffer must be
    // untouched.
    auto emptyMask = makeMask(std::array<bool, FIELDS_NUMBER>{});
    const int sizes[] = {std::numeric_limits<int>::min(), -1, 0, 1,
                         HEADER_SIZE, FULL_SIZE - 1};
    for (const int bufferSize : sizes)
    {
        std::array<uint8_t, FULL_SIZE + 8> buffer{};
        buffer.fill(SENTINEL);
        size = 1234;
        if (in.encode(buffer.data(), bufferSize, size) || size != 0)
        {
            return fail("Too small buffer must be rejected");
        }
        size = 1234;
        if (in.encode(buffer.data(), bufferSize, size, &emptyMask) ||
            size != 0)
        {
            return fail("Too small buffer must be rejected with mask");
        }
        for (const uint8_t value : buffer)
        {
            if (value != SENTINEL)
            {
                return fail("A rejected call changed the buffer");
            }
        }
    }

    // Exact buffer size and bigger buffer: nothing is written after the
    // encoded data.
    const int fits[] = {FULL_SIZE, FULL_SIZE + 1, 1024};
    for (const int bufferSize : fits)
    {
        std::array<uint8_t, 1024 + 8> buffer{};
        buffer.fill(SENTINEL);
        if (!in.encode(buffer.data(), bufferSize, size) || size != FULL_SIZE)
        {
            return fail("Can't encode to the buffer");
        }
        for (size_t i = static_cast<size_t>(size); i < buffer.size(); ++i)
        {
            if (buffer[i] != SENTINEL)
            {
                return fail("Data written after the end");
            }
        }
    }

    return true;
}



/// Decode the data into a copy of the reference object and check that the data
/// is rejected and that the object is not changed.
bool isRejected(const rapidpixel::VStabiliserParams& reference,
                uint8_t* data, int dataSize)
{
    rapidpixel::VStabiliserParams object = reference;
    const bool decoded = object.decode(data, dataSize);
    return !decoded && sameParams(object, reference);
}



/// Check that every truncation of the data (for masks of a few kinds) is
/// rejected.
bool checkTruncations(const rapidpixel::VStabiliserParams& source)
{
    rapidpixel::VStabiliserParams reference;
    fillRandom(reference);
    for (int mode = 0; mode < 64; ++mode)
    {
        auto mask = mode == 0 ? makeFullMask() : makeRandomMask();
        std::array<uint8_t, FULL_SIZE> masked{};
        int maskedSize = 0;
        rapidpixel::VStabiliserParams encoder = source;
        if (!encoder.encode(masked.data(), FULL_SIZE, maskedSize, &mask))
        {
            return fail("Can't encode data with mask");
        }
        for (int length = 0; length < maskedSize; ++length)
        {
            if (!isRejected(reference, masked.data(), length))
            {
                std::cout << "Truncated data accepted, size " << length
                          << std::endl;
                return false;
            }
        }
    }

    return true;
}



/// Check that every wrong value of the data type byte, of the major version
/// byte and of the minor version byte is rejected.
bool checkHeaderBytes(const rapidpixel::VStabiliserParams& reference,
                      const std::array<uint8_t, FULL_SIZE>& data)
{
    for (size_t position = 0; position < 3; ++position)
    {
        for (int value = 0; value < 256; ++value)
        {
            std::array<uint8_t, FULL_SIZE> changed = data;
            if (changed[position] == static_cast<uint8_t>(value))
            {
                continue;
            }
            changed[position] = static_cast<uint8_t>(value);
            if (!isRejected(reference, changed.data(), FULL_SIZE))
            {
                std::cout << "Wrong header byte " << position << " accepted"
                          << std::endl;
                return false;
            }
        }
    }

    return true;
}



/// Check that bytes after the last field are ignored.
bool checkTrailingBytes(const std::array<uint8_t, FULL_SIZE>& data)
{
    std::array<uint8_t, FULL_SIZE + 16> longer{};
    longer.fill(0xFF);
    std::memcpy(longer.data(), data.data(), data.size());
    std::array<uint8_t, FULL_SIZE> exactData = data;
    rapidpixel::VStabiliserParams exact;
    rapidpixel::VStabiliserParams withTail;
    return exact.decode(exactData.data(), FULL_SIZE) &&
           withTail.decode(longer.data(), static_cast<int>(longer.size())) &&
           sameParams(exact, withTail);
}



/// Check that a bool field is true for any byte except 0x00.
bool checkBoolBytes(const rapidpixel::VStabiliserParams& source)
{
    auto onlyFlags = makeMask(std::array<bool, FIELDS_NUMBER>{});
    onlyFlags.enable = true;
    onlyFlags.transparentBorder = true;
    std::array<uint8_t, FULL_SIZE> flagsData{};
    int flagsSize = 0;
    rapidpixel::VStabiliserParams encoder = source;
    if (!encoder.encode(flagsData.data(), FULL_SIZE, flagsSize, &onlyFlags) ||
        flagsSize != HEADER_SIZE + 2)
    {
        return fail("Can't encode bool fields");
    }
    const uint8_t bytes[] = {0x00, 0x01, 0x02, 0x7F, 0x80, 0xFF};
    for (const uint8_t first : bytes)
    {
        for (const uint8_t second : bytes)
        {
            flagsData[HEADER_SIZE] = first;
            flagsData[HEADER_SIZE + 1] = second;
            rapidpixel::VStabiliserParams decoded;
            if (!decoded.decode(flagsData.data(), flagsSize) ||
                decoded.enable != (first != 0x00) ||
                decoded.transparentBorder != (second != 0x00))
            {
                return fail("Wrong decoding of bool fields");
            }
        }
    }

    return true;
}



/// Invalid params data test: each rejected call returns false and does not
/// change the object.
bool invalidParamsDataTest()
{
    g_randomState = 5U;
    rapidpixel::VStabiliserParams source;
    fillRandom(source);
    std::array<uint8_t, FULL_SIZE> data{};
    int size = 0;
    if (!source.encode(data.data(), FULL_SIZE, size))
    {
        return fail("Can't encode data");
    }

    // The object that must stay unchanged after every rejected decode.
    rapidpixel::VStabiliserParams reference;
    fillRandom(reference);

    // Null pointer and wrong sizes.
    if (!isRejected(reference, nullptr, FULL_SIZE))
    {
        return fail("nullptr data must be rejected");
    }
    const int badSizes[] = {std::numeric_limits<int>::min(), -1, 0};
    for (const int badSize : badSizes)
    {
        if (!isRejected(reference, data.data(), badSize))
        {
            return fail("Wrong size must be rejected");
        }
    }

    return checkTruncations(source) &&
           checkHeaderBytes(reference, data) &&
           checkTrailingBytes(data) &&
           checkBoolBytes(source);
}



/// Damaged data test: random byte replacement, random size, tails. The result
/// must be deterministic; if the data is accepted it must encode to data that
/// decodes to the same object; if it is rejected the object must not change.
bool damagedDataTest()
{
    g_randomState = 6U;
    int accepted = 0;
    int rejected = 0;
    for (int i = 0; i < 60000; ++i)
    {
        rapidpixel::VStabiliserParams source;
        fillRandom(source);
        auto mask = makeRandomMask();
        std::array<uint8_t, FULL_SIZE + 16> data{};
        for (uint8_t& value : data)
        {
            value = static_cast<uint8_t>(randomBits());
        }
        int size = 0;
        if (!source.encode(data.data(), FULL_SIZE, size, &mask))
        {
            return fail("Can't encode data");
        }

        // Damage the data. Half of the samples keep the header valid to reach
        // the decoding of the fields.
        const int damaged = 1 + randomInt(4);
        for (int k = 0; k < damaged; ++k)
        {
            const int minPos = (i % 2 == 0) ? 3 : 0;
            const int pos = minPos + randomInt(size - minPos);
            data[static_cast<size_t>(pos)] = static_cast<uint8_t>(randomBits());
        }
        const int newSize = randomInt(size + 16);

        // Decode twice into different objects: the results must be equal.
        rapidpixel::VStabiliserParams first;
        rapidpixel::VStabiliserParams second;
        fillRandom(first);
        second = first;
        const rapidpixel::VStabiliserParams before = first;
        const bool firstResult = first.decode(data.data(), newSize);
        const bool secondResult = second.decode(data.data(), newSize);
        if (firstResult != secondResult || !sameParams(first, second))
        {
            return fail("The result of the decoding is not deterministic");
        }
        if (!firstResult)
        {
            ++rejected;
            if (!sameParams(first, before))
            {
                return fail("A rejected decode changed the object");
            }
            continue;
        }

        // Accepted: encode all fields and decode again.
        ++accepted;
        std::array<uint8_t, FULL_SIZE> encoded{};
        int encodedSize = 0;
        rapidpixel::VStabiliserParams again;
        if (!first.encode(encoded.data(), FULL_SIZE, encodedSize) ||
            !again.decode(encoded.data(), encodedSize) ||
            !sameParams(first, again))
        {
            return fail("Decoded params do not survive encode/decode");
        }
    }

    // Both results must occur, otherwise the campaign checks nothing.
    std::cout << "Accepted: " << accepted << ", rejected: " << rejected
              << std::endl;
    return accepted > 0 && rejected > 0;
}



/// Encode a command of one type, check the size, the layout and that nothing
/// is written after the end, decode the command and check the result. If
/// setParam is true the command is SET_PARAM, otherwise it is COMMAND.
bool checkCommandRoundTrip(bool setParam, int id)
{
    const float outValue = randomFloat();
    std::array<uint8_t, 32> data{};
    data.fill(SENTINEL);
    int size = 0;
    if (setParam)
    {
        rapidpixel::VStabiliser::encodeSetParamCommand(
                    data.data(), size,
                    static_cast<rapidpixel::VStabiliserParam>(id), outValue);
    }
    else
    {
        rapidpixel::VStabiliser::encodeCommand(
                    data.data(), size,
                    static_cast<rapidpixel::VStabiliserCommand>(id), outValue);
    }
    if (size != COMMAND_SIZE)
    {
        return fail("Wrong size of the encoded command");
    }
    for (size_t k = COMMAND_SIZE; k < data.size(); ++k)
    {
        if (data[k] != SENTINEL)
        {
            return fail("Command data written after the end");
        }
    }

    // Layout: data type, major and minor version, ID, value.
    int encodedId{0};
    std::memcpy(&encodedId, &data[3], sizeof(encodedId));
    const uint8_t dataType = setParam ? 0x01 : 0x00;
    if (data[0] != dataType || data[1] != VSTABILISER_MAJOR_VERSION ||
        data[2] != VSTABILISER_MINOR_VERSION || encodedId != id)
    {
        return fail("Wrong layout of the encoded command");
    }

    // Decode: only the ID of the command type is set.
    auto paramId = rapidpixel::VStabiliserParam::CUSTOM_3;
    auto commandId = rapidpixel::VStabiliserCommand::OFF;
    float value{0.0f};
    const int result = rapidpixel::VStabiliser::decodeCommand(
                data.data(), size, paramId, commandId, value);
    if (result != (setParam ? 1 : 0))
    {
        return fail("Command not decoded");
    }
    const bool sameId = setParam
            ? static_cast<int>(paramId) == id &&
              commandId == rapidpixel::VStabiliserCommand::OFF
            : static_cast<int>(commandId) == id &&
              paramId == rapidpixel::VStabiliserParam::CUSTOM_3;
    if (!sameId || bitsOf(value) != bitsOf(outValue))
    {
        return fail("Wrong decoded command");
    }

    return true;
}



/// Encode/decode commands test: every valid ID of both command types.
bool commandsTest()
{
    g_randomState = 7U;

    // Action commands and set param commands.
    for (int id = FIRST_COMMAND_ID; id <= LAST_COMMAND_ID; ++id)
    {
        for (int i = 0; i < 50; ++i)
        {
            if (!checkCommandRoundTrip(false, id))
            {
                return false;
            }
        }
    }
    for (int id = FIRST_PARAM_ID; id <= LAST_PARAM_ID; ++id)
    {
        for (int i = 0; i < 50; ++i)
        {
            if (!checkCommandRoundTrip(true, id))
            {
                return false;
            }
        }
    }

    // Null pointer: nothing is written, the size is zero.
    int size = 1234;
    rapidpixel::VStabiliser::encodeCommand(
                nullptr, size, rapidpixel::VStabiliserCommand::ON);
    if (size != 0)
    {
        return fail("nullptr must give zero size (command)");
    }
    size = 1234;
    rapidpixel::VStabiliser::encodeSetParamCommand(
                nullptr, size, rapidpixel::VStabiliserParam::FPS, 1.0f);
    if (size != 0)
    {
        return fail("nullptr must give zero size (set param)");
    }

    return true;
}



/// The output arguments which must not change if a command is rejected.
constexpr rapidpixel::VStabiliserParam REFERENCE_PARAM{
    rapidpixel::VStabiliserParam::CUSTOM_2};
constexpr rapidpixel::VStabiliserCommand REFERENCE_COMMAND{
    rapidpixel::VStabiliserCommand::OFF};
constexpr float REFERENCE_VALUE{12345.0f};



/// Decode the command and check that it is rejected (-1) and that the output
/// arguments are not changed.
bool isCommandRejected(uint8_t* data, int size)
{
    auto paramId = REFERENCE_PARAM;
    auto commandId = REFERENCE_COMMAND;
    float value{REFERENCE_VALUE};
    const int result = rapidpixel::VStabiliser::decodeCommand(
                data, size, paramId, commandId, value);
    return result == -1 && paramId == REFERENCE_PARAM &&
           commandId == REFERENCE_COMMAND &&
           bitsOf(value) == bitsOf(REFERENCE_VALUE);
}



/// Encode a valid command of one type (see checkCommandRoundTrip(...)).
void encodeValidCommand(bool setParam, std::array<uint8_t, 32>& data)
{
    data.fill(SENTINEL);
    int size = 0;
    if (setParam)
    {
        rapidpixel::VStabiliser::encodeSetParamCommand(
                    data.data(), size, rapidpixel::VStabiliserParam::FPS, 25.0f);
    }
    else
    {
        rapidpixel::VStabiliser::encodeCommand(
                    data.data(), size,
                    rapidpixel::VStabiliserCommand::HOLD_MSEC, 100.0f);
    }
}



/// Check that a null pointer and every wrong size (oversized too) are
/// rejected.
bool checkCommandSizes(std::array<uint8_t, 32>& data)
{
    if (!isCommandRejected(nullptr, COMMAND_SIZE))
    {
        return fail("nullptr command must be rejected");
    }
    const int badSizes[] = {std::numeric_limits<int>::min(), -1};
    for (const int badSize : badSizes)
    {
        if (!isCommandRejected(data.data(), badSize))
        {
            return fail("Negative size must be rejected");
        }
    }
    for (int badSize = 0; badSize <= static_cast<int>(data.size()); ++badSize)
    {
        if (badSize != COMMAND_SIZE &&
            !isCommandRejected(data.data(), badSize))
        {
            std::cout << "Wrong size accepted: " << badSize << std::endl;
            return false;
        }
    }

    return true;
}



/// Check that every wrong value of the data type byte, of the major version
/// byte and of the minor version byte is rejected. Values 0 and 1 of the data
/// type byte are valid types.
bool checkCommandHeader(const std::array<uint8_t, 32>& data)
{
    for (size_t position = 0; position < 3; ++position)
    {
        for (int newValue = 0; newValue < 256; ++newValue)
        {
            int validValue = VSTABILISER_MINOR_VERSION;
            if (position == 1)
            {
                validValue = VSTABILISER_MAJOR_VERSION;
            }
            const bool valid = position == 0 ? (newValue == 0 || newValue == 1)
                                             : newValue == validValue;
            if (valid)
            {
                continue;
            }
            std::array<uint8_t, 32> changed = data;
            changed[position] = static_cast<uint8_t>(newValue);
            if (!isCommandRejected(changed.data(), COMMAND_SIZE))
            {
                std::cout << "Wrong header byte " << position
                          << " accepted: " << newValue << std::endl;
                return false;
            }
        }
    }

    return true;
}



/// Check that IDs outside of the enum range are rejected. The IDs are the
/// first ID after the last valid ID of the command type and some other values.
bool checkCommandIds(const std::array<uint8_t, 32>& data, int firstBadId)
{
    const int ids[] = {std::numeric_limits<int>::min(), -1, 0, 1000,
                       std::numeric_limits<int>::max(), firstBadId};
    for (const int badId : ids)
    {
        std::array<uint8_t, 32> changed = data;
        std::memcpy(&changed[3], &badId, sizeof(badId));
        if (!isCommandRejected(changed.data(), COMMAND_SIZE))
        {
            std::cout << "Wrong ID accepted: " << badId << std::endl;
            return false;
        }
    }

    return true;
}



/// Invalid commands test: every rejected command returns -1 and does not
/// change the output arguments.
bool invalidCommandsTest()
{
    g_randomState = 8U;
    for (int type = 0; type < 2; ++type)
    {
        const bool setParam = type == 1;
        std::array<uint8_t, 32> data{};
        encodeValidCommand(setParam, data);
        const int firstBadId = setParam ? LAST_PARAM_ID + 1
                                        : LAST_COMMAND_ID + 1;
        if (!checkCommandSizes(data) || !checkCommandHeader(data) ||
            !checkCommandIds(data, firstBadId))
        {
            return false;
        }
    }

    return true;
}



/// JSON read/write test.
bool jsonReadWriteTest()
{
    g_randomState = 9U;
    rapidpixel::VStabiliserParams in;
    fillRandomFinite(in);

    // The fields which are not written to JSON keep their default values (0)
    // in the object that is read.
    rapidpixel::VStabiliserParamsMask jsonMask = makeFullMask();
    jsonMask.instantXOffset = false;
    jsonMask.instantYOffset = false;
    jsonMask.instantAOffset = false;
    jsonMask.processingTimeMks = false;

    // Write params to string and read them back.
    rapidpixel::ConfigReader stringWriter;
    std::string text;
    if (!stringWriter.set(in, "VStabiliserParams") ||
        !stringWriter.writeToString(text))
    {
        return fail("Can't write params to string");
    }
    rapidpixel::ConfigReader stringReader;
    rapidpixel::VStabiliserParams fromString;
    if (!stringReader.readFromString(text) ||
        !stringReader.get(fromString, "VStabiliserParams"))
    {
        return fail("Can't read params from string");
    }
    if (compareParams(in, fromString, jsonMask) != 0)
    {
        return false;
    }

    // Write params to file and read them back. The file is removed.
    TempFile file("TestVStabiliserParams.json");
    rapidpixel::ConfigReader inConfig;
    inConfig.set(in, "VStabiliserParams");
    if (!inConfig.writeToFile(file.name()))
    {
        return fail("Can't write params to file");
    }
    rapidpixel::ConfigReader outConfig;
    if (!outConfig.readFromFile(file.name()))
    {
        return fail("Can't open config file");
    }
    rapidpixel::VStabiliserParams out;
    if (!outConfig.get(out, "VStabiliserParams"))
    {
        return fail("Can't read params from file");
    }
    if (compareParams(in, out, jsonMask) != 0)
    {
        return false;
    }
    if (!file.remove())
    {
        return fail("Can't remove the test file");
    }

    // Invalid and incomplete documents are rejected and the object is not
    // changed.
    const std::string documents[] = {
        "",
        "{",
        "[1, 2, 3]",
        "{\"Other\": {\"scaleFactor\": 2}}",
        "{\"VStabiliserParams\": 5}",
        "{\"VStabiliserParams\": {}}",
        "{\"VStabiliserParams\": {\"scaleFactor\": 2}}",
        "{\"VStabiliserParams\": {\"scaleFactor\": \"two\"}}"};
    for (const std::string& document : documents)
    {
        rapidpixel::ConfigReader reader;
        rapidpixel::VStabiliserParams object;
        fillRandom(object);
        const rapidpixel::VStabiliserParams reference = object;
        if (reader.readFromString(document) &&
            reader.get(object, "VStabiliserParams"))
        {
            std::cout << "Document accepted: " << document << std::endl;
            return false;
        }
        if (!sameParams(object, reference))
        {
            return fail("A rejected document changed the object");
        }
    }

    // A file that does not exist.
    rapidpixel::ConfigReader missing;
    if (missing.readFromFile("NoSuchVStabiliserParams.json"))
    {
        return fail("A missing file must be rejected");
    }

    return true;
}



/// Version test: the version string is made of the version macros.
bool versionTest()
{
    const std::string expected = std::to_string(VSTABILISER_MAJOR_VERSION) +
            "." + std::to_string(VSTABILISER_MINOR_VERSION) + "." +
            std::to_string(VSTABILISER_PATCH_VERSION);
    std::cout << "Version: " << rapidpixel::VStabiliser::getVersion()
              << std::endl;
    return rapidpixel::VStabiliser::getVersion() == expected &&
           expected == VSTABILISER_VERSION;
}
}



// Entry point.
int main()
{
    std::cout << "#####################################" << std::endl;
    std::cout << "#                                   #" << std::endl;
    std::cout << "# VStabiliserParams test            #" << std::endl;
    std::cout << "#                                   #" << std::endl;
    std::cout << "#####################################" << std::endl;
    std::cout << std::endl;

    int failed = 0;
    failed += runTest("Copy test", copyTest) ? 0 : 1;
    failed += runTest("Layout test", layoutTest) ? 0 : 1;
    failed += runTest("Encode/Decode test", encodeDecodeTest) ? 0 : 1;
    failed += runTest("Encode/Decode test with params mask",
                      encodeDecodeWithMaskTest) ? 0 : 1;
    failed += runTest("Buffer boundaries test", bufferBoundariesTest) ? 0 : 1;
    failed += runTest("Invalid params data test",
                      invalidParamsDataTest) ? 0 : 1;
    failed += runTest("Damaged params data test", damagedDataTest) ? 0 : 1;
    failed += runTest("Encode/Decode commands test", commandsTest) ? 0 : 1;
    failed += runTest("Invalid commands test", invalidCommandsTest) ? 0 : 1;
    failed += runTest("JSON read/write test", jsonReadWriteTest) ? 0 : 1;
    failed += runTest("Version test", versionTest) ? 0 : 1;

    if (failed != 0)
    {
        std::cout << "Failed tests: " << failed << std::endl;
        return 1;
    }
    std::cout << "All tests passed" << std::endl;

    return 0;
}
