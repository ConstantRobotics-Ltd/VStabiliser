#pragma once
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include "Frame.h"



namespace cr
{
namespace vstab
{
/**
 * @brief The video stabiliser params enum.
 */
enum class VStabiliserParam
{
    /// Scale factor. If 1 the library will process original frame size, if 2
    /// the library will scale original frane size by 2, if 3 - by 3.
    SCALE_FACTOR = 1,
    /// Maximum horizontal image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only X_OFFSET_LIMIT
    /// shift.
    X_OFFSET_LIMIT,
    /// Maximum vertical image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only Y_OFFSET_LIMIT
    /// shift.
    Y_OFFSET_LIMIT,
    /// Maximum rotational image angle in degree per video frame. If image
    /// absolute rotational angle bigger than this limit the library should
    /// compensate only A_OFFSET_LIMIT angle.
    A_OFFSET_LIMIT,
    /// Horizontal smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stibilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    X_FILTER_COEFF,
    /// Vertical smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stibilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    Y_FILTER_COEFF,
    /// Rotational smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stibilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    A_FILTER_COEFF,
    /// Stabilisation mode:
    /// 0 - Stabilisation off. The library should just copy input image.
    /// 1 - Stabilisation on.
    MODE,
    /// Transparent border mode:
    /// 0 - Not transparent borders (black borders).
    /// 1 - Trasparent borders (parts of previous images).
    /// Particular implementation can have additional modes.
    TRANSPARENT_BORDER,
    /// Constant horizontal image offset in pixels. The library should add this
    /// offset to each processed video frame.
    CONST_X_OFFSET,
    /// Constant vertical image offset in pixels. The library should add this
    /// offset to each processed video frame.
    CONST_Y_OFFSET,
    /// Constant rotational angle in degree. The library should add this
    /// offset to each processed video frame.
    CONST_A_OFFSET,
    /// Instant (for one frame) horizontal image offset in pixels. The library
    /// should add this offset to next processed video frame. After processing
    /// next video frame this will show current offset for video frame.
    INSTANT_X_OFFSET,
    /// Instant (for one frame) vertical image offset in pixels. The library
    /// should add this offset to next processed video frame. After processing
    /// next video frame this will show current offset for video frame.
    INSTANT_Y_OFFSET,
    /// Instant (for one frame) rotational angle in degree. The library
    /// should add this offset to next processed video frame. After processing
    /// next video frame this will show current offset for video frame.
    INSTANT_A_OFFSET,
    /// Algorithm type. Default values:
    /// 0 - 2D. Stabilisation only on horizonatal and vertical directions.
    /// 1 - 3D. Stabilisation on horizontal and vertical directions + rotation.
    /// Particular implementation can have unique values.
    TYPE
};



/**
 * @brief The video stabiliser commands enum.
 */
enum class VStabiliserCommand
{
    /// Reset stabilisation algorithm.
    RESET = 1,
    /// Enable stabilisation. After execution parameter MODE must be set to 1.
    ON,
    /// Disable stabilisation. After execution parameter MODE must be set to 0.
    OFF
};



/**
 * @brief Video stabiliser interface class.
 */
class VStabiliser
{
public:

    /**
     * @brief Get string of current class version.
     * @return String of current class version.
     */
    static std::string getVersion();

    /**
     * @brief Set param.
     * @param id Param ID.
     * @param value Param value.
     * @return TRUE if param was accepted or FALSE if not.
     */
    virtual bool setParam(cr::vstab::VStabiliserParam id, float value) = 0;

    /**
     * @brief Get param.
     * @param id Param ID.
     * @return Param value or -1 if this param not supported by implementation.
     */
    virtual float getParam(cr::vstab::VStabiliserParam id) = 0;

    /**
     * @brief Execute command.
     * @param id Command ID.
     * @return TRUE if the command executed or FALSE if not.
     */
    virtual bool executeCommand(cr::vstab::VStabiliserCommand id) = 0;

    /**
     * @brief Stabilise video frame.
     * @param src Source vidoe frame.
     * @param dst Result video frame.
     * @return TRUE if video frame processed or FALSE in case any errors.
     */
    virtual bool stabilise(cr::video::Frame& src, cr::video::Frame& dst) = 0;
};
}
}



