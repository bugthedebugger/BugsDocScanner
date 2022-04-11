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
string warpAndGetOriginalImageWithDefaultContourSaveFile(string filePath, string savePath, string ext);
// Warp and get buffer of image with original color with default contours;
vector<uint8_t> warpAndGetOriginalImageWithDefaultContourBuf(string filePath);
// Warp and save image as bw color to file using default contours
string warpAndGetBWImageWithDefaultContourSaveFile(string filePath, string savePath, string ext);
// Warp and get buffer of image with bw color with default contours
vector<uint8_t> warpAndGetBWImageWithDefaultContourBuf(string filePath);

// The following methods read image from buffer
// TODO: warpAndGetOriginalImageWithDefaultContourSaveFile
string warpAndGetOriginalImageWithDefaultContourSaveFile(vector<uint8_t> buf, string savePath, string ext);
// TODO: warpAndGetOriginalImageFromBufWithDefaultContourBuf
// TODO: warpAndGetBWImageFromBufWithDefaultContourSaveFile
// TODO: warpAndGetBWImageFromBufWithDefaultContourBuf