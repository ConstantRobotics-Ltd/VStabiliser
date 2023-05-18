![frame_logo](_static/vstabiliser_logo.png)

**v1.0.0**

------

# Table of contents

- [Overview](#Overview)
- [Versions](#Versions)
- [Supported pixel formats](#Supported-pixel-formats)
- [Frame class description](#Frame-class-description)
  - [Frame class declaration](#Frame-class-declaration)
  - [Default constructor](#Default-constructor)
  - [Constructor with parameters](#Constructor-with-parameters)
  - [Copy-constructor](#Copy-constructor)
  - [getVersion method](#getVersion-method)
  - [Copy operator =](#Copy-operator-=)
  - [cloneTo method](#cloneTo-method)
  - [Compare operator ==](#Compare-operator-==)
  - [Compare operator !=](#Compare-operator-!=)
  - [release method](#release-method)
  - [serialize method](#serialize-method)
  - [deserialize method](#deserialize-method)
  - [Frame class public members](#Frame-class-public-members)
  

# Overview

Frame class is basic class for other projects. Main file **Frame.h** contains declaration of **Frame** class and **Fourcc** enum which describes pixel formats supported by **Frame** class.

# Versions

**Table 1** - Library versions.

| Version | Release date | What's new                                                   |
| ------- | ------------ | ------------------------------------------------------------ |
| 1.0.0   | 08.09.2020   | First Frame class version in VideoDataStructures repository. |
| 2.0.0   | 19.11.2020   | Trasformation matrix added.                                  |
| 3.0.0   | 20.12.2020   | Added NV12 pixel format.                                     |
| 4.0.0   | 20.01.2023   | - Interface changed. <br />- Added new pixel formats.<br />- Added shared_ptr for frame data.<br />- Added new methods for copy and compare data.<br />- Added new method for sereliazation and deserialization frame data.<br />- Tests changed. |
| 5.0.0   | 19.03.2023   | - shared_ptr replaced by normal pointer.<br />- Documentation updated. |

# Supported pixel formats

**Frame.h** file contains Fourcc enum which defines supported pixel formats. **Fourcc** enum declaration:

```cpp
namespace cr
{
namespace video
{

/// Macro to make FOURCC code.
#define MAKE_FOURCC_CODE(a,b,c,d) ((uint32_t)(((d)<<24)|((c)<<16)|((b)<<8)|(a)))

/**
 * @brief FOURCC codes enum.
 */
enum class Fourcc
{
    /// RGB 24bit pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-rgb.html#v4l2-pix-fmt-rgb24
    RGB24 = MAKE_FOURCC_CODE('R', 'G', 'B', '3'),
    /// BGR 24bit pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-rgb.html#v4l2-pix-fmt-bgr24
    BGR24 = MAKE_FOURCC_CODE('B', 'G', 'R', '3'),
    /// YUYV 16bits per pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-packed-yuv.html#v4l2-pix-fmt-yuyv
    YUYV  = MAKE_FOURCC_CODE('Y', 'U', 'Y', 'V'),
    /// UYVY 16bits per pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-packed-yuv.html#v4l2-pix-fmt-vyuy
    UYVY  = MAKE_FOURCC_CODE('U', 'Y', 'V', 'Y'),
    /// Grayscale 8bit.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-luma.html#v4l2-pix-fmt-grey
    GRAY  = MAKE_FOURCC_CODE('G', 'R', 'A', 'Y'),
    /// YUV 24bit per pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-packed-yuv.html#v4l2-pix-fmt-yuv24
    YUV24  = MAKE_FOURCC_CODE('Y', 'U', 'V', '3'),
    /// NV12 pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-nv12
    NV12  = MAKE_FOURCC_CODE('N', 'V', '1', '2'),
    /// NV21 pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-nv21
    NV21  = MAKE_FOURCC_CODE('N', 'V', '2', '1'),
    /// YU12 (YUV420) - Planar pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-yuv420
    YU12 = MAKE_FOURCC_CODE('Y', 'U', '1', '2'),
    /// YV12 (YVU420) - Planar pixel format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-yuv-planar.html#v4l2-pix-fmt-yuv420
    YV12 = MAKE_FOURCC_CODE('Y', 'V', '1', '2'),
    /// JPEG compressed format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-compressed.html#v4l2-pix-fmt-jpeg
    JPEG  = MAKE_FOURCC_CODE('J', 'P', 'E', 'G'),
    /// H264 compressed format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-compressed.html#v4l2-pix-fmt-h264
    H264  = MAKE_FOURCC_CODE('H', '2', '6', '4'),
    /// HEVC compressed format.
    /// https://docs.kernel.org/userspace-api/media/v4l/pixfmt-compressed.html#v4l2-pix-fmt-hevc
    HEVC  = MAKE_FOURCC_CODE('H', 'E', 'V', 'C')
};
}
}
```

**Table 2** - Bytes layout of suported pixel formats. Example of 4x4 pixels image.

| ![rgb](_static/rgb_pixel_format.png)RGB24      | ![bgr](_static/bgr_pixel_format.png)BGR24      |
| ---------------------------------------------- | ---------------------------------------------- |
| ![yuv](_static/yuv3_pixel_format.png)**YUV24** | ![gray](_static/gray_pixel_format.png)**GRAY** |
| ![yuyv](_static/yuyv_pixel_format.png)**YUYV** | ![uyvy](_static/uyvy_pixel_format.png)**UYVY** |
| ![nv12](_static/nv12_pixel_format.png)**NV12** | ![nv21](_static/nv21_pixel_format.png)**NV21** |
| ![yu12](_static/yu12_pixel_format.png)**YU12** | ![yv12](_static/yv12_pixel_format.png)**YV12** |

# Frame class description

## Frame class declaration

**Frame.h** file contains **Frame** class declaration. Frame class declaration:

```cpp
namespace cr
{
namespace video
{
/**
 * @brief Video frame class.
 */
class Frame
{
public:

    /**
     * @brief Get string of current class version.
     * @return String of current class version.
     */
    static std::string getVersion();

    /**
     * @brief Default class constructor.
     */
    Frame();

    /**
     * @brief Class constructor with parameters.
     * @param width Frame width (pixels).
     * @param height Frame height (pixels).
     * @param fourcc FOURCC code of data format.
     * @param size Frame data size (bytes).
     * @param data Pointer to data buffer.
     */
    Frame(uint32_t width,
          uint32_t height,
          Fourcc fourcc,
          uint32_t size = 0,
          uint8_t* data = nullptr);

    /**
     * @brief Copy class constructor.
     * @param src Source class object.
     */
    Frame(Frame& src);

    /**
     * @brief Class destructor.
     */
    ~Frame();

    /**
     * @brief Operator "=". Operator makes full copy of data.
     * @param src Source frame object.
     */
    Frame& operator= (const Frame& src);

    /**
     * @brief Operator "!=". Operator to compare two frame objects.
     * @param src Source frame object.
     * @return TRUE if the frames are not identical or FALSE.
     */
    bool operator!= (Frame& src);

    /**
     * @brief Operator "==". Operator to compare two frame objects.
     * @param src Source frame object.
     * @return TRUE if the frames are identical or FALSE.
     */
    bool operator== (Frame& src);

    /**
     * @brief Clone data. Method copies frame and copy just pointer to data.
     * @param dst Output frame.
     */
    void cloneTo(Frame& dst);

    /**
     * @brief Free frame memory.
     */
    void release();

    /**
     * @brief Serialize frame data. The method will encode data with params.
     * @param data Pointer to data buffer.
     *             Buffer size mus be >= frame data size + 26.
     * @param size Size of serialized data.
     */
    void serialize(uint8_t* data, int& size);

    /**
     * @brief Deserialize data data to frame object.
     * @param data Pointer to serialized data.
     * @param size Size of serialized data.
     * @return TRUE if the data deserialized or FALSE.
     */
    bool deserialize(uint8_t* data, int size);

    /// Frame width (pixels).
    uint32_t width{0};
    /// Frame height (pixels).
    uint32_t height{0};
    /// FOURCC code of data format.
    Fourcc fourcc{Fourcc::YUV24};
    /// Frame data size (bytes).
    uint32_t size{0};
    /// ID of frame.
    uint32_t frameId{0};
    /// ID of video source.
    uint32_t sourceId{0};
    /// Pointer to frame data.
    uint8_t* data{nullptr};
};
}
}
```

## Default constructor

Default Frame class constructor doesn't do anything. It doesn't allocate memory. Constructor declaration:

```cpp
Frame();
```

## Constructor with parameters

Constructor with parameters allocates memory and initilises Frame attributes (size, pixesl format etc.). By default allocated memory filled by 0. Constructor declaration:

```cpp
Frame(uint32_t width, uint32_t height, Fourcc fourcc, uint32_t size = 0, uint8_t* data = nullptr);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| width     | Frame width. Must be > 0.                                    |
| height    | Frame height. Must be > 0.                                   |
| fourcc    | Pixel format according to **Fourcc** enum declared in Frame.h file. |
| size      | Optional parameter. Size of external frame data. If user wants to \initialize frame data from another buffer it can be don by initializing parameter **size** and **data**. |
| data      | Optional parameter. Pointer to external frame data to be copied. |

Example of frame initialization:

```cpp
// Frame filled by 0.
cr::video::Frame image1(640, 480, cr::video::Fourcc::RGB24);

// Copy.
cr::video::Frame image2 = cr::video::Frame(640, 480, cr::video::Fourcc::YUV24);

// Init by external data. Data will be copied.
cr::video::Frame image3(640, 480, cr::video::Fourcc::BGR24, 921600, externalDataBuffer);

// Init frame atributes manually.
Frame image4;
image4.data = externalDataBuffer;
image4.width = 640;
image4.height = 480;
image4.size = image4.width * image4.height * 3;
image4.fourcc = cr::video::Fourcc::BGR24;

// Dynamic allocation.
cr::video::Frame* image5 = new cr::video::Frame(640, 480, cr::video::Fourcc::YUV24);
```

## Copy-constructor

Copy constructor copy frame data from other Frame class instance. Constructor declaration:

```cpp
Frame(Frame& src);
```

Example of frame initialization:

```cpp
// Frame filled by 0.
cr::video::Frame image1(640, 480, cr::video::Fourcc::RGB24);

// Copy-constructor.
cr::video::Frame image2(image1);

// Copy-constructor.
cr::video::Frame image3 = cr::video::Frame(image1);
```

## getVersion method

**getVersion()** method return string of current version of **Frame** class. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **Frame** class instance. Example:

```cpp
std::cout << "Frame class version: " << cr::video::Frame::getVersion() << std::endl;
```

## Copy operator =

Copy operator **"="** intended to full copy of frame data. Operator copies frame data and frame attributes. Operator declaration:

```cpp
Frame& operator= (const Frame& src);
```

Example:

```cpp
// Frame filled by 0.
cr::video::Frame image1(640, 480, cr::video::Fourcc::RGB24);

// Make copy.
cr::video::Frame image2 = image1;
```

## cloneTo method

**cloneTo(...)** method designed to clone frame object without copy of data. Method copies frame attributes and initialize pointer to frame data without copy of data. Method declaration:

```cpp
void cloneTo(Frame& dst);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| dst       | Frame object for initialization. Method initialize only frame attributes and copies pointer to frame data. |

Example:

```cpp
// Frame filled by 0.
cr::video::Frame image1(640, 480, cr::video::Fourcc::RGB24);

// Clone.
cr::video::Frame image2;
image1.cloneTo(image2);
```

## Compare operator ==

Compare operator **"=="** compares data attributes and frame data of to Frame objects. Operator declaration:

```cpp
bool operator== (Frame& src);
```

**Returns:** TRUE if all attributes and data of two Frame objects are identical (or when comparing an object to itself) or FALSE if not.

Example:

```cpp
// Frame filled by 0.
cr::video::Frame image1(640, 480, cr::video::Fourcc::RGB24);
cr::video::Frame image2(640, 480, cr::video::Fourcc::RGB24);

// Compare.
if (image1 == image2)
    std::cout << "Identical" << std::endl;

// Change data.
image1.data[0] = rand() % 255;

// Compare.
if (image1 == image2)
    std::cout << "Identical" << std::endl;
else
    std::cout << "Not identical" << std::endl;
```

## Compare operator !=

Compare operator **"!="** compares data attributes and frame data of to Frame objects. Operator declaration:

```cpp
bool operator!= (Frame& src);
```

**Returns:** TRUE if all attributes and data of two Frame objects are identical (or when comparing an object to itself) or FALSE if not.

Example:

```cpp
// Frame filled by 0.
cr::video::Frame image1(640, 480, cr::video::Fourcc::RGB24);
cr::video::Frame image2(640, 480, cr::video::Fourcc::RGB24);

// Compare.
if (image1 != image2)
    std::cout << "Not identical" << std::endl;
else
    std::cout << "Indentical"

// Change data.
image1.data[0] = rand() % 255;

// Compare.
if (image1 != image2)
    std::cout << "Not identical" << std::endl;
else
    std::cout << "Indentical" << std::endl;
```

## release method

**release()** method intended to release allocated memory and reset frame attributes. Method declaration:

```cpp
void release();
```

Example:

```cpp
// Create Frame object with memory allocation.
cr::video::Frame image1(640, 480, cr::video::Fourcc::RGB24);

// Release Frame object.
image1.release();
```

## serialize method

**serialize(...)** method intended for serialization of Frame object with data. Sometimes the user needs to serialize an object in order to transfer or write it somewhere. Method declaration:

```cpp
void serialize(uint8_t* data, int& size);
```

| Parameter | Description              |
| --------- | ------------------------ |
| data      | Pointer to data buffer.  |
| size      | Size of serialized data. |

Example:

```cpp
// Init frames.
Frame srcFrame(640, 480, Fourcc::BGR24);

// Fill source frame.
for (uint32_t i = 0; i < srcFrame.size; ++i)
     srcFrame.data[i] = (uint8_t)(rand() % 255);

// Serialize data.
uint8_t* data = new uint8_t[1920 * 1080 * 4];
int size = 0;
srcFrame.serialize(data, size);
```

## deserialize method

**deserialize(...)** method intended for deserialization of Frame object. Method declaration:

```cpp
bool deserialize(uint8_t* data, int size);
```

| Parameter | Description              |
| --------- | ------------------------ |
| data      | Pointer to data buffer.  |
| size      | Size of serialized data. |

**Returns:** TRUE of the data deserialized or FALSE if not.

Example:

```cpp
// Init frames.
Frame srcFrame(640, 480, Fourcc::BGR24);
Frame dstFrame(1280, 720, Fourcc::YUV24);

// Fill source frame.
for (uint32_t i = 0; i < srcFrame.size; ++i)
     srcFrame.data[i] = (uint8_t)(rand() % 255);

// Serialize data.
uint8_t* data = new uint8_t[1920 * 1080 * 4];
int size = 0;
srcFrame.serialize(data, size);

// Deserialize data.
if (!dstFrame.deserialize(data, size))
{
    std::cout << "Data not deserialized" << std::endl;
    return false;
}

// Compare atributes.
if (srcFrame.size != dstFrame.size)
{
    std::cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << std::endl;
    return false;
}
if (srcFrame.width != dstFrame.width)
{
    std::cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << std::endl;
    return false;
}
if (srcFrame.height != dstFrame.height)
{
    std::cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << std::endl;
    return false;
}
if (srcFrame.fourcc != dstFrame.fourcc)
{
    std::cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << std::endl;
    return false;
}
if (srcFrame.sourceId != dstFrame.sourceId)
{
    cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << std::endl;
    return false;
}
if (srcFrame.frameId != dstFrame.frameId)
{
    std::cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << std::endl;
    return false;
}

// Compare frame data.
for (uint32_t i = 0; i < srcFrame.size; ++i)
{
    if (srcFrame.data[i] != dstFrame.data[i])
    {
        std::cout << "[" << __LINE__ << "] " << __FILE__ << " : ERROR" << std::endl;
        return false;
    }
}
```

## Frame class public members

Frame class public members declaration:

```cpp
/// Frame width (pixels).
uint32_t width{0};
/// Frame height (pixels).
uint32_t height{0};
/// FOURCC code of data format.
Fourcc fourcc{Fourcc::YUV24};
/// Frame data size (bytes).
uint32_t size{0};
/// ID of frame.
uint32_t frameId{0};
/// ID of video source.
uint32_t sourceId{0};
/// Pointer to frame data.
uint8_t* data{nullptr};
```

**Table 3** - Frame class public members.

| Field    | Description                                                  |
| -------- | ------------------------------------------------------------ |
| width    | Frame width.                                                 |
| height   | Frame height.                                                |
| fourcc   | Fourcc code according to **Fourcc** enum declared in **Frame.h** file. |
| size     | Size of frame data.                                          |
| frameId  | Frame ID. User defines this filed.                           |
| sourceId | Source ID. User defines this field.                          |
| data     | Pointer to frame data.                                       |

