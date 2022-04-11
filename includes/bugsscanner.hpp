#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std; 


struct ImgBuffer {
  uint8_t* buffer;
  long unsigned int size;
};

// Function to calculate biggest contour
vector<vector<cv::Point>> biggestContour(std::vector<std::vector<cv::Point>> contours);
// Function to get the contour
void getContour(vector<vector<cv::Point>> biggestContourPoints, cv::Point2f (&dst)[4]);
// Function to get origin of image
void getOrigin(cv::InputArray img, cv::Point2f (&dst)[4]);
// Function to detect edge with bw and threshold
void edgeDetectionFilter1(cv::InputArray src, cv::OutputArray dst);
// Function to detect edge with gaussian and canny
void edgeDetectionFilter2(cv::InputArray src, cv::OutputArray dst);
// Warp image using default contour
void warpImage(cv::Mat src, cv::Mat dst);
// Warp image using custom contour
void warpImageCustomContour(cv::Mat src, cv::Mat dst, struct Contour contour);
// uint8_t list to vector<uint8_t>
vector<uint8_t> uint8_t_list_to_vector(ImgBuffer buf);

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
  vector<uint8_t> warpAndGetOriginalImageBuf(char* filePath);
  // Warp and save image as bw color to file using default contours
  char* warpAndGetBWImageSaveFile(char* filePath, char* savePath, char* ext);
  // Warp and get buffer of image with bw color with default contours
  vector<uint8_t> warpAndGetBWImageBuf(char* filePath);

  // The following methods read image from buffer
  // Warp and save image with original color to file using default contours
  char* warpAndGetOriginalImageSaveFileInbuf(ImgBuffer buf, char* savePath, char* ext);
  // Warp and get buffer of image with original color with default contours;
  vector<uint8_t> warpAndGetOriginalImageSaveBufInBuf(vector<uint8_t> buf);
  // Warp and save image as bw color to file using default contours
  char* warpAndGetBWImageSaveFileInBuf(vector<uint8_t> buf, char* savePath, char* ext);
  // Warp and get buffer of image with bw color with default contours
  vector<uint8_t> warpAndGetBWImageSaveBufInBuf(vector<uint8_t> buf);

  // Warp and save image with original color to file using custom contours
  char* warpAndGetOriginalImageSaveFileCustomContour(char* filePath, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with original color with custom contours;
  vector<uint8_t> warpAndGetOriginalImageBufCustomContour(char* filePath, struct Contour contour);
  // Warp and save image as bw color to file using custom contours
  char* warpAndGetBWImageSaveFileCustomContour(char*filePath, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with bw color with custom contours
  vector<uint8_t> warpAndGetBWImageBufCustomContour(char* filePath, struct Contour contour);

  // The following methods read image from buffer
  // Warp and save image with original color to file using custom contours
  char* warpAndGetOriginalImageSaveFileCustomContourInBuf(vector<uint8_t> buf, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with original color with custom contours;
  vector<uint8_t> warpAndGetOriginalImageBufCustonContourInBuf(vector<uint8_t> buf, struct Contour contour);
  // Warp and save image as bw color to file using custom contours
  char* warpAndGetBWImageSaveFileCustomContourInBuf(vector<uint8_t> buf, char* savePath, struct Contour contour, char* ext);
  // Warp and get buffer of image with bw color with custom contours
  vector<uint8_t> warpAndGetBWImageBufCustomContourInBuf(vector<uint8_t> buf, struct Contour contour);

  // Function to calculate contour
  Contour findContourFromImagePath(char* src);
  Contour findContourFromImageBuffer(vector<uint8_t> buf);
}