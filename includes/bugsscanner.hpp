#include <stdint.h>

struct ImgBuffer {
  uint8_t* buffer;
  uint64 size;
};

extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer createImgBuffer(uint8_t* buffer, long unsigned int size);

struct Coordinates {
  int64 x;
  int64 y;
};

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Coordinates createCoordinates(int64 x, int64 y);

struct Contour {
  Coordinates topLeft;
  Coordinates bottomLeft;
  Coordinates bottomRight;
  Coordinates topRight;
};

extern "C" __attribute__((visibility("default"))) __attribute__((used))
Contour createContour(
  Coordinates topLeft,
  Coordinates bottomLeft,
  Coordinates bottomRight,
  Coordinates topRight
);

// Return unique file name
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* getFileName(char* ext);
// Warp and save image with original color to file using default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetOriginalImageSaveFile(char* filePath, char* savePath, char* ext);
// Warp and get buffer of image with original color with default contours;
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetOriginalImageBuf(char* filePath);
// Warp and save image as bw color to file using default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetBWImageSaveFile(char* filePath, char* savePath, char* ext);
// Warp and get buffer of image with bw color with default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetBWImageBuf(char* filePath);

// The following methods read image from buffer
// Warp and save image with original color to file using default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetOriginalImageSaveFileInbuf(uchar* buf,  long unsigned int bufSize, char* savePath, char* ext);
// Warp and get buffer of image with original color with default contours;
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetOriginalImageSaveBufInBuf(uchar* buf,  long unsigned int bufSize);
// Warp and save image as bw color to file using default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetBWImageSaveFileInBuf(uchar* buf,  long unsigned int bufSize, char* savePath, char* ext);
// Warp and get buffer of image with bw color with default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetBWImageSaveBufInBuf(uchar* buf,  long unsigned int bufSize);

// Warp and save image with original color to file using custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetOriginalImageSaveFileCustomContour(char* filePath, char* savePath, struct Contour contour, char* ext);
// Warp and get buffer of image with original color with custom contours;
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetOriginalImageBufCustomContour(char* filePath, struct Contour contour);
// Warp and save image as bw color to file using custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetBWImageSaveFileCustomContour(char*filePath, char* savePath, struct Contour contour, char* ext);
// Warp and get buffer of image with bw color with custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetBWImageBufCustomContour(char* filePath, struct Contour contour);

// The following methods read image from buffer
// Warp and save image with original color to file using custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetOriginalImageSaveFileCustomContourInBuf(uchar* buf,  long unsigned int bufSize, char* savePath, struct Contour contour, char* ext);
// Warp and get buffer of image with original color with custom contours;
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetOriginalImageBufCustonContourInBuf(uchar* buf,  long unsigned int bufSize, struct Contour contour);
// Warp and save image as bw color to file using custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetBWImageSaveFileCustomContourInBuf(uchar* buf,  long unsigned int bufSize, char* savePath, struct Contour contour, char* ext);
// Warp and get buffer of image with bw color with custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetBWImageBufCustomContourInBuf(uchar* buf,  long unsigned int bufSize, struct Contour contour);

// Function to calculate contour
// find contour from image path
extern "C" __attribute__((visibility("default"))) __attribute__((used))
Contour findContourFromImagePath(char* src);
// find contour from image buffer
extern "C" __attribute__((visibility("default"))) __attribute__((used))
Contour findContourFromImageBuffer(uchar* buf,  long unsigned int bufSize);
