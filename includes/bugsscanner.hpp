#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std; 

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
void warpImage(cv::Mat src, cv::Mat dst, vector<vector<cv::Point>> contour);
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
string warpAndGetOriginalImageSaveFile(vector<uint8_t> buf, string savePath, string ext);
// Warp and get buffer of image with original color with default contours;
vector<uint8_t> warpAndGetOriginalImageBuf(vector<uint8_t> buf);
// Warp and save image as bw color to file using default contours
string warpAndGetBWImageSaveFile(vector<uint8_t> buf, string savePath, string ext);
// Warp and get buffer of image with bw color with default contours
vector<uint8_t> warpAndGetBWImageBuf(vector<uint8_t> buf);

// Warp and save image with original color to file using custom contours
string warpAndGetOriginalImageSaveFile(string filePath, string savePath, vector<vector<cv::Point>> contour, string ext);
// Warp and get buffer of image with original color with custom contours;
vector<uint8_t> warpAndGetOriginalImageBuf(string filePath, vector<vector<cv::Point>> contour);
// Warp and save image as bw color to file using custom contours
string warpAndGetBWImageSaveFile(string filePath, string savePath, vector<vector<cv::Point>> contour, string ext);
// Warp and get buffer of image with bw color with custom contours
vector<uint8_t> warpAndGetBWImageBuf(string filePath, vector<vector<cv::Point>> contour);

// The following methods read image from buffer
// Warp and save image with original color to file using custom contours
string warpAndGetOriginalImageSaveFile(vector<uint8_t> buf, string savePath, vector<vector<cv::Point>> contour, string ext);
// Warp and get buffer of image with original color with custom contours;
vector<uint8_t> warpAndGetOriginalImageBuf(vector<uint8_t> buf, vector<vector<cv::Point>> contour);
// Warp and save image as bw color to file using custom contours
string warpAndGetBWImageSaveFile(vector<uint8_t> buf, string savePath, vector<vector<cv::Point>> contour, string ext);
// Warp and get buffer of image with bw color with custom contours
vector<uint8_t> warpAndGetBWImageBuf(vector<uint8_t> buf, vector<vector<cv::Point>> contour);