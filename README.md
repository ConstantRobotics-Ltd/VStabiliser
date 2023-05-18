![frame_logo](_static/vstabiliser_logo.png)

# VStabiliser interface C++ class

**v1.0.0**

------

# Table of contents

- [Overview](#Overview)
- [Versions](#Versions)
- [VStabiliser class description](#VStabiliser-class-description)
  - [VStabiliser class declaration](#VStabiliser-class-declaration)
  - [getVersion method](#getVersion-method)
  - [setParam method](#setParam-method)
  - [getParam method](#getParam-method)
  - [executeCommand method](#executeCommand-method)
  - [stabilise method](#stabilise-method)
  - [getOffsets methods](#getOffsets-methods)

- [Data structures](#Data-structures)
  - [VStabiliserParam enum](#VStabiliserParam-enum)
  - [VStabiliserCommand enum](#VStabiliserCommand-enum)


# Overview

**VStabiliser** C++ library provides interface for different implementation of video stabilisation algorithms. **VStabiliser** library also determines behavior and use principles of different implementation of video stabilisation algorithms. File **VStabiliser.h** includes declaration of interface class **VStabiliser**.

# Versions

**Table 1** - Library versions.

| Version | Release date | What's new     |
| ------- | ------------ | -------------- |
| 1.0.0   | 18.05.2023   | First version. |

# VStabiliser class description

## VStabiliser class declaration

**VStabiliser.h** file contains **VStabiliser** class declaration. Class declaration:

```cpp
namespace cr
{
namespace vstab
{
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

    /**
     * @brief Get offsets: horithontal, vertical and rotation. The method must
     * return current offsets which implemented to last processed video frame.
     * @param dX Horizontal offset.
     * @param dY Vertical offset.
     * @param dA Rotational angle.
     */
    virtual void getOffsets(float& dX, float& dY, float& dA) = 0;
};
}
}
```

## getVersion method

**getVersion()** method return string of current version of **VStabiliser** class. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **Frame** class instance. Example:

```cpp
std::cout << "VStabiliser class version: " << cr::vstab::VStabiliser::getVersion() << std::endl;
```

## setParam method

**setParam(...)** method intended to change video stabiliser parameter. Method declaration:

```cpp
virtual bool setParam(cr::vstab::VStabiliserParam id, float value) = 0;
```

| Parameter | Description                                            |
| --------- | ------------------------------------------------------ |
| id        | Parameter ID according to **VStabiliserParam** enum.   |
| value     | Parameter value **VStabiliserParam** enum description. |

**Returns:** TRUE if param was accepted or FALSE if not.

## getParam method

**getParam(...)** method intended to change video stabiliser parameter. Method declaration:

```cpp
virtual float getParam(cr::vstab::VStabiliserParam id) = 0;
```

| Parameter | Description                                          |
| --------- | ---------------------------------------------------- |
| id        | Parameter ID according to **VStabiliserParam** enum. |

**Returns:** Parameter value or -1 if this param not supported by implementation.

## executeCommand method

**executeCommand(...)** method intended to execute stabiliser command. Method declaration:

```cpp
virtual bool executeCommand(cr::vstab::VStabiliserCommand id) = 0;
```

| Parameter | Description                                          |
| --------- | ---------------------------------------------------- |
| id        | Command ID according to **VStabiliserCommand** enum. |

**Returns:** TRUE if the command executed or FALSE if not.

## stabilise method

**stabilise(...)** method performs video stabilisation. Method declaration:

```cpp
virtual bool stabilise(cr::video::Frame& src, cr::video::Frame& dst) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| src       | Source frame. The methods accepts only RAW frame data (not compressed pixel formats, see description of **Frame** class). Particular implementation can support all, only one or few pixel formats listed in **Frame** class description. |
| dst       | Result frame. The pixel format of the result frame must be the same as source frame. If stabilisation disabled (param **MODE** set to 0) the library must copy data from source frame to result frame. |

**Returns:** TRUE if video frame processed or FALSE in case any errors.

## getOffsets methods

**getOffsets(...)** method returns horizontal offset (pixels), vertical offset (pixels) and rotation angle (degree) implemented to last processed video frame (**stabilise** method). Method declaration:

```cpp
virtual void getOffsets(float& dX, float& dY, float& dA) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| dX        | Reference to output value of horizontal offset (pixels) implemented to last processed video frame. |
| dY        | Reference to output value of vertical offset (pixels) implemented to last processed video frame. |
| dA        | Reference to output value of rotational angle (degree) implemented to last processed video frame. |

# Data structures

## VStabiliserParam enum

**VStabiliserParam** enum lists video stabiliser parameters to set or to obtain. Enum declaration.

```cpp
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
    /// should add this offset to next processed video frame.
    INSTANT_X_OFFSET,
    /// Instant (for one frame) vertical image offset in pixels. The library
    /// should add this offset to next processed video frame.
    INSTANT_Y_OFFSET,
    /// Instant (for one frame) rotational angle in degree. The library
    /// should add this offset to next processed video frame.
    INSTANT_A_OFFSET,
    /// Algorithm type. Default values:
    /// 0 - 2D. Stabilisation only on horizonatal and vertical directions.
    /// 1 - 3D. Stabilisation on horizontal and vertical directions + rotation.
    /// Particular implementation can have unique values.
    TYPE
};
```

**Table 2** - Video stabiliser parameters description.

| Parameter          | Description                                                  |
| ------------------ | ------------------------------------------------------------ |
| SCALE_FACTOR       | Scale factor. If 1 the library will process original frame size, if 2 the library will scale original frane size by 2, if 3 - by 3. |
| X_OFFSET_LIMIT     | Maximum horizontal image shift in pixels per video frame. If image shift bigger than this limit the library should compensate only X_OFFSET_LIMIT shift. |
| Y_OFFSET_LIMIT     | Maximum vertical image shift in pixels per video frame. If image shift bigger than this limit the library should compensate only Y_OFFSET_LIMIT shift. |
| A_OFFSET_LIMIT     | Maximum rotational image angle in degree per video frame. If image absolute rotational angle bigger than this limit the library should compensate only A_OFFSET_LIMIT angle. |
| X_FILTER_COEFF     | Horizontal smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stibilisation algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| Y_FILTER_COEFF     | Vertical smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stibilisation algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| A_FILTER_COEFF     | Rotational smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stibilisation algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| MODE               | Stabilisation mode: 0 - Stabilisation off. The library should just copy input image. 1 - Stabilisation on. |
| TRANSPARENT_BORDER | Transparent border mode:<br/>0 - Not transparent borders (black borders).<br/>1 - Transparent borders (parts of previous images). Particular implementation can have additional modes. |
| CONST_X_OFFSET     | Constant horizontal image offset in pixels. The library should add this offset to each processed video frame. |
| CONST_Y_OFFSET     | Constant vertical image offset in pixels. The library should add this offset to each processed video frame. |
| CONST_A_OFFSET     | Constant rotational angle in degree. The library should add this offset to each processed video frame. |
| INSTANT_X_OFFSET   | Instant (for one frame) horizontal image offset in pixels. The library should add this offset to next processed video frame. |
| INSTANT_Y_OFFSET   | Instant (for one frame) vertical image offset in pixels. The library should add this offset to next processed video frame. |
| INSTANT_A_OFFSET   | Instant (for one frame) rotational angle in degree. The library should add this offset to next processed video frame. |
| TYPE               | Algorithm type. Particular implementation can have unique values. Default values:<br/>0 - 2D. Stabilisation only on horizonatal and vertical directions.<br/>1 - 3D. Stabilisation on horizontal and vertical directions + rotation. |

## VStabiliserCommand enum

**VStabiliserCommand** enum lists video stabiliser commands. Enum declaration.

```cpp
enum class VStabiliserCommand
{
    /// Reset stabilisation algorithm.
    RESET = 1,
    /// Enable stabilisation. After execution parameter MODE must be set to 1.
    ON,
    /// Disable stabilisation. After execution parameter MODE must be set to 0.
    OFF
};
```

**Table 3** - Video stabiliser commands description.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| RESET     | Reset stabilisation algorithm.                               |
| ON        | Enable stabilisation. After execution parameter **MODE** must be set to 1. |
| OFF       | Disable stabilisation. After execution parameter **MODE** must be set to 0. |

