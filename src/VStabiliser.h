#pragma once
#include <cstdint>
#include <cstring>
#include <memory>
#include <string>
#include "Frame.h"
#include "ConfigReader.h"



namespace rapidpixel
{
/**
 * @brief Video stabilizer params mask. Used to serialize VStabiliserParams.
 *
 * Each flag enables serialization of the VStabiliserParams field with the same
 * name (see VStabiliserParams::encode(...)).
 */
struct VStabiliserParamsMask
{
    bool scaleFactor{true};         ///< Serialize scaleFactor field.
    bool xOffsetLimit{true};        ///< Serialize xOffsetLimit field.
    bool yOffsetLimit{true};        ///< Serialize yOffsetLimit field.
    bool aOffsetLimit{true};        ///< Serialize aOffsetLimit field.
    bool xFilterCoeff{true};        ///< Serialize xFilterCoeff field.
    bool yFilterCoeff{true};        ///< Serialize yFilterCoeff field.
    bool aFilterCoeff{true};        ///< Serialize aFilterCoeff field.
    bool enable{true};              ///< Serialize enable field.
    bool transparentBorder{true};   ///< Serialize transparentBorder field.
    bool constXOffset{true};        ///< Serialize constXOffset field.
    bool constYOffset{true};        ///< Serialize constYOffset field.
    bool constAOffset{true};        ///< Serialize constAOffset field.
    bool instantXOffset{false};     ///< Serialize instantXOffset field.
    bool instantYOffset{false};     ///< Serialize instantYOffset field.
    bool instantAOffset{false};     ///< Serialize instantAOffset field.
    bool type{true};                ///< Serialize type field.
    bool cutFrequencyHz{true};      ///< Serialize cutFrequencyHz field.
    bool fps{true};                 ///< Serialize fps field.
    bool processingTimeMks{true};   ///< Serialize processingTimeMks field.
    bool logMod{true};              ///< Serialize logMod field.
    bool backend{true};             ///< Serialize backend field.
    bool custom1{true};             ///< Serialize custom1 field.
    bool custom2{true};             ///< Serialize custom2 field.
    bool custom3{true};             ///< Serialize custom3 field.
};



/**
 * @brief Video stabilizer params class.
 *
 * @note The class is not thread-safe: an object must not be accessed from
 * several threads at the same time without external synchronization.
 */
class VStabiliserParams
{
public:
    /// Scale factor. Value depends on implementation. Default:
    /// If 1 the library will process original frame size, if 2
    /// the library will scale original frame size by 2, if 3 - by 3.
    int scaleFactor{1};
    /// Maximum horizontal image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only xOffsetLimit
    /// shift.
    int xOffsetLimit{150};
    /// Maximum vertical image shift in pixels per video frame. If image shift
    /// bigger than this limit the library should compensate only yOffsetLimit
    /// shift.
    int yOffsetLimit{150};
    /// Maximum rotational image angle in radians per video frame. If image
    /// absolute rotational angle bigger than this limit the library should
    /// compensate only aOffsetLimit angle.
    float aOffsetLimit{10.0f};
    /// Horizontal smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stabilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    float xFilterCoeff{0.9f};
    /// Vertical smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stabilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    float yFilterCoeff{0.9f};
    /// Rotational smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stabilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    float aFilterCoeff{0.9f};
    /// Enable/disable stabilisation.
    bool enable{true};
    /// Enable/disable transparent borders.
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
    /// 0 - 2D type 1. Stabilisation only on horizontal and vertical.
    /// 1 - 2D type 2. Stabilisation only on horizontal and vertical.
    /// 2 - 3D. Stabilisation on horizontal and vertical + rotation.
    /// Particular implementation can have unique values.
    int type{2};
    /// Cut frequency, Hz. Stabiliser will block vibrations with frequency
    /// > cutFrequencyHz.
    float cutFrequencyHz{2.0f};
    /// Frames per second of input video.
    float fps{30.0f};
    /// Processing time, mks. Processing time for last video frame.
    int processingTimeMks{0};
    /// Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal,
    /// 3 - File and terminal.
    int logMod{0};
    /// Backend. Value depends on implementation.
    int backend{0};
    /// Custom param 1. Value depends on implementation.
    float custom1{0};
    /// Custom param 2. Value depends on implementation.
    float custom2{0};
    /// Custom param 3. Value depends on implementation.
    float custom3{0};

    // The fields instantXOffset, instantYOffset, instantAOffset and
    // processingTimeMks are not written to JSON and not read from JSON.
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
                  logMod,
                  backend,
                  custom1,
                  custom2,
                  custom3)

