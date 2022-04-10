#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
// #include "bugs_scanner/bugs_scanner.hpp"

#define CONTOUR_THRESHOLD 5000

using namespace std; 

vector<vector<cv::Point>> biggestContour(std::vector<std::vector<cv::Point>> contours);
void getContour(vector<vector<cv::Point>> biggestContourPoints, cv::Point2f (&dst)[4]);
void getOrigin(cv::InputArray img, cv::Point2f (&dst)[4]);
void edgeDetectionFilter1(cv::InputArray src, cv::OutputArray dst);
void warpAndGetOriginalImage(cv::Mat src, cv::Mat dst);
void warpAndGetBWImage(cv::Mat src, cv::Mat dst);
void warpAndGetBWSharpImage(cv::Mat src, cv::Mat dst);

vector<vector<cv::Point>> biggestContour(std::vector<std::vector<cv::Point>> contours) {
  vector <cv::Point> _biggestContour; 
  double maxArea = CONTOUR_THRESHOLD;

  for(int i=0; i<contours.size(); i++) {
    vector<cv::Point> contour = contours[i];
    double area = cv::contourArea(contour);
    
    if(area > maxArea) {
      double peri = cv::arcLength(contour, true);
      cv::Mat outputArray;
      cv::approxPolyDP(contour, outputArray, 0.02 * peri, true);
      if(outputArray.size().height == 4) {
        _biggestContour = outputArray;
        maxArea = area;
      }
    }
  }

  vector<vector<cv::Point>> result;
  cout << "Biggest contour: " << _biggestContour << endl;
  if(!_biggestContour.empty()) {
    result.push_back(_biggestContour);
  }

  return result;
}

void getContour(vector<vector<cv::Point>> biggestContourPoints, cv::Point2f (&dst)[4]) {
  if(!biggestContourPoints.empty()){
    dst[0] = biggestContourPoints[0][0];
    dst[1] = biggestContourPoints[0][1];
    dst[2] = biggestContourPoints[0][2];
    dst[3] = biggestContourPoints[0][3];
  }
}

void getOrigin(cv::InputArray img, cv::Point2f (&dst)[4]) {
  if(!img.empty()) {
    dst[0] = cv::Point(0,0);
    dst[1] = cv::Point(0,img.size().height);
    dst[2] = cv::Point(img.size().width,img.size().height);
    dst[3] = cv::Point(img.size().width,0);
  }
}

void edgeDetectionFilter1(cv::InputArray src, cv::OutputArray dst) {
  cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
  cv::threshold(src, dst, 128, 255, cv::THRESH_BINARY);
}

void edgeDetectionFilter2(cv::InputArray src, cv::OutputArray dst){
  cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(src, dst, {5, 5}, 1, 0);
  cv::Canny(src, dst, 245, 300);
  cv::Mat kernel = cv::Mat::ones(5,5,1);
  cv::dilate(src, dst, kernel, cv::Point(-1, -1), 2);
  cv::erode(src, dst, kernel, cv::Point(-1, -1), 1);
}

