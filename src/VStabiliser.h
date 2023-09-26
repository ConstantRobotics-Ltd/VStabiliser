#pragma once
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include "Frame.h"
#include "ConfigReader.h"



namespace cr
{
namespace vstab
{
/**
 * @brief Video stabilizer params mask. Used to serialize VStabiliserParams.
 */
typedef struct VStabiliserParamsMask
{
    bool scaleFactor{true};
    bool xOffsetLimit{true};
    bool yOffsetLimit{true};
    bool aOffsetLimit{true};
    bool xFilterCoeff{true};
    bool yFilterCoeff{true};
    bool aFilterCoeff{true};
    bool enable{true};
    bool transparentBorder{true};
    bool constXOffset{true};
    bool constYOffset{true};
    bool constAOffset{true};
    bool instantXOffset{false};
    bool instantYOffset{false};
    bool instantAOffset{false};
    bool type{true};
    bool cutFrequencyHz{true};
    bool fps{true};
    bool processingTimeMks{true};
    bool logMod{true};
} VStabiliserParamsMask;



/**
 * @brief Video stabilizer params class.
 */
class VStabiliserParams
{
public:
    /// Scale factor. Value depends on implementation. Default:
    /// If 1 the library will process original frame size, if 2
    /// the library will scale original frane size by 2, if 3 - by 3.
    int scaleFactor{1};
    /// Maximum horizontal image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only xOffsetLimit
    /// shift.
    int xOffsetLimit{150};
    /// Maximum vertical image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only yOffsetLimit
    /// shift.
    int yOffsetLimit{150};
    /// Maximum rotational image angle in readians per video frame. If image
    /// absolute rotational angle bigger than this limit the library should
    /// compensate only aOffsetLimit angle.
    float aOffsetLimit{10.0f};
    /// Horizontal smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stibilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    float xFilterCoeff{0.9f};
    /// Vertical smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stibilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    float yFilterCoeff{0.9f};
    /// Rotational smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stibilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    float aFilterCoeff{0.9f};
    /// Enable/disable stabilisation.
    bool enable{true};
    /// Enable/disable trasparent borders.
    bool transparentBorder{true};
    /// Constant horizontal image offset in pixels. The library should add this
    /// offset to each processed video frame.
    int constXOffset{0};
    /// Constant vertical image offset in pixels. The library should add this
    /// offset to each processed video frame.
    int constYOffset{0};
    /// Constant rotational angle in radians. The library should add this
    /// offset to each processed video frame.
    float constAOffset{0.0f};
    /// Instant (for one frame) horizontal image offset in pixels. The library
    /// should add this offset to next processed video frame.
    int instantXOffset{0};
    /// Instant (for one frame) vertical image offset in pixels. The library
    /// should add this offset to next processed video frame.
    int instantYOffset{0};
    /// Instant (for one frame) rotational angle in radians. The library
    /// should add this offset to next processed video frame.
    float instantAOffset{0.0f};
    /// Algorithm type. Default values:
    /// 0 - 2D type 1. Stabilisation only on horizonatal and vertical.
    /// 1 - 2D type 2. Stabilisation only on horizonatal and vertical.
    /// 2 - 3D. Stabilisation on horizontal and vertical + rotation.
    /// Particular implementation can have unique values.
    int type{2};
    /// Cat frequency, Hz. Stabiliser will block vibrations with frequency
    /// > cutFrequencyHz.
    float cutFrequencyHz{2.0f};
    /// Frames per second of input video.
    float fps{30.0f};
    /// Processing time, mks. Processing time for last video frame.
    int processingTimeMks{0};
    /// Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal,
    /// 3 - File and terminal.
    int logMod{0};

    JSON_READABLE(VStabiliserParams,
                  scaleFactor,
                  xOffsetLimit,
                  yOffsetLimit,
                  aOffsetLimit,
                  xFilterCoeff,
                  yFilterCoeff,
                  aFilterCoeff,
                  enable,
                  transparentBorder,
                  constXOffset,
                  constYOffset,
                  constAOffset,
                  type,
                  cutFrequencyHz,
                  fps,
                  logMod);

    /**
     * @brief operator =
     * @param src Source object.
     * @return VStabiliserParams obect.
     */
    VStabiliserParams& operator= (const VStabiliserParams& src);

    /**
     * @brief Encode params.
     * @param data Pointer to data buffer.
     * @param bufferSize Data buffer size.
     * @param size Size of data.
     * @param mask Pointer to params mask to include in data.
     * @return TRUE if params encoded (serialized) or FALSE if not.
     */
    bool encode(uint8_t* data, int bufferSize,
                int& size, VStabiliserParamsMask* mask = nullptr);