    /**
     * @brief Encode (serialize) params.
     *
     * The encoded data contains the data type, the major and minor version of
     * the library, a 3 bytes mask of the encoded fields and the values of the
     * fields selected by the mask. The decode(...) method accepts only data
     * encoded by the library of the same major and minor version.
     *
     * @param data Pointer to data buffer. Must not be nullptr. The buffer must
     * be valid for bufferSize bytes.
     * @param bufferSize Data buffer size, bytes. Must be >= 96 (the size of
     * the data with all fields) even if the mask excludes some fields.
     * @param size Size of encoded data, bytes. Set to 0 if params not encoded.
     * @param mask Pointer to params mask to include in data. If nullptr all
     * fields are included in the data.
     * @return TRUE if params encoded (serialized) or FALSE if not (data is
     * nullptr or bufferSize is less than 96). If FALSE is returned nothing is
     * written to the buffer.
     */
    bool encode(uint8_t* data, int bufferSize,
                int& size, VStabiliserParamsMask* mask = nullptr);

    /**
     * @brief Decode (deserialize) params encoded by encode(...) method.
     *
     * The method checks the data type, the version and the size of the data
     * before reading each field. Fields that are not included in the data are
     * set to zero (false). Values of the fields are not checked against any
     * range: the video stabiliser implementation must validate them. Bytes
     * after the last field are ignored.
     *
     * @param data Pointer to encoded data. Must not be nullptr.
     * @param dataSize Size of encoded data, bytes.
     * @return TRUE is params decoded or FALSE if not. If FALSE is returned the
     * object is not changed.
     */
    bool decode(uint8_t* data, int dataSize);
};



/**
 * @brief The video stabiliser params enum.
 *
 * @note CUSTOM_3 is the last value: VStabiliser::decodeCommand(...) uses it to
 * check the range of the param ID. A new value must be added to the end of the
 * enum and the check must be updated.
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
    /// of values depends on the specific implementation of the stabilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    X_FILTER_COEFF,
    /// Vertical smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stabilisation
    /// algorithm. Default values [0-1]: 0 - the library will not compensate for
    /// constant camera motion, video will not be stabilized, 1 - no smoothing
    /// of constant camera motion (the library will compensate for the current
    /// picture drift completely without considering constant motion).
    Y_FILTER_COEFF,
    /// Rotational smoothing coefficient of constant camera movement. The range
    /// of values depends on the specific implementation of the stabilisation
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
    /// 1 - Transparent borders (parts of previous images).
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
    /// 0 - 2D type 1. Stabilisation only on horizontal and vertical.
    /// 1 - 2D type 2. Stabilisation only on horizontal and vertical.
    /// 2 - 3D. Stabilisation on horizontal and vertical + rotation.
    /// Particular implementation can have unique values.
    TYPE,
    /// Cut frequency, Hz. Stabiliser will block vibrations with frequency
    /// > CUT_FREQUENCY_HZ.
    CUT_FREQUENCY_HZ,
    /// Frames per second of input video.
    FPS,
    /// Processing time, mks. Processing time for last video frame.
    PROCESSING_TIME_MKS,
    /// Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal,
    /// 3 - File and terminal.
    LOG_MODE,
    /// Backend. Value depends on implementation.
    BACKEND,
    /// Custom param 1. Value depends on implementation.
    CUSTOM_1,
    /// Custom param 2. Value depends on implementation.
    CUSTOM_2,
    /// Custom param 3. Value depends on implementation. The last value of the
    /// enum.
    CUSTOM_3
};



/**
 * @brief The video stabiliser commands enum.
 *
 * @note HOLD_MSEC is the last value: VStabiliser::decodeCommand(...) uses it
 * to check the range of the command ID. A new value must be added to the end
 * of the enum and the check must be updated.
 */
enum class VStabiliserCommand
{
    /// Reset stabilisation algorithm. No params.
    RESET = 1,
    /// Enable stabilisation. After execution parameter MODE must be set to 1.
    /// No params.
    ON,
    /// Disable stabilisation. After execution parameter MODE must be set to 0.
    /// No params.
    OFF,
    /// Hold processing for some time. Parameter - time in milliseconds. The
    /// last value of the enum.
    HOLD_MSEC
};



/**
 * @brief Video stabiliser interface class.
 */
class VStabiliser
{
public:

    /**
     * @brief Class destructor.
     */
    virtual ~VStabiliser();

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
    virtual bool initVStabiliser(const VStabiliserParams& params) = 0;

