#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std; 

// Function to calculate biggest contour
vector<vector<cv::Point>> biggestContour(std::vector<std::vector<cv::Point>> contours, cv::Size imgSize);
// Function to get the contour
void getContour(vector<vector<cv::Point>> biggestContourPoints, cv::Point2f (&dst)[4]);
// Function to get origin of image
void getOrigin(cv::Size imgsize, cv::Point2f (&dst)[4]);
// Function to detect edge with bw and threshold
void edgeDetectionFilter1(cv::InputArray src, cv::OutputArray dst);
// Function to detect edge with gaussian and canny
void edgeDetectionFilter2(cv::InputArray src, cv::OutputArray dst);
// Warp image using default contour
cv::Mat warpImage(cv::Mat src, cv::Mat dst);
// Warp image using custom contour
cv::Mat warpImageCustomContour(cv::Mat src, cv::Mat dst, struct Contour contour);
// uint8_t list to vector<uint8_t>
vector<uint8_t> uint8_t_list_to_vector(ImgBuffer buf);
// get distance between two coordinates
double distanceBetweenPoints(cv::Point p1, cv::Point p2);
// re-arrange contour points as topleft, bottomleft, bottomright, toright;
vector<cv::Point> rearrangeContour(vector<cv::Point> contour,  cv::Size imgSize);
// returns size from contour
cv::Size getNewSizeFromContour(Contour contour);