    /**
     * @brief Decode params.
     * @param data Pointer to data.
     * @param dataSize Size of data.
     * @return TRUE is params decoded or FALSE if not.
     */
    bool decode(uint8_t* data, int dataSize);
};



/**
 * @brief The video stabiliser params enum.
 */
enum class VStabiliserParam
{
    /// Scale factor. Value depends on implementation. Default:
    /// If 1 the library will process original frame size, if 2
    /// the library will scale original frame size by 2, if 3 - by 3.
    SCALE_FACTOR = 1,
    /// Maximum horizontal image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only X_OFFSET_LIMIT
    /// shift.
    X_OFFSET_LIMIT,
    /// Maximum vertical image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only Y_OFFSET_LIMIT
    /// shift.
    Y_OFFSET_LIMIT,
    /// Maximum rotational image angle in radians per video frame. If image
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
    /// Constant rotational angle in radians. The library should add this
    /// offset to each processed video frame.
    CONST_A_OFFSET,
    /// Instant (for one frame) horizontal image offset in pixels. The library
    /// should add this offset to next processed video frame.
    INSTANT_X_OFFSET,
    /// Instant (for one frame) vertical image offset in pixels. The library
    /// should add this offset to next processed video frame.
    INSTANT_Y_OFFSET,
    /// Instant (for one frame) rotational angle in radians. The library
    /// should add this offset to next processed video frame.
    INSTANT_A_OFFSET,
    /// Algorithm type. Default values:
    /// 0 - 2D type 1. Stabilisation only on horizonatal and vertical.
    /// 1 - 2D type 2. Stabilisation only on horizonatal and vertical.
    /// 2 - 3D. Stabilisation on horizontal and vertical + rotation.
    /// Particular implementation can have unique values.
    TYPE,
    /// Cat frequency, Hz. Stabiliser will block vibrations with frequency
    /// > CUT_FREQUENCY_HZ.
    CUT_FREQUENCY_HZ,
    /// Frames per second of input video.
    FPS,
    /// Processing time, mks. Processing time for last video frame.
    PROCESSING_TIME_MKS,
    /// Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal,
    /// 3 - File and terminal.
    LOG_MODE
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
     * @brief Init all video stabiliser params by params structure.
     * @param params Parameters class.
     * @return TRUE if params was accepted or FALSE if not.
     */
    virtual bool initVStabiliser(VStabiliserParams& params) = 0;

    /**
     * @brief Set param.
     * @param id Param ID.
     * @param value Param value.
     * @return TRUE if param was accepted or FALSE if not.
     */
    virtual bool setParam(VStabiliserParam id, float value) = 0;

    /**
     * @brief Get param.
     * @param id Param ID.
     * @return Param value or -1 if this param not supported by implementation.
     */
    virtual float getParam(VStabiliserParam id) = 0;

    /**
     * @brief Get params.
     * @param params Params class object.
     */
    virtual void getParams(VStabiliserParams& params) = 0;

    /**
     * @brief Execute command.
     * @param id Command ID.
     * @return TRUE if the command executed or FALSE if not.
     */
    virtual bool executeCommand(VStabiliserCommand id) = 0;

    /**
     * @brief Stabilise video frame.
     * @param src Source vidoe frame.
     * @param dst Result video frame.
     * @return TRUE if video frame processed or FALSE in case any errors.
     */
    virtual bool stabilise(cr::video::Frame& src, cr::video::Frame& dst) = 0;

    /**
     * @brief Get offsets: horithontal, vertical and rotation. The method must
     * return current offsets which implemented to last processed video frame.
     * @param dX Horizontal offset.
     * @param dY Vertical offset.
     * @param dA Rotational angle, radians.
     */
    virtual void getOffsets(float& dX, float& dY, float& dA) = 0;

    /**
     * @brief Encode set param command.
     * @param data Pointer to data buffer. Must have size >= 11.
     * @param size Size of encoded data.
     * @param id Parameter id.
     * @param value Parameter value.
     */
    static void encodeSetParamCommand(
            uint8_t* data, int& size, VStabiliserParam id, float value);

    /**
     * @brief Encode command.
     * @param data Pointer to data buffer. Must have size >= 11.
     * @param size Size of encoded data.
     * @param id Command ID.
     */
    static void encodeCommand(
            uint8_t* data, int& size, VStabiliserCommand id);

    /**
     * @brief Decode command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @param paramId Output command ID.
     * @param commandId Output command ID.
     * @param value Param or command value.
     * @return 0 - command decoded, 1 - set param command decoded, -1 - error.
     */
    static int decodeCommand(uint8_t* data,
                             int size,
                             VStabiliserParam& paramId,
                             VStabiliserCommand& commandId,
                             float& value);

    /**
     * @brief Decode and execute command.
     * @param data Pointer to command data.
     * @param size Size of data.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;
};
}
}