    /**
     * @brief Set param.
     * @note The implementation must be thread-safe.
     * @param id Param ID.
     * @param value Param value. The value is not checked by the interface:
     * the implementation must validate it.
     * @return TRUE if param was accepted or FALSE if not.
     */
    virtual bool setParam(VStabiliserParam id, float value) = 0;

    /**
     * @brief Get param.
     * @note The implementation must be thread-safe.
     * @param id Param ID.
     * @return Param value or -1 if this param not supported by implementation.
     */
    virtual float getParam(VStabiliserParam id) = 0;

    /**
     * @brief Get params.
     * @note The implementation must be thread-safe.
     * @param params Params class object.
     */
    virtual void getParams(VStabiliserParams& params) = 0;

    /**
     * @brief Execute command.
     * @note The implementation must be thread-safe.
     * @param id Command ID.
     * @param value Command argument. The value is not checked by the
     * interface: the implementation must validate it.
     * @return TRUE if the command executed or FALSE if not.
     */
    virtual bool executeCommand(VStabiliserCommand id, float value = 0.0f) = 0;

    /**
     * @brief Stabilise video frame.
     * @param src Source video frame.
     * @param dst Result video frame.
     * @return TRUE if video frame processed or FALSE in case any errors.
     */
    virtual bool stabilise(rapidpixel::Frame& src, rapidpixel::Frame& dst) = 0;

    /**
     * @brief Get offsets: horizontal, vertical and rotation. The method must
     * return current offsets which implemented to last processed video frame.
     * @note The implementation must be thread-safe.
     * @param dX Horizontal offset.
     * @param dY Vertical offset.
     * @param dA Rotational angle, radians.
     */
    virtual void getOffsets(float& dX, float& dY, float& dA) = 0;

    /**
     * @brief Encode set param command.
     *
     * The method is thread-safe. It writes exactly 11 bytes.
     *
     * @param data Pointer to data buffer. Must not be nullptr. The buffer must
     * be valid for 11 bytes.
     * @param size Size of encoded data, bytes. Set to 11, or to 0 if data is
     * nullptr (nothing is written in this case).
     * @param id Parameter id.
     * @param value Parameter value.
     */
    static void encodeSetParamCommand(
            uint8_t* data, int& size, VStabiliserParam id, float value);

    /**
     * @brief Encode command.
     *
     * The method is thread-safe. It writes exactly 11 bytes.
     *
     * @param data Pointer to data buffer. Must not be nullptr. The buffer must
     * be valid for 11 bytes.
     * @param size Size of encoded data, bytes. Set to 11, or to 0 if data is
     * nullptr (nothing is written in this case).
     * @param id Command ID.
     * @param value Command argument.
     */
    static void encodeCommand(
            uint8_t* data, int& size, VStabiliserCommand id,
            float value = 0.0f);

    /**
     * @brief Decode command.
     *
     * The method is thread-safe. It checks the pointer, the size (both types
     * of the command have the size of 11 bytes), the data type, the version
     * and the range of the ID. The output arguments are changed only if the
     * command is decoded. The argument value is not checked by the method.
     *
     * @param data Pointer to command data. Must not be nullptr.
     * @param size Size of data, bytes. Must be 11.
     * @param paramId Output param ID (set param command).
     * @param commandId Output command ID (action command).
     * @param value Output param value or command argument.
     * @return 0 - command decoded, 1 - set param command decoded, -1 - error.
     */
    static int decodeCommand(uint8_t* data,
                             int size,
                             VStabiliserParam& paramId,
                             VStabiliserCommand& commandId,
                             float& value);

    /**
     * @brief Decode and execute command.
     * @note The implementation must be thread-safe.
     * @param data Pointer to command data.
     * @param size Size of data, bytes.
     * @return TRUE if command decoded and executed or FALSE if not.
     */
    virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;

protected:

    /**
     * @brief Default constructor. The class can be used as a base only.
     */
    VStabiliser() = default;

    /**
     * @brief Copy constructor.
     * @param src Source object.
     */
    VStabiliser(const VStabiliser& src) = default;

    /**
     * @brief Move constructor.
     * @param src Source object.
     */
    VStabiliser(VStabiliser&& src) = default;

    /**
     * @brief Copy assignment operator.
     * @param src Source object.
     * @return Reference to this object.
     */
    VStabiliser& operator=(const VStabiliser& src) = default;

    /**
     * @brief Move assignment operator.
     * @param src Source object.
     * @return Reference to this object.
     */
    VStabiliser& operator=(VStabiliser&& src) = default;
};
}



// Compatibility: the names of namespace rapidpixel are also available in
// namespace cr::vstab.
namespace cr
{
namespace vstab
{
using namespace ::rapidpixel;
}
}
