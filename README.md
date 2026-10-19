![vstabiliser_web_logo](./static/vstabiliser_web_logo.png)



# **VStabiliser interface C++ library**

**v2.6.1**



# Table of contents

- [Overview](#overview)
- [Versions](#versions)
- [Library files](#library-files)
- [VStabiliser class description](#vstabiliser-class-description)
  - [VStabiliser class declaration](#vstabiliser-class-declaration)
  - [getVersion method](#getversion-method)
  - [initVStabiliser method](#initvstabiliser-method)
  - [setParam method](#setparam-method)
  - [getParam method](#getparam-method)
  - [getParams method](#getparams-method)
  - [executeCommand method](#executecommand-method)
  - [stabilise method](#stabilise-method)
  - [getOffsets methods](#getoffsets-methods)
  - [encodeSetParamCommand method](#encodesetparamcommand-method)
  - [encodeCommand method](#encodecommand-method)
  - [decodeCommand method](#decodecommand-method)
  - [decodeAndExecuteCommand method](#decodeandexecutecommand-method)
- [Data structures](#data-structures)
  - [VStabiliserParam enum](#vstabiliserparam-enum)
  - [VStabiliserCommand enum](#vstabilisercommand-enum)
- [VStabiliserParams class description](#vstabiliserparams-class-description)
  - [VStabiliserParams class declaration](#vstabiliserparams-class-declaration)
  - [Serialize video stabiliser params](#serialize-video-stabiliser-params)
  - [Deserialize video stabiliser params](#deserialize-video-stabiliser-params)
  - [Read params from JSON file and write to JSON file](#read-params-from-json-file-and-write-to-json-file)
- [Build and connect to your project](#build-and-connect-to-your-project)
  - [Build stand alone](#build-stand-alone)
  - [CMake options](#cmake-options)
  - [Connect to your project](#connect-to-your-project)
- [Portability](#portability)
- [How to make custom implementation](#how-to-make-custom-implementation)



# Overview

**VStabiliser** C++ library provides standard interface as well as defines data structures and rules for different implementation of video stabilization algorithms. **VStabiliser** interface class does nothing, just provides interface, defines data structures and provides methods to encode/decode commands and encode/decode params. Different video stabilizer classes inherit interface from **VStabiliser** C++ class. **VStabiliser.h** file contains list of data structures ([VStabiliserCommand](#vstabilisercommand-enum) enum, [VStabiliserParam](#vstabiliserparam-enum) enum and [VStabiliserParams](#vstabiliserparams-class-description) class) and **VStabiliser** class declaration. [VStabiliserCommand](#vstabilisercommand-enum) enum contains IDs of commands supported by **VStabiliser** class. [VStabiliserParam](#vstabiliserparam-enum) enum contains IDs of params supported by **VStabiliser** class. [VStabiliserParams](#vstabiliserparams-class-description) class contains fields for video stabilizer params values and provides methods to encode/decode and read/write params from JSON file. All video stabilizers should include params and commands listed in **VStabiliser.h** file. **VStabiliser** class depends on two external libraries: [Frame](https://rapidpixel.constantrobotics.com/docs/Service/Frame.html) (describes video frame data structure) and [ConfigReader](https://rapidpixel.constantrobotics.com/docs/Service/ConfigReader.html) (provides methods to read/write JSON config files). The libraries are not part of the repository (see [Build and connect to your project](#build-and-connect-to-your-project) section). It uses C++17 standard.



# Versions

**Table 1** - Library versions.

| Version | Release date | What's new                                                   |
| ------- | ------------ | ------------------------------------------------------------ |
| 1.0.0   | 18.05.2023   | First version.                                               |
| 2.0.0   | 11.07.2023   | - Added VStabiliserParams class to keep library parameters.<br />- Added method to encode/decode commands.<br />- Added test application.<br />- Added license.<br />- Repository made public. |
| 2.0.1   | 12.07.2023   | - Fixed typo in VStabiliserParams<br />- Updated test application. |
| 2.1.0   | 30.07.2023   | - Switched from radians to radians in params.                |
| 2.2.0   | 24.07.2023   | - Updated encode(...) and decode(...) methods of VStabiliserParams.<br />- Added decodeAndExecuteCommand(...) method.<br />- Added example of video stabiliser implementation. |
| 2.3.0   | 26.09.2023   | - Signature of getParams(...) method changed.                |
| 2.3.1   | 13.11.2023   | - Frame class updated.                                       |
| 2.4.1   | 14.12.2023   | - Virtual destructor added.<br />- Frame class updated.      |
| 2.4.2   | 04.01.2024   | - GitHub compliant license added.                            |
| 2.4.3   | 25.03.2024   | - Frame class updated.<br />- ConfigReader class updated.<br />- Documentation updated. |
| 2.4.4   | 25.07.2024   | - CMake structure updated.                                   |
| 2.5.0   | 29.09.2024   | - Add new HOLD_MSEC action command.<br/>- Change signature of encodeCommand(...) method and executeCommand(...) method. |
| 2.6.0   | 16.04.2025   | - Add new parameters.                                        |
| 2.6.1   | 19.10.2026   | - Git submodules removed: the Frame and ConfigReader libraries are expected in the folders next to the library repository (stand alone build) or must be added by the parent project.<br />- License file removed.<br />- Serialization of VStabiliserParams and of the commands rewritten (the encoded data is the same).<br />- decode(...) of VStabiliserParams changes the object only if the whole data is valid.<br />- decodeCommand(...) checks the exact size of the command (11 bytes), the range of the command and param IDs and returns -1 for a SET_PARAM command of a wrong size (it returned 0).<br />- encode(...), decode(...), encodeCommand(...), encodeSetParamCommand(...) and decodeCommand(...) reject nullptr; encode(...) sets size to 0 if params are not encoded.<br />- Protected default, copy and move special member functions added to VStabiliser class.<br />- Example: implements all methods of the interface (initVStabiliser(...) had a different signature), is thread-safe and validates values.<br />- Test application rewritten: deterministic, leaves no files, returns non-zero code on errors.<br />- Compiler warnings and hardening options added to the CMake files.<br />- Documentation updated. |


# Library files

The library is supplied as source code only. The user is provided with a set of files in the form of a CMake project (repository). The repository structure is shown below:

```xml
.gitignore ------------------------- Git ignore file.
CMakeLists.txt --------------------- Main CMake file of the library.
README.md -------------------------- Description of the library.
example ---------------------------- Folder with custom video stabilizer class.
    CMakeLists.txt ----------------- CMake file for custom video stabilizer class.
    CustomVStabiliser.cpp ---------- Source code file of the CustomVStabiliser class.
    CustomVStabiliser.h ------------ Header with CustomVStabiliser class declaration.
    CustomVStabiliserVersion.h ----- Header file which includes CustomVStabiliser version.
    CustomVStabiliserVersion.h.in -- CMake service file to generate version file.
static ----------------------------- Folder with images for the description.
    vstabiliser_web_logo.png ------- Logo of the library.
test ------------------------------- Folder with test application.
    CMakeLists.txt ----------------- CMake file for test application.
    main.cpp ----------------------- Source code file of test application.
src -------------------------------- Folder with source code of the library.
    CMakeLists.txt ----------------- CMake file of the library.
    VStabiliser.cpp ---------------- Source code file of the library.
    VStabiliser.h ------------------ Header file which includes VStabiliser class declaration.
    VStabiliserVersion.h ----------- Header file which includes version of the library.
    VStabiliserVersion.h.in -------- CMake service file to generate version file.
```



# VStabiliser class description



## VStabiliser class declaration

**VStabiliser.h** file contains **VStabiliser** class declaration. Class declaration:

```cpp
namespace cr
{
namespace vstab
{
/// Video stabiliser interface class.
class VStabiliser
{
public:

    /// Class destructor.
    virtual ~VStabiliser();

    /// Get string of current class version.
    static std::string getVersion();

    /// Init all video stabiliser params by params structure.
    virtual bool initVStabiliser(const VStabiliserParams& params) = 0;

    /// Set param.
    virtual bool setParam(VStabiliserParam id, float value) = 0;

    /// Get param.
    virtual float getParam(VStabiliserParam id) = 0;

    /// Get params.
    virtual void getParams(VStabiliserParams& params) = 0;

    /// Execute command.
    virtual bool executeCommand(VStabiliserCommand id, float value = 0.0f) = 0;

    /// Stabilise video frame.
    virtual bool stabilise(cr::video::Frame& src, cr::video::Frame& dst) = 0;

    /// Get offsets: horizontal, vertical and rotation.
    virtual void getOffsets(float& dX, float& dY, float& dA) = 0;

    /// Encode set param command.
    static void encodeSetParamCommand(
            uint8_t* data, int& size, VStabiliserParam id, float value);

    /// Encode command.
    static void encodeCommand(
            uint8_t* data, int& size, VStabiliserCommand id, float value = 0.0f);

    /// Decode command.
    static int decodeCommand(uint8_t* data,
                             int size,
                             VStabiliserParam& paramId,
                             VStabiliserCommand& commandId,
                             float& value);

    /// Decode and execute command.
    virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;

protected:

    /// Default constructor. The class can be used as a base only.
    VStabiliser() = default;

    /// Copy constructor.
    VStabiliser(const VStabiliser& src) = default;

    /// Move constructor.
    VStabiliser(VStabiliser&& src) = default;

    /// Copy assignment operator.
    VStabiliser& operator=(const VStabiliser& src) = default;

    /// Move assignment operator.
    VStabiliser& operator=(VStabiliser&& src) = default;
};
}
}
```



## getVersion method

The **getVersion()** method return string of current version of **VStabiliser** class. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **VStabiliser** class instance. Example:

```cpp
std::cout << "VStabiliser class version: " << cr::vstab::VStabiliser::getVersion() << std::endl;
```

Console output:

```bash
VStabiliser class version: 2.6.1
```



## initVStabiliser method

The **initVStabiliser(...)** method initializes video stabilizer by set of parameters. Method copies all video stabilizer parameter to internal variables. Method declaration:

```cpp
virtual bool initVStabiliser(const VStabiliserParams& params) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| params    | Parameters class (see  [VStabiliserParams](#vstabiliserparams-class-description) class description). |

**Returns:** TRUE if params was accepted or FALSE if not.



## setParam method

The **setParam(...)** method changes video stabilizer parameter. The particular implementation of the video stabilizer must provide thread-safe **setParam(...)** method call. This means that the **setParam(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool setParam(cr::vstab::VStabiliserParam id, float value) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| id        | Parameter ID according to [VStabiliserParam](#vstabiliserparam-enum) enum. |
| value     | Parameter value. Depends on parameter ID. The interface does not check the value: the implementation must validate it. |

**Returns:** TRUE if param was accepted or FALSE if not.



## getParam method

The **getParam(...)** method intended to get video stabilizer parameter value. The particular implementation of the video stabilizer must provide thread-safe **getParam(...)** method call. This means that the **getParam(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual float getParam(cr::vstab::VStabiliserParam id) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| id        | Parameter ID according to [VStabiliserParam](#vstabiliserparam-enum) enum. |

**Returns:** Parameter value or **-1** if this param not supported by implementation.



## getParams method

The **getParams(...)** method returns all video stabilizer parameters. The particular implementation of the video stabilizer must provide thread-safe **getParams(...)** method call. This means that the **getParams(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual void getParams(VStabiliserParams& params) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| params    | [VStabiliserParams](#vstabiliserparams-class-description) class object. |



## executeCommand method

The **executeCommand(...)** method intended to execute video stabilizer action command. The particular implementation of the video stabiliser must provide thread-safe **executeCommand(...)** method call. This means that the **executeCommand(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool executeCommand(VStabiliserCommand id, float value = 0.0f) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| id        | Command ID according to [VStabiliserCommand](#vstabilisercommand-enum) enum. |
| value     | Command argument. Depends on command ID. The interface does not check the value: the implementation must validate it. |

**Returns:** TRUE if the command executed or FALSE if not.



## stabilise method

The **stabilise(...)** method performs video stabilization. Method declaration:

```cpp
virtual bool stabilise(cr::video::Frame& src, cr::video::Frame& dst) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| src       | Source frame. The methods accepts only RAW frame data (not compressed pixel formats, see description of [Frame](https://rapidpixel.constantrobotics.com/docs/Service/Frame.html) class). Particular implementation can support all, only one or few pixel formats listed in [Frame](https://rapidpixel.constantrobotics.com/docs/Service/Frame.html) class description. |
| dst       | Result frame. The pixel format of the result frame must be the same as source frame. If stabilization disabled (param **MODE** set to 0) the library must copy data from source frame to result frame. |

**Returns:** TRUE if video frame processed or FALSE in case any errors.



## getOffsets methods

The **getOffsets(...)** method returns horizontal offset (pixels), vertical offset (pixels) and rotation angle (radians) implemented to last processed video frame (**stabilise** method). The particular implementation of the video stabiliser must provide thread-safe **getOffsets(...)** method call. This means that the **getOffsets(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual void getOffsets(float& dX, float& dY, float& dA) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| dX        | Reference to output value of horizontal offset (pixels) implemented to last processed video frame. |
| dY        | Reference to output value of vertical offset (pixels) implemented to last processed video frame. |
| dA        | Reference to output value of rotational angle (radians) implemented to last processed video frame. |



## encodeSetParamCommand method

The **encodeSetParamCommand(...)** static method encodes command to change any parameters of remote video stabiliser. To control a stabiliser remotely, the developer has to develop his own protocol and according to it encode the command and deliver it over the communication channel. To simplify this, the **VStabiliser** class contains static methods for encoding the control commands. The **VStabiliser** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). **encodeSetParamCommand(...)** designed to encode SET_PARAM command. Method declaration:

```cpp
static void encodeSetParamCommand(uint8_t* data, int& size, VStabiliserParam id, float value);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer for encoded command. Must not be nullptr. Must have size >= 11. |
| size      | Size of encoded data. Will be 11 bytes. If data is nullptr the size is set to 0 and nothing is written. |
| id        | Parameter ID according to [VStabiliserParam](#vstabiliserparam-enum) enum. |
| value     | Parameter value.                                             |

**encodeSetParamCommand(...)** is static and used without **VStabiliser** class instance. This method used on client side (control system). Example:

```cpp
// Buffer for encoded data.
uint8_t data[11];
// Size of encoded data.
int size = 0;
// Encode command.
cr::vstab::VStabiliser::encodeSetParamCommand(
        data, size, cr::vstab::VStabiliserParam::SCALE_FACTOR, 2.0f);
```



## encodeCommand method

The **encodeCommand(...)** static method encodes video stabiliser action command. To control a stabiliser remotely, the developer has to develop his own protocol and according to it encode the command and deliver it over the communication channel. To simplify this, the **VStabiliser** class contains static methods for encoding the control commands. The **VStabiliser** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). **encodeCommand(...)** designed to encode COMMAND command (action command). Method declaration:

```cpp
static void encodeCommand(uint8_t* data, int& size, VStabiliserCommand id, float value = 0.0f);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer for encoded command. Must not be nullptr. Must have size >= 11. |
| size      | Size of encoded data. Will be 11 bytes. If data is nullptr the size is set to 0 and nothing is written. |
| id        | Command ID according to [VStabiliserCommand](#vstabilisercommand-enum) enum. |
| value     | Command argument. Depends on command ID.                     |

**encodeCommand(...)** is static and used without **VStabiliser** class instance. This method used on client side (control system). Encoding example:

```cpp
// Buffer for encoded data.
uint8_t data[11];
// Size of encoded data.
int size = 0;
// Encode command.
cr::vstab::VStabiliser::encodeCommand(
        data, size, cr::vstab::VStabiliserCommand::HOLD_MSEC, 1200.0f);
```



## decodeCommand method

The **decodeCommand(...)** static method designed to decode command on video stabiliser side. To control a stabiliser remotely, the developer has to develop his own protocol and according to it encode the command and deliver it over the communication channel. To simplify this, the **VStabiliser** interface class contains static method to decode input command (commands should be encoded by methods **encodeSetParamsCommand(...)** or **encodeCommand(...)**). The **VStabiliser** class provides two types of commands: a parameter change command (SET_PARAM) and an action command (COMMAND). Method declaration:

```cpp
static int decodeCommand(uint8_t* data, int size, VStabiliserParam& paramId, VStabiliserCommand& commandId, float& value);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to input command. Must not be nullptr.               |
| size      | Size of command. Must be 11 bytes for both types of commands (SET_PARAM and COMMAND). |
| paramId   | Video stabiliser parameter ID according to [VStabiliserParam](#vstabiliserparam-enum) enum. After decoding SET_PARAM command the method will return parameter ID. |
| commandId | Video stabiliser command ID according to [VStabiliserCommand](#vstabilisercommand-enum) enum. After decoding COMMAND the method will return command ID. |
| value     | Video stabiliser parameter value after decoding SET_PARAM command or command argument after decoding COMMAND. The value is not checked by the method. |

**Returns:** **0** - in case decoding COMMAND, **1** - in case decoding SET_PARAM command or **-1** in case errors: data is nullptr, size is not 11 bytes, wrong data type, version of the library which encoded the command differs in major or minor number, or the ID is outside of the range of [VStabiliserCommand](#vstabilisercommand-enum) or [VStabiliserParam](#vstabiliserparam-enum) enum. The output arguments are changed only if the command is decoded.

**decodeCommand(...)** is static and used without **VStabiliser** class instance. Command decoding example:

```cpp
// Buffer for encoded data.
uint8_t data[11];
int size = 0;
cr::vstab::VStabiliser::encodeCommand(
        data, size, cr::vstab::VStabiliserCommand::ON);

// Decode command.
cr::vstab::VStabiliserCommand commandId = cr::vstab::VStabiliserCommand::RESET;
cr::vstab::VStabiliserParam paramId = cr::vstab::VStabiliserParam::SCALE_FACTOR;
float value = 0.0f;
if (cr::vstab::VStabiliser::decodeCommand(
        data, size, paramId, commandId, value) != 0)
{
    std::cout << "Command not decoded" << std::endl;
    return false;
}
```



## decodeAndExecuteCommand method

The **decodeAndExecuteCommand(...)** method decodes and executes command on video stabiliser side. The particular implementation of the video stabiliser must provide thread-safe **decodeAndExecuteCommand(...)** method call. This means that the **decodeAndExecuteCommand(...)** method can be safely called from any thread. Method declaration:

```cpp
virtual bool decodeAndExecuteCommand(uint8_t* data, int size) = 0;
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to input command.                                    |
| size      | Size of command. Must be 11 bytes for both types of commands (SET_PARAM and COMMAND). |

**Returns:** TRUE if command decoded (SET_PARAM or COMMAND) and executed (action command or set param command).



# Data structures



## VStabiliserParam enum

**VStabiliserParam** enum lists video stabiliser parameters to set or to obtain. Enum declaration.

```cpp
namespace cr
{
namespace vstab
{
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
}
}
```

**Table 2** - Video stabiliser parameters description.

| Parameter           | Description                                                  |
| ------------------- | ------------------------------------------------------------ |
| SCALE_FACTOR        | Scale factor. Value depends on implementation. Default: If 1 the library will process original frame size, if 2 the library will scale original frame size by 2, if 3 - by 3. |
| X_OFFSET_LIMIT      | Maximum horizontal image shift in pixels per video frame. If image shift bigger than this limit the library should compensate only X_OFFSET_LIMIT shift. |
| Y_OFFSET_LIMIT      | Maximum vertical image shift in pixels per video frame. If image shift bigger than this limit the library should compensate only Y_OFFSET_LIMIT shift. |
| A_OFFSET_LIMIT      | Maximum rotational image angle in radians per video frame. If image absolute rotational angle bigger than this limit the library should compensate only A_OFFSET_LIMIT angle. |
| X_FILTER_COEFF      | Horizontal smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stabilization algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| Y_FILTER_COEFF      | Vertical smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stabilization algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| A_FILTER_COEFF      | Rotational smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stabilization algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| MODE                | Stabilization mode: 0 - Stabilization off. The library should just copy input image. 1 - Stabilization on. |
| TRANSPARENT_BORDER  | Transparent border mode:<br/>0 - Not transparent borders (black borders).<br/>1 - Transparent borders (parts of previous images). Particular implementation can have additional modes. |
| CONST_X_OFFSET      | Constant horizontal image offset in pixels. The library should add this offset to each processed video frame. |
| CONST_Y_OFFSET      | Constant vertical image offset in pixels. The library should add this offset to each processed video frame. |
| CONST_A_OFFSET      | Constant rotational angle in radians. The library should add this offset to each processed video frame. |
| INSTANT_X_OFFSET    | Instant (for one frame) horizontal image offset in pixels. The library should add this offset to next processed video frame. |
| INSTANT_Y_OFFSET    | Instant (for one frame) vertical image offset in pixels. The library should add this offset to next processed video frame. |
| INSTANT_A_OFFSET    | Instant (for one frame) rotational angle in radians. The library should add this offset to next processed video frame. |
| TYPE                | Algorithm type. Particular implementation can have unique values. Default values:<br/>0 - 2D type 1. Stabilization only on horizontal and vertical directions.<br/>1 - 2D type 2. Stabilization only on horizontal and vertical directions.<br/>2 - 3D. Stabilization on horizontal and vertical directions + rotation. |
| CUT_FREQUENCY_HZ    | Cut frequency, Hz. Stabiliser will block vibrations with frequency > CUT_FREQUENCY_HZ. |
| FPS                 | Frames per second of input video.                            |
| PROCESSING_TIME_MKS | Read only. Processing time, mks. Processing time for last video frame.  |
| LOG_MODE            | Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal, 3 - File and terminal. |
| BACKEND             | Computational backend. Value depends on implementation.     |
| CUSTOM_1            | Custom param 1. Value depends on implementation.             |
| CUSTOM_2            | Custom param 2. Value depends on implementation.             |
| CUSTOM_3            | Custom param 3. Value depends on implementation.             |



## VStabiliserCommand enum

**VStabiliserCommand** enum lists video stabiliser commands. Enum declaration.

```cpp
namespace cr
{
namespace vstab
{
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
}
}
```

**Table 3** - Video stabiliser commands description.

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| RESET     | Reset stabilisation algorithm. No parameters.                |
| ON        | Enable stabilisation. After execution parameter **MODE** must be set to 1. No parameters. |
| OFF       | Disable stabilisation. After execution parameter **MODE** must be set to 0. No parameters. |
| HOLD_MSEC | Hold processing for some time. Parameter - time in milliseconds. During timeout the algorithm should not do stabilization. This command is used to stop stabilization when pan-tilt moving. |



# VStabiliserParams class description

**VStabiliserParams** class used for video stabiliser initialization (**initVStabiliser(...)** method) or to get all actual params (**getParams()** method). Also **VStabiliserParams** provide structure to write/read params from JSON files (**JSON_READABLE** macro) and provide methos to encode and decode params.



## VStabiliserParams class declaration

**VStabiliser.h** file contains **VStabiliserParams** class declaration. Class declaration:

```cpp
namespace cr
{
namespace vstab
{
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
    JSON_READABLE(VStabiliserParams, scaleFactor, xOffsetLimit, yOffsetLimit,
                  aOffsetLimit, xFilterCoeff, yFilterCoeff, aFilterCoeff,
                  enable, transparentBorder, constXOffset, constYOffset,
                  constAOffset, type, cutFrequencyHz, fps, logMod, backend,
                  custom1, custom2, custom3)

    /// Encode params.
    bool encode(uint8_t* data, int bufferSize,
                int& size, VStabiliserParamsMask* mask = nullptr);

    /// Decode params.
    bool decode(uint8_t* data, int dataSize);
};
}
}
```

**Table 4** - VStabiliserParams class fields description is equivalent to **VStabiliserParam** enum description.

| Field              | type  | Description                                                  |
| ------------------ | ----- | ------------------------------------------------------------ |
| scaleFactor        | int   | Scale factor. Value depends on implementation. Default: If 1 the library will process original frame size, if 2 the library will scale original frame size by 2, if 3 - by 3. |
| xOffsetLimit       | int   | Maximum horizontal image shift in pixels per video frame. If image shift bigger than this limit the library should compensate only xOffsetLimit shift. |
| yOffsetLimit       | int   | Maximum vertical image shift in pixels per video frame. If image shift bigger than this limit the library should compensate only yOffsetLimit shift. |
| aOffsetLimit       | float | Maximum rotational image angle in radians per video frame. If image absolute rotational angle bigger than this limit the library should compensate only aOffsetLimit angle. |
| xFilterCoeff       | float | Horizontal smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stabilisation algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| yFilterCoeff       | float | Vertical smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stabilisation algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| aFilterCoeff       | float | Rotational smoothing coefficient of constant camera movement. The range of values depends on the specific implementation of the stabilisation algorithm. Default values [0-1]: 0 - the library will not compensate for constant camera motion, video will not be stabilized, 1 - no smoothing of constant camera motion (the library will compensate for the current picture drift completely without considering constant motion). |
| enable             | bool  | Enable/disable stabilisation.                                |
| transparentBorder  | bool  | Enable/disable transparent borders.                          |
| constXOffset       | int   | Constant horizontal image offset in pixels. The library should add this offset to each processed video frame. |
| constYOffset       | int   | Constant vertical image offset in pixels. The library should add this offset to each processed video frame. |
| constAOffset       | float | Constant rotational angle in radians. The library should add this offset to each processed video frame. |
| instantXOffset     | int   | Instant (for one frame) horizontal image offset in pixels. The library should add this offset to next processed video frame. |
| instantYOffset     | int   | Instant (for one frame) vertical image offset in pixels. The library should add this offset to next processed video frame. |
| instantAOffset     | float | Instant (for one frame) rotational angle in radians. The library should add this offset to next processed video frame. |
| type               | int   | Algorithm type. Default values: 0 - 2D type 1. Stabilization only on horizontal and vertical. 1 - 2D type 2. Stabilization only on horizontal and vertical. 2 - 3D. Stabilization on horizontal and vertical + rotation. Particular implementation can have unique values. |
| cutFrequencyHz     | float | Cut frequency, Hz. Stabiliser will block vibrations with frequency > cutFrequencyHz. |
| fps                | float | Frames per second of input video.                            |
| processingTimeMks  | int   | Read only. Processing time, mks. Processing time for last video frame.  |
| logMod             | int   | Logging mode. Values: 0 - Disable, 1 - Only file, 2 - Only terminal, 3 - File and terminal. |
| backend            | int   | Computational backend. Value depends on implementation.     |
| custom1            | float | Custom param 1. Value depends on implementation.             |
| custom2            | float | Custom param 2. Value depends on implementation.             |
| custom3            | float | Custom param 3. Value depends on implementation.             |

**Note:** *VStabiliserParams class fields listed in Table 4 **must** reflect params set/get by methods setParam(...) and getParam(...).*



## Serialize video stabiliser params

**VStabiliserParams** class provides method **encode(...)** to serialize video stabiliser params (fields of VStabiliserParams class, see Table 4). Serialization of params necessary in case when you need to send params via communication channels. Method provide options to exclude particular parameters from serialization. To do this method inserts binary mask (3 bytes) where each bit represents particular parameter and **decode(...)** method recognizes it. The encoded data starts with the data type, the major and the minor version of the library and the mask. Integer and float values are stored as 4 bytes in the byte order of the host (see [Portability](#portability) section). Method declaration:

```cpp
bool encode(uint8_t* data, int bufferSize, int& size, VStabiliserParamsMask* mask = nullptr);
```

| Parameter  | Value                                                        |
| ---------- | ------------------------------------------------------------ |
| data       | Pointer to data buffer. Must not be nullptr. The buffer must be valid for **bufferSize** bytes. |
| bufferSize | Data buffer size. Must have size >= 96 bytes (the size of the data with all fields) for any mask. |
| size       | Size of encoded data. Set to 0 if params not encoded.        |
| mask       | Parameters mask - pointer to **VStabiliserParamsMask** structure. **VStabiliserParamsMask** (declared in VStabiliser.h file) determines flags for each field (parameter) declared in **VStabiliserParams** class. If the user wants to exclude any parameters from serialization, he can put a pointer to the mask. If the user wants to exclude a particular parameter from serialization, he should set the corresponding flag to false in the VStabiliserParamsMask structure. If the mask is not set (nullptr) all fields are encoded. |

**Returns:** TRUE if params encoded (serialized) or FALSE if not (data is nullptr or bufferSize is less than 96). If FALSE is returned nothing is written to the buffer.

**VStabiliserParamsMask** structure declaration:

```cpp
namespace cr
{
namespace vstab
{
struct VStabiliserParamsMask
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
    bool backend{true};
    bool custom1{true};
    bool custom2{true};
    bool custom3{true};
};
}
}
```

Example without parameters mask:

```cpp
// Encode data.
cr::vstab::VStabiliserParams in;
uint8_t data[1024];
int size = 0;
if (!in.encode(data, 1024, size))
{
    std::cout << "Can't encode data" << std::endl;
    return false;
}
std::cout << "Encoded data size: " << size << " bytes" << std::endl;
```

Example with parameters mask:

```cpp
// Prepare mask. Exclude scaleFactor. Others by default.
cr::vstab::VStabiliserParamsMask mask;
mask.scaleFactor = false;

// Encode data.
cr::vstab::VStabiliserParams in;
uint8_t data[1024];
int size = 0;
if (!in.encode(data, 1024, size, &mask))
{
    std::cout << "Can't encode data" << std::endl;
    return false;
}
std::cout << "Encoded data size: " << size << " bytes" << std::endl;
```



## Deserialize video stabiliser params

**VStabiliserParams** class provides method **decode(...)** to deserialize params (fields of VStabiliserParams class, see Table 4). Deserialization of params necessary in case when you need to receive params via communication channels. Method automatically recognizes which parameters were serialized by **encode(...)** method. Fields which are not included in the data are set to zero (false). The method checks the data type, the major and the minor version of the library and the size of the data before reading each field. Values of the fields are not checked against any range: the video stabiliser implementation must validate them. Bytes after the last field are ignored. Method declaration:

```cpp
bool decode(uint8_t* data, int dataSize);
```

| Parameter | Value                                                        |
| --------- | ------------------------------------------------------------ |
| data      | Pointer to data buffer. Must not be nullptr.                 |
| dataSize  | Size of data in the buffer, bytes.                           |

**Returns:** TRUE if data decoded (deserialized) or FALSE if not (data is nullptr, wrong data type, wrong version, data is too short). If FALSE is returned the object is not changed.

Example:

```cpp
// Encode data.
cr::vstab::VStabiliserParams in;
uint8_t data[1024];
int size = 0;
if (!in.encode(data, 1024, size))
{
    std::cout << "Can't encode data" << std::endl;
    return false;
}
std::cout << "Encoded data size: " << size << " bytes" << std::endl;

// Decode data.
cr::vstab::VStabiliserParams out;
if (!out.decode(data, size))
{
    std::cout << "Can't decode data" << std::endl;
    return false;
}
```



## Read params from JSON file and write to JSON file

**VStabiliser** interface class library depends on **ConfigReader** library which provides method to read params from JSON file and to write params to JSON file. The fields **instantXOffset**, **instantYOffset**, **instantAOffset** and **processingTimeMks** are not written to JSON and not read from JSON. If the document can not be read, or a field is missing or has a wrong type, **get(...)** of **ConfigReader** returns false and the params object is not changed. Example of writing and reading params to JSON file:

```cpp
// Prepare params.
cr::vstab::VStabiliserParams in;
in.scaleFactor = 2;
in.xOffsetLimit = 100;
in.yOffsetLimit = 80;
in.aOffsetLimit = 0.5f;
in.xFilterCoeff = 0.5f;
in.yFilterCoeff = 0.5f;
in.aFilterCoeff = 0.5f;
in.enable = false;
in.fps = 25.0f;

// Write params to file.
cr::utils::ConfigReader inConfig;
if (!inConfig.set(in, "VStabiliserParams") ||
    !inConfig.writeToFile("TestVStabiliserParams.json"))
{
    std::cout << "Can't write params to file" << std::endl;
    return false;
}

// Read params from file.
cr::utils::ConfigReader outConfig;
if (!outConfig.readFromFile("TestVStabiliserParams.json"))
{
    std::cout << "Can't open config file" << std::endl;
    return false;
}

cr::vstab::VStabiliserParams out;
if (!outConfig.get(out, "VStabiliserParams"))
{
    std::cout << "Can't read params from file" << std::endl;
    return false;
}
```

**TestVStabiliserParams.json** will look like:

```json
{
    "VStabiliserParams": {
        "scaleFactor": 2,
        "xOffsetLimit": 100,
        "yOffsetLimit": 80,
        "aOffsetLimit": 0.5,
        "xFilterCoeff": 0.5,
        "yFilterCoeff": 0.5,
        "aFilterCoeff": 0.5,
        "enable": false,
        "transparentBorder": true,
        "constXOffset": 0,
        "constYOffset": 0,
        "constAOffset": 0.0,
        "type": 2,
        "cutFrequencyHz": 2.0,
        "fps": 25.0,
        "logMod": 0,
        "backend": 0,
        "custom1": 0.0,
        "custom2": 0.0,
        "custom3": 0.0
    }
}
```



# Build and connect to your project

## Build stand alone

The repository does not contain sub-repositories. The libraries **VStabiliser** depends on (**Frame** and **ConfigReader**) must be cloned into the same folder as **VStabiliser**. The folder structure must be:

```bash
<work folder>
    VStabiliser
    Frame
    ConfigReader
```

Typical commands to build the library, the test application and the example (use the URLs of the repositories provided to you):

```bash
git clone <VStabiliser repository URL>
git clone <Frame repository URL>
git clone <ConfigReader repository URL>
cd VStabiliser
mkdir build
cd build
cmake ..
make
```

On Windows use `cmake --build . --config Release` instead of `make`. If **VStabiliser** is the top-level CMake project, the main CMakeLists.txt file adds the folders ../Frame and ../ConfigReader (in this order) before the library itself. If a folder does not exist CMake stops with an error.



## CMake options

**Table 5** - CMake options of the repository. In the options below ${PARENT} is the prefix defined by the top-level project (empty if **VStabiliser** is the top-level project).

| Option                              | Default                                           | Description                                                  |
| ----------------------------------- | ------------------------------------------------- | ------------------------------------------------------------ |
| ${PARENT}_VSTABILISER               | ON                                                | Build the library.                                           |
| ${PARENT}_VSTABILISER_TEST          | ON (stand alone), OFF (included in other project) | Build the test application.                                  |
| ${PARENT}_VSTABILISER_EXAMPLE       | ON (stand alone), OFF (included in other project) | Build the example.                                           |
| ${PARENT}_SUBMODULE_CACHE_OVERWRITE | ON                                                | Forced rewriting of the options above in the CMake cache. The top-level project sets it to OFF when it includes **VStabiliser** as a sub-repository. |
| CMAKE_BUILD_TYPE                    | Release (if not specified)                        | Build type.                                                  |



## Connect to your project

If you want connect **VStabiliser** library to your CMake project as source code you can make follow. The library does not add its dependencies when it is included in another project: the project must add **Frame** and **ConfigReader** libraries itself. For example, if your repository has structure:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
```

Create folder **3rdparty** in your repository folder and copy **VStabiliser**, **Frame** and **ConfigReader** repository folders there. Structure of your repository:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
3rdparty
    Frame
    ConfigReader
    VStabiliser
```

Create CMakeLists.txt file in **3rdparty** folder. CMakeLists.txt should contain:

```cmake
cmake_minimum_required(VERSION 3.13)

################################################################################
## 3RD-PARTY
## dependencies for the project
################################################################################
project(3rdparty LANGUAGES CXX)

################################################################################
## SETTINGS
## basic 3rd-party settings before use
################################################################################
# To inherit the top-level architecture when the project is used as a submodule.
SET(PARENT ${PARENT}_YOUR_PROJECT_3RDPARTY)
# Disable self-overwriting of parameters inside included subdirectories.
SET(${PARENT}_SUBMODULE_CACHE_OVERWRITE OFF CACHE BOOL "" FORCE)

################################################################################
## CONFIGURATION
## 3rd-party submodules configuration
################################################################################
SET(${PARENT}_SUBMODULE_FRAME                           ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_FRAME)
    SET(${PARENT}_FRAME                                 ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_FRAME_TEST                            OFF CACHE BOOL "" FORCE)
endif()

SET(${PARENT}_SUBMODULE_CONFIG_READER                   ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_CONFIG_READER)
    SET(${PARENT}_CONFIG_READER                         ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_CONFIG_READER_EXAMPLES                OFF CACHE BOOL "" FORCE)
endif()

SET(${PARENT}_SUBMODULE_VSTABILISER                     ON  CACHE BOOL "" FORCE)
if (${PARENT}_SUBMODULE_VSTABILISER)
    SET(${PARENT}_VSTABILISER                           ON  CACHE BOOL "" FORCE)
    SET(${PARENT}_VSTABILISER_TEST                      OFF CACHE BOOL "" FORCE)
    SET(${PARENT}_VSTABILISER_EXAMPLE                   OFF CACHE BOOL "" FORCE)
endif()

################################################################################
## INCLUDING SUBDIRECTORIES
## Adding subdirectories according to the 3rd-party configuration
################################################################################
if (${PARENT}_SUBMODULE_FRAME)
    add_subdirectory(Frame)
endif()

if (${PARENT}_SUBMODULE_CONFIG_READER)
    add_subdirectory(ConfigReader)
endif()

if (${PARENT}_SUBMODULE_VSTABILISER)
    add_subdirectory(VStabiliser)
endif()
```

File **3rdparty/CMakeLists.txt** adds the dependencies and the folder **VStabiliser** to your project and excludes test applications and examples from compiling (by default test applications and examples are excluded from compiling if **VStabiliser** is included as sub-repository). The order of the folders in the file does not matter for the build, but the dependencies must be added by your project. If they are not added, the build stops with the error `Frame.h: No such file or directory`. Structure of your repository:

```bash
CMakeLists.txt
src
    CMakeList.txt
    yourLib.h
    yourLib.cpp
3rdparty
    CMakeLists.txt
    Frame
    ConfigReader
    VStabiliser
```

Next you need include folder 3rdparty in main **CMakeLists.txt** file of your repository. Add string at the end of your main **CMakeLists.txt**:

```cmake
add_subdirectory(3rdparty)
```

Next you have to include VStabiliser library in your **src/CMakeLists.txt** file:

```cmake
target_link_libraries(${PROJECT_NAME} VStabiliser)
```

Done!



# Portability

The encoded params and commands store **int** and **float** values as 4 bytes in the byte order of the host (the byte order is not converted), so the data can be decoded only on a host with the same byte order. The code checks at compile time that **int** and **float** are 4 bytes and that **float** is IEEE 754 binary32. The library has no CPU specific code. It was built and run (little-endian hosts only) with MSVC 2022 (x86-64), GCC 12 and 13 (x86-64), GCC 13 (AArch64) and Clang 15 and 18 (x86-64); the encoded data was identical on all of them.



# How to make custom implementation

The **VStabiliser** class provides only an interface, data structures, and methods for encoding and decoding commands and params. To create your own implementation of the video stabiliser, you must include the VStabiliser repository in your project (see [Build and connect to your project](#build-and-connect-to-your-project) section). The catalogue **example** (see [Library files](#library-files) section) includes an example of the design of the custom video stabiliser. You must implement all the methods of the VStabiliser interface class. Custom video stabiliser class declaration:

```cpp
class CustomVStabiliser: public VStabiliser
{
public:

    /// Class constructor.
    CustomVStabiliser();

    /// Class destructor.
    ~CustomVStabiliser() override;

    /// Copy and move operations are not available: the class owns a mutex.
    CustomVStabiliser(const CustomVStabiliser&) = delete;
    CustomVStabiliser(CustomVStabiliser&&) = delete;
    CustomVStabiliser& operator= (const CustomVStabiliser&) = delete;
    CustomVStabiliser& operator= (CustomVStabiliser&&) = delete;

    /// Get string of current class version.
    static std::string getVersion();

    /// Init all video stabiliser params by params structure.
    bool initVStabiliser(const VStabiliserParams& params) override;

    /// Set param.
    bool setParam(VStabiliserParam id, float value) override;

    /// Get param.
    float getParam(VStabiliserParam id) override;

    /// Get params.
    void getParams(VStabiliserParams& params) override;

    /// Execute command.
    bool executeCommand(VStabiliserCommand id, float value = 0.0f) override;

    /// Stabilise video frame.
    bool stabilise(cr::video::Frame& src, cr::video::Frame& dst) override;

    /// Get offsets: horizontal, vertical and rotation.
    void getOffsets(float& dX, float& dY, float& dA) override;

    /// Decode and execute command.
    bool decodeAndExecuteCommand(uint8_t* data, int size) override;

private:

    /// Video stabiliser params (default params). Protected by m_paramsMutex.
    VStabiliserParams m_params;
    /// Mutex to protect m_params.
    std::mutex m_paramsMutex;
};
```