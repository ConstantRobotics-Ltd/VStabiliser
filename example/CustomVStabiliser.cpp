#include "CustomVStabiliser.h"
#include "CustomVStabiliserVersion.h"



cr::vstab::CustomVStabiliser::CustomVStabiliser()
{

}



cr::vstab::CustomVStabiliser::~CustomVStabiliser()
{

}



std::string cr::vstab::CustomVStabiliser::getVersion()
{
    return CUSTOM_VSTABILISER_VERSION;
}



bool cr::vstab::CustomVStabiliser::initVStabiliser(cr::vstab::VStabiliserParams& params)
{
    // Copy params.
    m_params = params;

    return true;
}



bool cr::vstab::CustomVStabiliser::setParam(cr::vstab::VStabiliserParam id, float value)
{
    // Check parameter ID.
    switch (id)
    {
    case cr::vstab::VStabiliserParam::SCALE_FACTOR:
    {
        m_params.scaleFactor = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::X_OFFSET_LIMIT:
    {
        m_params.xOffsetLimit = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::Y_OFFSET_LIMIT:
    {
        m_params.yOffsetLimit = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::A_OFFSET_LIMIT:
    {
        m_params.aOffsetLimit = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::X_FILTER_COEFF:
    {
        m_params.xFilterCoeff = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::Y_FILTER_COEFF:
    {
        m_params.yFilterCoeff = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::A_FILTER_COEFF:
    {
        m_params.aFilterCoeff = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::MODE:
    {
        m_params.enable = (int)value == 0 ? false : true;
        return true;
    }
    case cr::vstab::VStabiliserParam::TRANSPARENT_BORDER:
    {
        m_params.transparentBorder = (int)value == 0 ? false : true;
        return true;
    }
    case cr::vstab::VStabiliserParam::CONST_X_OFFSET:
    {
        m_params.constXOffset = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::CONST_Y_OFFSET:
    {
        m_params.constYOffset = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::CONST_A_OFFSET:
    {
        m_params.constAOffset = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::INSTANT_X_OFFSET:
    {
        m_params.instantXOffset = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::INSTANT_Y_OFFSET:
    {
        m_params.instantYOffset = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::INSTANT_A_OFFSET:
    {
        m_params.instantAOffset = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::TYPE:
    {
        m_params.type = (int)value;
        return true;
    }
    case cr::vstab::VStabiliserParam::CUT_FREQUENCY_HZ:
    {
        m_params.cutFrequencyHz = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::FPS:
    {
        m_params.fps = value;
        return true;
    }
    case cr::vstab::VStabiliserParam::PROCESSING_TIME_MKS:
    {
        // Read only parameter.
        return false;
    }
    case cr::vstab::VStabiliserParam::LOG_MODE:
    {
        m_params.logMod = (int)value;
        return true;
    }
    default:
    {
        return false;
    }
    }

    return false;
}



float cr::vstab::CustomVStabiliser::getParam(VStabiliserParam id)
{
    // Check parameter ID.
    switch (id)
    {
    case cr::vstab::VStabiliserParam::SCALE_FACTOR:
    {
        return (float)m_params.scaleFactor;
    }
    case cr::vstab::VStabiliserParam::X_OFFSET_LIMIT:
    {
        return (float)m_params.xOffsetLimit;
    }
    case cr::vstab::VStabiliserParam::Y_OFFSET_LIMIT:
    {
        return (float)m_params.yOffsetLimit;
    }
    case cr::vstab::VStabiliserParam::A_OFFSET_LIMIT:
    {
        return m_params.aOffsetLimit;
    }
    case cr::vstab::VStabiliserParam::X_FILTER_COEFF:
    {
        return m_params.xFilterCoeff;
    }
    case cr::vstab::VStabiliserParam::Y_FILTER_COEFF:
    {
        return m_params.yFilterCoeff;
    }
    case cr::vstab::VStabiliserParam::A_FILTER_COEFF:
    {
        return m_params.aFilterCoeff;
    }
    case cr::vstab::VStabiliserParam::MODE:
    {
        return m_params.enable ? 1.0f : 0.0f;
    }
    case cr::vstab::VStabiliserParam::TRANSPARENT_BORDER:
    {
        return m_params.transparentBorder ? 1.0f : 0.0f;
    }
    case cr::vstab::VStabiliserParam::CONST_X_OFFSET:
    {
        return (float)m_params.constXOffset;
    }
    case cr::vstab::VStabiliserParam::CONST_Y_OFFSET:
    {
        return (float)m_params.constYOffset;
    }
    case cr::vstab::VStabiliserParam::CONST_A_OFFSET:
    {
        return m_params.constAOffset;
    }
    case cr::vstab::VStabiliserParam::INSTANT_X_OFFSET:
    {
        return (float)m_params.instantXOffset;
    }
    case cr::vstab::VStabiliserParam::INSTANT_Y_OFFSET:
    {
        return (float)m_params.instantYOffset;
    }
    case cr::vstab::VStabiliserParam::INSTANT_A_OFFSET:
    {
        return m_params.instantAOffset;
    }
    case cr::vstab::VStabiliserParam::TYPE:
    {
        return (float)m_params.type;
    }
    case cr::vstab::VStabiliserParam::CUT_FREQUENCY_HZ:
    {
        return m_params.cutFrequencyHz;
    }
    case cr::vstab::VStabiliserParam::FPS:
    {
        return m_params.fps;
    }
    case cr::vstab::VStabiliserParam::PROCESSING_TIME_MKS:
    {
        return (float)m_params.processingTimeMks;
    }
    case cr::vstab::VStabiliserParam::LOG_MODE:
    {
        return (float)m_params.logMod;
    }
    default:
    {
        return -1.0f;
    }
    }

    return -1.0f;
}



void cr::vstab::CustomVStabiliser::getParams(cr::vstab::VStabiliserParams& params)
{
    params = m_params;
}



bool cr::vstab::CustomVStabiliser::executeCommand(VStabiliserCommand id)
{
    // Check command ID.
    switch (id)
    {
    case cr::vstab::VStabiliserCommand::RESET:
    {
        return true;
    }
    case cr::vstab::VStabiliserCommand::ON:
    {
        return true;
    }
    case cr::vstab::VStabiliserCommand::OFF:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }

    return false;
}



bool cr::vstab::CustomVStabiliser::stabilise(cr::video::Frame& src, cr::video::Frame& dst)
{
    // Copy frame.
    dst = src;

    return true;
}



void cr::vstab::CustomVStabiliser::getOffsets(float& dX, float& dY, float& dA)
{
    dX = (float)m_params.instantXOffset;
    dY = (float)m_params.instantYOffset;
    dA = m_params.instantAOffset;
}



bool cr::vstab::CustomVStabiliser::decodeAndExecuteCommand(uint8_t* data, int size)
{
    // Init params.
    VStabiliserParam paramId;
    VStabiliserCommand commandId;
    float value = 0.0f;

    // Decode.
    switch (VStabiliser::decodeCommand(data, size, paramId, commandId, value))
    {
    case 0:
    {
        return executeCommand(commandId);
    }
    case 1:
    {
        return setParam(paramId, value);
    }
    default:
    {
        return false;
    }
    }

    return true;
}
