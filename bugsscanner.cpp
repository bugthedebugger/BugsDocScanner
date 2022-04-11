#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include "bugsscanner.hpp"
// #include "bugs_scanner/bugs_scanner.hpp"

#define CONTOUR_THRESHOLD 5000

using namespace std; 

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

string warpAndGetOriginalImageSaveFile(string filePath, string savePath, string ext=".jpg") {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  string imageSavepath = savePath + getFileName(ext);
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

string warpAndGetOriginalImageSaveFile(string filePath, string savePath, vector<vector<cv::Point>> contour,  string ext=".jpg") {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src, contour);
  string imageSavepath = savePath + getFileName(ext);
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

vector<uint8_t> warpAndGetOriginalImageBuf(string filePath) {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  string bufferType = ".jpg";
  vector<uchar> buf;
  cv::imencode(bufferType, src, buf);
  return buf;
}

vector<uint8_t> warpAndGetOriginalImageBuf(string filePath, vector<vector<cv::Point>> contour) {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src, contour);
  string bufferType = ".jpg";
  vector<uchar> buf;
  cv::imencode(bufferType, src, buf);
  return buf;
}

string warpAndGetBWImageSaveFile(string filePath, string savePath, string ext=".jpg") {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string imageSavepath = savePath + getFileName(ext);
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

vector<uint8_t> warpAndGetBWImageBuf(string filePath) {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uchar> buf;
  cv::imencode(bufferType, src, buf);
  return buf;
}

string warpAndGetOriginalImageSaveFile(vector<uint8_t> buf, string savePath, string ext) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImage(src, src);
  string imageSavepath = savePath + getFileName(ext);
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

vector<uint8_t> warpAndGetOriginalImageBuf(vector<uint8_t> buf) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImage(src, src);
  string bufferType = ".jpg";
  vector<uchar> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  return processedBuf;
}

string warpAndGetBWImageSaveFile(vector<uint8_t> buf, string savePath, string ext) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string imageSavepath = savePath + getFileName(ext);
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

vector<uint8_t> warpAndGetBWImageBuf(vector<uint8_t> buf) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uchar> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  return processedBuf;
}

void warpImage(cv::Mat src, cv::Mat dst) {
  cv::Mat draw = src.clone();
  std::vector<std::vector<cv::Point>> contour1;
  std::vector<std::vector<cv::Point>> contour2;

  cv::Mat imgCopy1 = draw.clone();
  cv::Mat imgCopy2 = draw.clone();

  // Convert image to gray scale and detect edges
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
    cv::Point2f inputQuad[4];
    getContour(biggest, inputQuad);
    cv::Point2f outputQuad[4];
    getOrigin(src, outputQuad);
    cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad, outputQuad);
    cv::warpPerspective(src, dst, warpPerspective, src.size());
  }
}

void warpImage(cv::Mat src, cv::Mat dst, vector<vector<cv::Point>> contour) {
  cv::Mat draw = src.clone();

  cv::Mat imgCopy1 = draw.clone();
  cv::Mat imgCopy2 = draw.clone();

  // Convert image to gray scale and detect edges
  edgeDetectionFilter1(imgCopy1, imgCopy1);
  edgeDetectionFilter2(imgCopy2, imgCopy2);

  if(!contour.empty()) {
    cv::Point2f inputQuad[4];
    getContour(contour, inputQuad);
    cv::Point2f outputQuad[4];
    getOrigin(src, outputQuad);
    cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad, outputQuad);
    cv::warpPerspective(src, dst, warpPerspective, src.size());
  }
}

string getFileName(string ext) {
  uint64_t timeSinceEpochMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
  ).count();

  char fileName[200];
  sprintf(fileName, "%ld%s", timeSinceEpochMilliseconds, ext.c_str());
  return fileName; 
}




int main() {
  string image_path = "images/original/1.jpg";
  string output_path = "images/processed/";

  cv::Mat sourceImage = cv::imread(image_path);
  vector<uint8_t> sourceImageBuffer;
  cv::imencode(".jpg", sourceImage, sourceImageBuffer);

  vector<vector<cv::Point>> contour;
  contour.push_back({
    cv::Point(0,0),
    cv::Point(0, 50),
    cv::Point(50, 50),
    cv::Point(50, 0),
  });

  warpAndGetOriginalImageSaveFile(image_path, output_path, contour, ".jpg");
  vector<uint8_t> processedBuffer = warpAndGetOriginalImageBuf(image_path, contour);
  // warpAndGetOriginalImageWithDefaultContourSaveFile(image_path, "images/processed/");
  // warpAndGetBWImageWithDefaultContourSaveFile(image_path, "images/processed/");
  // vector<uint8_t> buffer1 = warpAndGetOriginalImageWithDefaultContourBuf(image_path);
  // vector<uint8_t> buffer2 = warpAndGetBWImageWithDefaultContourBuf(image_path);

  cv::Mat img = cv::imdecode(processedBuffer, cv::IMREAD_COLOR);
  
  cv::imshow("Buffered", img);
  //
  /**
   * @brief gets image buffer
   * string byteStr = ".jpg"; 
   * vector<uchar> buf;
   * cv::imencode(byteStr, draw, buf);
   * 
   */
  cv::waitKey(0);

  return 0;
}
