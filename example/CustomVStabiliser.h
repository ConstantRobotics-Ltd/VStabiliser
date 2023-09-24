#pragma once
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include "VStabiliser.h"



namespace cr
{
namespace vstab
{
/// Custom video stabiliser class.
class CustomVStabiliser: public VStabiliser
{
public:

    /// Class constructor.
    CustomVStabiliser();

    /// Class destructor.
    ~CustomVStabiliser();

    /// Get string of current class version.
    std::string getVersion();

    /// Init all video stabiliser params by params structure.
    bool initVStabiliser(VStabiliserParams& params);

    /// Set param.
    bool setParam(VStabiliserParam id, float value);

    /// Get param.
    float getParam(VStabiliserParam id);

    /// Get params.
    VStabiliserParams getParams();

    /// Execute command.
    bool executeCommand(VStabiliserCommand id);

    /// Stabilise video frame.
    bool stabilise(cr::video::Frame& src, cr::video::Frame& dst);

    /// Get offsets: horithontal, vertical and rotation.
    void getOffsets(float& dX, float& dY, float& dA);

    /// Decode and execute command.
    bool decodeAndExecuteCommand(uint8_t* data, int size);

private:

    /// Video stabiliser params (default params).
    VStabiliserParams m_params;
};
}
}



