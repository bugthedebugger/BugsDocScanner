#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std; 

extern "C" {
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
  // Return unique file name
  string getFileName(string ext);
  // Warp and save image with original color to file using default contours
  string warpAndGetOriginalImageSaveFile(string filePath, string savePath, string ext);
  // Warp and get buffer of image with original color with default contours;
  vector<uint8_t> warpAndGetOriginalImageBuf(string filePath);
  // Warp and save image as bw color to file using default contours
  string warpAndGetBWImageSaveFile(string filePath, string savePath, string ext);
  // Warp and get buffer of image with bw color with default contours
  vector<uint8_t> warpAndGetBWImageBuf(string filePath);

  // The following methods read image from buffer
  // Warp and save image with original color to file using default contours
  string warpAndGetOriginalImageSaveFileInbuf(vector<uint8_t> buf, string savePath, string ext);
  // Warp and get buffer of image with original color with default contours;
  vector<uint8_t> warpAndGetOriginalImageSaveBufInBuf(vector<uint8_t> buf);
  // Warp and save image as bw color to file using default contours
  string warpAndGetBWImageSaveFileInBuf(vector<uint8_t> buf, string savePath, string ext);
  // Warp and get buffer of image with bw color with default contours
  vector<uint8_t> warpAndGetBWImageSaveBufInBuf(vector<uint8_t> buf);

  // Warp and save image with original color to file using custom contours
  string warpAndGetOriginalImageSaveFileCustomContour(string filePath, string savePath, struct Contour contour, string ext);
  // Warp and get buffer of image with original color with custom contours;
  vector<uint8_t> warpAndGetOriginalImageBufCustomContour(string filePath, struct Contour contour);
  // Warp and save image as bw color to file using custom contours
  string warpAndGetBWImageSaveFileCustomContour(string filePath, string savePath, struct Contour contour, string ext);
  // Warp and get buffer of image with bw color with custom contours
  vector<uint8_t> warpAndGetBWImageBufCustomContour(string filePath, struct Contour contour);

  // The following methods read image from buffer
  // Warp and save image with original color to file using custom contours
  string warpAndGetOriginalImageSaveFileCustomContourInBuf(vector<uint8_t> buf, string savePath, struct Contour contour, string ext);
  // Warp and get buffer of image with original color with custom contours;
  vector<uint8_t> warpAndGetOriginalImageBufCustonContourInBuf(vector<uint8_t> buf, struct Contour contour);
  // Warp and save image as bw color to file using custom contours
  string warpAndGetBWImageSaveFileCustomContourInBuf(vector<uint8_t> buf, string savePath, struct Contour contour, string ext);
  // Warp and get buffer of image with bw color with custom contours
  vector<uint8_t> warpAndGetBWImageBufCustomContourInBuf(vector<uint8_t> buf, struct Contour contour);

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

  // Function to calculate contour
  Contour findContourFromImagePath(string src);
  Contour findContourFromImageBuffer(vector<uint8_t> buf);
}