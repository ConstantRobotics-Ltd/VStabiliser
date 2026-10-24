#include "CustomVStabiliser.h"
#include "CustomVStabiliserVersion.h"
#include <cstdint>
#include <limits>
#include <mutex>
#include <string>
#include "Frame.h"
#include "VStabiliser.h"



namespace
{
/// Limit of float values which can be converted to int. 2^31 is exactly
/// representable as float, so (-LIMIT <= value < LIMIT) is the exact range of
/// int.
constexpr float INT_RANGE_LIMIT{2147483648.0f};
/// The biggest int value. The smallest allowed int value of the params which
/// can be negative is -INT_MAX_VALUE: the smallest int can not be negated.
constexpr int INT_MAX_VALUE{std::numeric_limits<int>::max()};
/// The biggest finite float value. A value above it is infinity.
constexpr float FLOAT_MAX_VALUE{std::numeric_limits<float>::max()};
/// The smallest positive float value: used for params which must be positive.
constexpr float FLOAT_MIN_POSITIVE{std::numeric_limits<float>::min()};
/// The minimum scale factor: 1 - the original frame size.
constexpr int MIN_SCALE_FACTOR{1};
/// The minimum value of the smoothing coefficients: 0 - the constant camera
/// motion is not compensated.
constexpr float MIN_FILTER_COEFF{0.0f};
/// The maximum value of the smoothing coefficients: 1 - no smoothing.
constexpr float MAX_FILTER_COEFF{1.0f};
/// The maximum algorithm type: 0 - 2D type 1, 1 - 2D type 2, 2 - 3D.
constexpr int MAX_TYPE{2};
/// The maximum logging mode: 3 - file and terminal.
constexpr int MAX_LOG_MODE{3};



/// Check that the value is inside [minValue, maxValue]. The check is false for
/// NaN.
template <class T>
bool inRange(T value, T minValue, T maxValue)
{
    return value >= minValue && value <= maxValue;
}



/// Convert float param value to int. A conversion of NaN or of a value outside
/// of the int range is undefined behavior, so such values are rejected. The
/// condition is written as !(a && b) on purpose: the comparisons are false for
/// NaN, so NaN is rejected.
bool toInt(float value, int& result)
{
    if (!(value >= -INT_RANGE_LIMIT && value < INT_RANGE_LIMIT))
    {
        return false;
    }
    result = static_cast<int>(value);
    return true;
}



/// Assign the param value to the int target if it is an int inside
/// [minValue, maxValue]. The target is not changed otherwise.
bool assignInt(int& target, float value, int minValue, int maxValue)
{
    int converted{0};
    if (!toInt(value, converted) || !inRange(converted, minValue, maxValue))
    {
        return false;
    }
    target = converted;
    return true;
}



/// Assign the param value to the float target if it is inside
/// [minValue, maxValue]. The target is not changed otherwise.
bool assignFloat(float& target, float value, float minValue, float maxValue)
{
    if (!inRange(value, minValue, maxValue))
    {
        return false;
    }
    target = value;
    return true;
}



/// Assign the param value to the bool target: the value must be 0 or 1. The
/// target is not changed otherwise.
bool assignFlag(bool& target, float value)
{
    int converted{0};
    if (!assignInt(converted, value, 0, 1))
    {
        return false;
    }
    target = converted != 0;
    return true;
}



/// Set one param of the params object. The object is not changed if the value
/// is not valid for the param.
bool setParamValue(rapidpixel::VStabiliserParams& params,
                   rapidpixel::VStabiliserParam id, float value)
{
    switch (id)
    {
    case rapidpixel::VStabiliserParam::SCALE_FACTOR:
    {
        return assignInt(params.scaleFactor, value,
                         MIN_SCALE_FACTOR, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::X_OFFSET_LIMIT:
    {
        return assignInt(params.xOffsetLimit, value, 0, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::Y_OFFSET_LIMIT:
    {
        return assignInt(params.yOffsetLimit, value, 0, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::A_OFFSET_LIMIT:
    {
        return assignFloat(params.aOffsetLimit, value, 0.0f, FLOAT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::X_FILTER_COEFF:
    {
        return assignFloat(params.xFilterCoeff, value,
                           MIN_FILTER_COEFF, MAX_FILTER_COEFF);
    }
    case rapidpixel::VStabiliserParam::Y_FILTER_COEFF:
    {
        return assignFloat(params.yFilterCoeff, value,
                           MIN_FILTER_COEFF, MAX_FILTER_COEFF);
    }
    case rapidpixel::VStabiliserParam::A_FILTER_COEFF:
    {
        return assignFloat(params.aFilterCoeff, value,
                           MIN_FILTER_COEFF, MAX_FILTER_COEFF);
    }
    case rapidpixel::VStabiliserParam::MODE:
    {
        return assignFlag(params.enable, value);
    }
    case rapidpixel::VStabiliserParam::TRANSPARENT_BORDER:
    {
        return assignFlag(params.transparentBorder, value);
    }
    case rapidpixel::VStabiliserParam::CONST_X_OFFSET:
    {
        return assignInt(params.constXOffset, value,
                         -INT_MAX_VALUE, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::CONST_Y_OFFSET:
    {
        return assignInt(params.constYOffset, value,
                         -INT_MAX_VALUE, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::CONST_A_OFFSET:
    {
        return assignFloat(params.constAOffset, value,
                           -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::INSTANT_X_OFFSET:
    {
        return assignInt(params.instantXOffset, value,
                         -INT_MAX_VALUE, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::INSTANT_Y_OFFSET:
    {
        return assignInt(params.instantYOffset, value,
                         -INT_MAX_VALUE, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::INSTANT_A_OFFSET:
    {
        return assignFloat(params.instantAOffset, value,
                           -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::TYPE:
    {
        return assignInt(params.type, value, 0, MAX_TYPE);
    }
    case rapidpixel::VStabiliserParam::CUT_FREQUENCY_HZ:
    {
        return assignFloat(params.cutFrequencyHz, value,
                           0.0f, FLOAT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::FPS:
    {
        return assignFloat(params.fps, value,
                           FLOAT_MIN_POSITIVE, FLOAT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::PROCESSING_TIME_MKS:
    {
        // Read only parameter.
        return false;
    }
    case rapidpixel::VStabiliserParam::LOG_MODE:
    {
        return assignInt(params.logMod, value, 0, MAX_LOG_MODE);
    }
    case rapidpixel::VStabiliserParam::BACKEND:
    {
        return assignInt(params.backend, value,
                         -INT_MAX_VALUE, INT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::CUSTOM_1:
    {
        return assignFloat(params.custom1, value,
                           -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::CUSTOM_2:
    {
        return assignFloat(params.custom2, value,
                           -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE);
    }
    case rapidpixel::VStabiliserParam::CUSTOM_3:
    {
        return assignFloat(params.custom3, value,
                           -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE);
    }
    default:
    {
        return false;
    }
    }
}



/// Check that all values of the params object are valid. The ranges are the
/// same as in setParamValue(...).
bool isValid(const rapidpixel::VStabiliserParams& params)
{
    return inRange(params.scaleFactor, MIN_SCALE_FACTOR, INT_MAX_VALUE) &&
           inRange(params.xOffsetLimit, 0, INT_MAX_VALUE) &&
           inRange(params.yOffsetLimit, 0, INT_MAX_VALUE) &&
           inRange(params.aOffsetLimit, 0.0f, FLOAT_MAX_VALUE) &&
           inRange(params.xFilterCoeff, MIN_FILTER_COEFF, MAX_FILTER_COEFF) &&
           inRange(params.yFilterCoeff, MIN_FILTER_COEFF, MAX_FILTER_COEFF) &&
           inRange(params.aFilterCoeff, MIN_FILTER_COEFF, MAX_FILTER_COEFF) &&
           inRange(params.constXOffset, -INT_MAX_VALUE, INT_MAX_VALUE) &&
           inRange(params.constYOffset, -INT_MAX_VALUE, INT_MAX_VALUE) &&
           inRange(params.constAOffset, -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE) &&
           inRange(params.instantXOffset, -INT_MAX_VALUE, INT_MAX_VALUE) &&
           inRange(params.instantYOffset, -INT_MAX_VALUE, INT_MAX_VALUE) &&
           inRange(params.instantAOffset, -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE) &&
           inRange(params.type, 0, MAX_TYPE) &&
           inRange(params.cutFrequencyHz, 0.0f, FLOAT_MAX_VALUE) &&
           inRange(params.fps, FLOAT_MIN_POSITIVE, FLOAT_MAX_VALUE) &&
           inRange(params.processingTimeMks, 0, INT_MAX_VALUE) &&
           inRange(params.logMod, 0, MAX_LOG_MODE) &&
           inRange(params.backend, -INT_MAX_VALUE, INT_MAX_VALUE) &&
           inRange(params.custom1, -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE) &&
           inRange(params.custom2, -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE) &&
           inRange(params.custom3, -FLOAT_MAX_VALUE, FLOAT_MAX_VALUE);
}
}



rapidpixel::CustomVStabiliser::CustomVStabiliser() = default;



rapidpixel::CustomVStabiliser::~CustomVStabiliser() = default;



std::string rapidpixel::CustomVStabiliser::getVersion()
{
    return CUSTOM_VSTABILISER_VERSION;
}



bool rapidpixel::CustomVStabiliser::initVStabiliser(
        const rapidpixel::VStabiliserParams& params)
{
    // Check params: they are copied only if all values are valid.
    if (!isValid(params))
    {
        return false;
    }

    // Copy params.
    const std::lock_guard<std::mutex> lock(m_paramsMutex);
    m_params = params;

    return true;
}



bool rapidpixel::CustomVStabiliser::setParam(
        rapidpixel::VStabiliserParam id, float value)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);
    return setParamValue(m_params, id, value);
}



float rapidpixel::CustomVStabiliser::getParam(rapidpixel::VStabiliserParam id)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Check parameter ID.
    switch (id)
    {
    case rapidpixel::VStabiliserParam::SCALE_FACTOR:
    {
        return static_cast<float>(m_params.scaleFactor);
    }
    case rapidpixel::VStabiliserParam::X_OFFSET_LIMIT:
    {
        return static_cast<float>(m_params.xOffsetLimit);
    }
    case rapidpixel::VStabiliserParam::Y_OFFSET_LIMIT:
    {
        return static_cast<float>(m_params.yOffsetLimit);
    }
    case rapidpixel::VStabiliserParam::A_OFFSET_LIMIT:
    {
        return m_params.aOffsetLimit;
    }
    case rapidpixel::VStabiliserParam::X_FILTER_COEFF:
    {
        return m_params.xFilterCoeff;
    }
    case rapidpixel::VStabiliserParam::Y_FILTER_COEFF:
    {
        return m_params.yFilterCoeff;
    }
    case rapidpixel::VStabiliserParam::A_FILTER_COEFF:
    {
        return m_params.aFilterCoeff;
    }
    case rapidpixel::VStabiliserParam::MODE:
    {
        return m_params.enable ? 1.0f : 0.0f;
    }
    case rapidpixel::VStabiliserParam::TRANSPARENT_BORDER:
    {
        return m_params.transparentBorder ? 1.0f : 0.0f;
    }
    case rapidpixel::VStabiliserParam::CONST_X_OFFSET:
    {
        return static_cast<float>(m_params.constXOffset);
    }
    case rapidpixel::VStabiliserParam::CONST_Y_OFFSET:
    {
        return static_cast<float>(m_params.constYOffset);
    }
    case rapidpixel::VStabiliserParam::CONST_A_OFFSET:
    {
        return m_params.constAOffset;
    }
    case rapidpixel::VStabiliserParam::INSTANT_X_OFFSET:
    {
        return static_cast<float>(m_params.instantXOffset);
    }
    case rapidpixel::VStabiliserParam::INSTANT_Y_OFFSET:
    {
        return static_cast<float>(m_params.instantYOffset);
    }
    case rapidpixel::VStabiliserParam::INSTANT_A_OFFSET:
    {
        return m_params.instantAOffset;
    }
    case rapidpixel::VStabiliserParam::TYPE:
    {
        return static_cast<float>(m_params.type);
    }
    case rapidpixel::VStabiliserParam::CUT_FREQUENCY_HZ:
    {
        return m_params.cutFrequencyHz;
    }
    case rapidpixel::VStabiliserParam::FPS:
    {
        return m_params.fps;
    }
    case rapidpixel::VStabiliserParam::PROCESSING_TIME_MKS:
    {
        return static_cast<float>(m_params.processingTimeMks);
    }
    case rapidpixel::VStabiliserParam::LOG_MODE:
    {
        return static_cast<float>(m_params.logMod);
    }
    case rapidpixel::VStabiliserParam::BACKEND:
    {
        return static_cast<float>(m_params.backend);
    }
    case rapidpixel::VStabiliserParam::CUSTOM_1:
    {
        return m_params.custom1;
    }
    case rapidpixel::VStabiliserParam::CUSTOM_2:
    {
        return m_params.custom2;
    }
    case rapidpixel::VStabiliserParam::CUSTOM_3:
    {
        return m_params.custom3;
    }
    default:
    {
        return -1.0f;
    }
    }
}



void rapidpixel::CustomVStabiliser::getParams(
        rapidpixel::VStabiliserParams& params)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);
    params = m_params;
}



bool rapidpixel::CustomVStabiliser::executeCommand(
        rapidpixel::VStabiliserCommand id, float value)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);

    // Check command ID.
    switch (id)
    {
    case rapidpixel::VStabiliserCommand::RESET:
    {
        return true;
    }
    case rapidpixel::VStabiliserCommand::ON:
    {
        // After the command the param MODE must be 1.
        m_params.enable = true;
        return true;
    }
    case rapidpixel::VStabiliserCommand::OFF:
    {
        // After the command the param MODE must be 0.
        m_params.enable = false;
        return true;
    }
    case rapidpixel::VStabiliserCommand::HOLD_MSEC:
    {
        // The argument is the time in milliseconds.
        return inRange(value, 0.0f, FLOAT_MAX_VALUE);
    }
    default:
    {
        return false;
    }
    }
}



bool rapidpixel::CustomVStabiliser::stabilise(
        rapidpixel::Frame& src, rapidpixel::Frame& dst)
{
    // Copy frame.
    dst = src;

    return true;
}



void rapidpixel::CustomVStabiliser::getOffsets(float& dX, float& dY, float& dA)
{
    const std::lock_guard<std::mutex> lock(m_paramsMutex);
    dX = static_cast<float>(m_params.instantXOffset);
    dY = static_cast<float>(m_params.instantYOffset);
    dA = m_params.instantAOffset;
}



bool rapidpixel::CustomVStabiliser::decodeAndExecuteCommand(
        uint8_t* data, int size)
{
    // Decode command. The decoder checks the size, the version and the IDs.
    VStabiliserParam paramId{VStabiliserParam::SCALE_FACTOR};
    VStabiliserCommand commandId{VStabiliserCommand::RESET};
    float value{0.0f};
    const int result =
            VStabiliser::decodeCommand(data, size, paramId, commandId, value);

    // Execute the decoded command.
    if (result == 0)
    {
        return executeCommand(commandId, value);
    }
    if (result == 1)
    {
        return setParam(paramId, value);
    }

    return false;
}