void warpAndGetOriginalImage(cv::Mat src, cv::Mat dst) {
  cv::Mat draw = src.clone();
  std::vector<std::vector<cv::Point>> contour1;
  std::vector<std::vector<cv::Point>> contour2;

  // Convert image to gray scale to prepare for edge detection
  cv::Mat imgCopy1 = draw.clone();
  cv::Mat imgCopy2 = draw.clone();

  edgeDetectionFilter1(imgCopy1, imgCopy1);
  edgeDetectionFilter2(imgCopy2, imgCopy2);

  // Find contour
  cv::findContours(imgCopy1, contour1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::findContours(imgCopy2, contour2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  vector<vector<cv::Point>> biggest1  = biggestContour(contour1);
  vector<vector<cv::Point>> biggest2 = biggestContour(contour2);
  vector<vector<cv::Point>> biggest;

  if(biggest2.empty()) {
    biggest = biggest1;
  } else if(biggest1.empty()) {
    biggest  = biggest2;
  }

  if(!biggest.empty()) {
    // Uncomment following line to hide contour line
    // cv::drawContours(draw, biggest, 0, cv::Scalar(255, 0, 0), 20);
    cv::Point2f inputQuad[4];
    getContour(biggest, inputQuad);

    cv::Point2f outputQuad[4];
    getOrigin(draw, outputQuad);

    cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad,outputQuad);
    cv::warpPerspective(draw, dst, warpPerspective, draw.size());
  }
}

void warpAndGetBWImage(cv::Mat src, cv::Mat dst) {
  cv::Mat draw = src.clone();
  std::vector<std::vector<cv::Point>> contour1;
  std::vector<std::vector<cv::Point>> contour2;

  // Convert image to gray scale to prepare for edge detection
  cv::Mat imgCopy1 = draw.clone();
  cv::Mat imgCopy2 = draw.clone();

  edgeDetectionFilter1(imgCopy1, imgCopy1);
  edgeDetectionFilter2(imgCopy2, imgCopy2);

  // Find contour
  cv::findContours(imgCopy1, contour1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  cv::findContours(imgCopy2, contour2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  vector<vector<cv::Point>> biggest1  = biggestContour(contour1);
  vector<vector<cv::Point>> biggest2 = biggestContour(contour2);
  vector<vector<cv::Point>> biggest;

  if(biggest2.empty()) {
    biggest = biggest1;
  } else if(biggest1.empty()) {
    biggest  = biggest2;
  }

  edgeDetectionFilter1(draw, draw);
  if(!biggest.empty()) {
    // Uncomment following line to hide contour line
    // cv::drawContours(draw, biggest, 0, cv::Scalar(255, 0, 0), 20);
    cv::Point2f inputQuad[4];
    getContour(biggest, inputQuad);

    cv::Point2f outputQuad[4];
    getOrigin(draw, outputQuad);

    cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad,outputQuad);
    cv::warpPerspective(draw, dst, warpPerspective, draw.size());
  }
}

void warpAndGetBWSharpImage(cv::InputArray src, cv::OutputArray dst) {
  //
}

int main() {
  string image_path = "images/original/1.jpg";
  cout << "Opencv version: " << CV_VERSION << endl;
  cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);

  if(img.empty()) {
    cout << "Cound not read the image: " << image_path << endl;
    return 1;
  }

  cout << "Image dimensions: " << img.size << endl;

  cv::Mat draw = img.clone();
  // warpAndGetOriginalImage(draw, draw);
  warpAndGetBWImage(draw, draw);
  // std::vector<std::vector<cv::Point>> contour1;
  // std::vector<std::vector<cv::Point>> contour2;

  // // Convert image to gray scale to prepare for edge detection
  // cv::Mat imgCopy1 = img.clone();
  // cv::Mat imgCopy2 = img.clone();

  // edgeDetectionFilter1(imgCopy1, imgCopy1);
  // edgeDetectionFilter2(imgCopy2, imgCopy2);

  // // Find contour
  // cv::findContours(imgCopy1, contour1, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  // cv::findContours(imgCopy2, contour2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  // vector<vector<cv::Point>> biggest1  = biggestContour(contour1);
  // vector<vector<cv::Point>> biggest2 = biggestContour(contour2);
  // vector<vector<cv::Point>> biggest;

  // if(biggest2.empty()) {
  //   biggest = biggest1;
  // } else if(biggest1.empty()) {
  //   biggest  = biggest2;
  // }

  // if(!biggest.empty()) {
  //   // Uncomment following line to hide contour line
  //   // cv::drawContours(draw, biggest, 0, cv::Scalar(255, 0, 0), 20);
  //   cv::Point2f inputQuad[4];
  //   getContour(biggest, inputQuad);

  //   cv::Point2f outputQuad[4];
  //   getOrigin(img, outputQuad);

  //   cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad,outputQuad);
  //   cv::warpPerspective(draw, draw, warpPerspective, img.size());
  // }

  /**
   * @brief gets image buffer
   * string byteStr = ".jpg"; 
   * vector<uchar> buf;
   * cv::imencode(byteStr, draw, buf);
   * 
   */

  edgeDetectionFilter1(draw, draw);
  imshow("Display window", draw);
  
  cout << endl;
  cv::waitKey(0);

  return 0;
}
