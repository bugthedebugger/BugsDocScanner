#include <stdint.h>

struct ImgBuffer {
  uint8_t* buffer;
  long unsigned int size;
};

struct Coordinates {
  float x;
  float y;
};

struct Contour {
  Coordinates topLeft;
  Coordinates bottomLeft;
  Coordinates bottomRight;
  Coordinates topRight;
};

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
char* warpAndGetOriginalImageSaveFileInbuf(ImgBuffer buf, char* savePath, char* ext);
// Warp and get buffer of image with original color with default contours;
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetOriginalImageSaveBufInBuf(ImgBuffer buf);
// Warp and save image as bw color to file using default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetBWImageSaveFileInBuf(ImgBuffer buf, char* savePath, char* ext);
// Warp and get buffer of image with bw color with default contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetBWImageSaveBufInBuf(ImgBuffer buf);

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
char* warpAndGetOriginalImageSaveFileCustomContourInBuf(ImgBuffer buf, char* savePath, struct Contour contour, char* ext);
// Warp and get buffer of image with original color with custom contours;
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetOriginalImageBufCustonContourInBuf(ImgBuffer buf, struct Contour contour);
// Warp and save image as bw color to file using custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
char* warpAndGetBWImageSaveFileCustomContourInBuf(ImgBuffer buf, char* savePath, struct Contour contour, char* ext);
// Warp and get buffer of image with bw color with custom contours
extern "C" __attribute__((visibility("default"))) __attribute__((used))
ImgBuffer warpAndGetBWImageBufCustomContourInBuf(ImgBuffer buf, struct Contour contour);

// Function to calculate contour
// find contour from image path
extern "C" __attribute__((visibility("default"))) __attribute__((used))
Contour findContourFromImagePath(char* src);
// find contour from image buffer
extern "C" __attribute__((visibility("default"))) __attribute__((used))
Contour findContourFromImageBuffer(ImgBuffer buf);
