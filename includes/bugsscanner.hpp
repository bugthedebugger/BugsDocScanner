#include <stdio.h>
#include <vector>
#include <iostream>

using namespace std; 

struct ImgBuffer {
  uint8_t* buffer;
  long unsigned int size;
};

extern "C" {
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
  char* getFileName(char* ext);

  // Warp and save image with original color to file using default contours
  char* warpAndGetOriginalImageSaveFile(char* filePath, char* savePath, char* ext);
  // Warp and get buffer of image with original color with default contours;
  ImgBuffer warpAndGetOriginalImageBuf(char* filePath);
  // Warp and save image as bw color to file using default contours
  char* warpAndGetBWImageSaveFile(char* filePath, char* savePath, char* ext);
  // Warp and get buffer of image with bw color with default contours
  ImgBuffer warpAndGetBWImageBuf(char* filePath);

  // The following methods read image from buffer
  // Warp and save image with original color to file using default contours
  char* warpAndGetOriginalImageSaveFileInbuf(ImgBuffer buf, char* savePath, char* ext);
  // Warp and get buffer of image with original color with default contours;
  ImgBuffer warpAndGetOriginalImageSaveBufInBuf(ImgBuffer buf);
  // Warp and save image as bw color to file using default contours
  char* warpAndGetBWImageSaveFileInBuf(ImgBuffer buf, char* savePath, char* ext);
  // Warp and get buffer of image with bw color with default contours
  ImgBuffer warpAndGetBWImageSaveBufInBuf(ImgBuffer buf);

  // Warp and save image with original color to file using custom contours
  char* warpAndGetOriginalImageSaveFileCustomContour(char* filePath, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with original color with custom contours;
  ImgBuffer warpAndGetOriginalImageBufCustomContour(char* filePath, struct Contour contour);
  // Warp and save image as bw color to file using custom contours
  char* warpAndGetBWImageSaveFileCustomContour(char*filePath, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with bw color with custom contours
  ImgBuffer warpAndGetBWImageBufCustomContour(char* filePath, struct Contour contour);

  // The following methods read image from buffer
  // Warp and save image with original color to file using custom contours
  char* warpAndGetOriginalImageSaveFileCustomContourInBuf(ImgBuffer buf, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with original color with custom contours;
  ImgBuffer warpAndGetOriginalImageBufCustonContourInBuf(ImgBuffer buf, struct Contour contour);
  // Warp and save image as bw color to file using custom contours
  char* warpAndGetBWImageSaveFileCustomContourInBuf(ImgBuffer buf, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with bw color with custom contours
  ImgBuffer warpAndGetBWImageBufCustomContourInBuf(ImgBuffer buf, struct Contour contour);

  // Function to calculate contour
  Contour findContourFromImagePath(char* src);
  Contour findContourFromImageBuffer(ImgBuffer buf);
}