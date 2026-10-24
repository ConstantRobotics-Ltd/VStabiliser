#pragma once
#include <cstdint>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include "VStabiliser.h"



namespace rapidpixel
{
/**
 * @brief Custom video stabiliser class.
 *
 * @note The class is thread-safe: all methods which use the params can be
 * called from any thread. The copy and move operations are deleted because the
 * class owns a mutex.
 */
class CustomVStabiliser: public VStabiliser
{
public:

    /**
     * @brief Class constructor.
     */
    CustomVStabiliser();

    /**
     * @brief Class destructor.
     */
    ~CustomVStabiliser() override;

    /**
     * @brief Copy constructor is not available: the class owns a mutex.
     */
    CustomVStabiliser(const CustomVStabiliser&) = delete;

    /**
     * @brief Move constructor is not available: the class owns a mutex.
     */
    CustomVStabiliser(CustomVStabiliser&&) = delete;

    /**
     * @brief Copy assignment is not available: the class owns a mutex.
     * @return Reference to this object.
     */
    CustomVStabiliser& operator= (const CustomVStabiliser&) = delete;

    /**
     * @brief Move assignment is not available: the class owns a mutex.
     * @return Reference to this object.
     */
    CustomVStabiliser& operator= (CustomVStabiliser&&) = delete;

    /**
     * @brief Get string of current class version.
     * @return String of current class version.
     */
    static std::string getVersion();

    /**
     * @brief Init all video stabiliser params by params structure.
     * @param params Parameters class. All values must be valid (see
     * setParam(...)).
     * @return TRUE if params was accepted or FALSE if not. If FALSE is
     * returned the params are not changed.
     */
    bool initVStabiliser(const VStabiliserParams& params) override;

    /**
     * @brief Set param.
     * @param id Param ID.
     * @param value Param value. A value that is not valid for the param (NaN,
     * infinity, out of range) is rejected.
     * @return TRUE if param was accepted or FALSE if not (unknown or read only
     * param, not valid value). If FALSE is returned the param is not changed.
     */
    bool setParam(VStabiliserParam id, float value) override;

    /**
     * @brief Get param.
     * @param id Param ID.
     * @return Param value or -1 if the param ID is unknown.
     */
    float getParam(VStabiliserParam id) override;

    /**
     * @brief Get params.
     * @param params Params class object.
     */
    void getParams(VStabiliserParams& params) override;

    /**
     * @brief Execute command.
     * @param id Command ID.
     * @param value Command argument. For HOLD_MSEC it is the time in
     * milliseconds and must not be negative.
     * @return TRUE if the command executed or FALSE if not.
     */
    bool executeCommand(VStabiliserCommand id, float value = 0.0f) override;

    /**
     * @brief Stabilise video frame. The example copies the frame.
     * @param src Source video frame.
     * @param dst Result video frame.
     * @return TRUE if video frame processed or FALSE in case any errors.
     */
    bool stabilise(rapidpixel::Frame& src, rapidpixel::Frame& dst) override;

    /**
     * @brief Get offsets: horizontal, vertical and rotation.
     * @param dX Horizontal offset.
     * @param dY Vertical offset.
     * @param dA Rotational angle, radians.
     */
    void getOffsets(float& dX, float& dY, float& dA) override;

    /**
     * @brief Decode and execute command.
     * @param data Pointer to command data.
     * @param size Size of data, bytes.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    bool decodeAndExecuteCommand(uint8_t* data, int size) override;

private:

    /// Video stabiliser params (default params). Protected by m_paramsMutex.
    VStabiliserParams m_params;
    /// Mutex to protect m_params.
    std::mutex m_paramsMutex;
};
}
