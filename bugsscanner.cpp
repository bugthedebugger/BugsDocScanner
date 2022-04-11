#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include "bugsscanner.hpp"
#include <string.h>
#include <stdlib.h>

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

char* warpAndGetOriginalImageSaveFile(char* filePath, char* savePath, char* ext) {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetOriginalImageSaveFileCustomContour(char* filePath, char* savePath, struct Contour contour,  char* ext) {
  cv::Mat src = cv::imread(filePath);
  warpImageCustomContour(src, src, contour);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}



vector<uint8_t> warpAndGetOriginalImageBuf(char* filePath) {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  string bufferType = ".jpg";
  vector<uchar> buf;
  cv::imencode(bufferType, src, buf);
  return buf;
}

vector<uint8_t> warpAndGetOriginalImageBufCustomContour(char* filePath, struct Contour contour) {
  cv::Mat src = cv::imread(filePath);
  warpImageCustomContour(src, src, contour);
  string bufferType = ".jpg";
  vector<uchar> buf;
  cv::imencode(bufferType, src, buf);
  return buf;
}

char* warpAndGetBWImageSaveFile(char* filePath, char* savePath, char* ext) {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetBWImageSaveFileCustomContour(char* filePath, char* savePath, struct Contour contour, char* ext) {
  cv::Mat src = cv::imread(filePath);
  warpImageCustomContour(src, src, contour);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

vector<uint8_t> warpAndGetBWImageBuf(char* filePath) {
  cv::Mat src = cv::imread(filePath);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uchar> buf;
  cv::imencode(bufferType, src, buf);
  return buf;
}

vector<uint8_t> warpAndGetBWImageBufCustomContour(char* filePath, struct Contour contour) {
  cv::Mat src = cv::imread(filePath);
  warpImageCustomContour(src, src, contour);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uchar> buf;
  cv::imencode(bufferType, src, buf);
  return buf;
}

char* warpAndGetOriginalImageSaveFileInbuf(ImgBuffer buf, char* savePath, char* ext) {
  cv::Mat src = cv::imdecode(uint8_t_list_to_vector(buf), cv::IMREAD_COLOR);
  warpImage(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetOriginalImageSaveFileCustomContourInBuf(vector<uint8_t> buf, char* savePath, struct Contour contour, char* ext) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImageCustomContour(src, src, contour);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

vector<uint8_t> warpAndGetOriginalImageSaveBufInBuf(vector<uint8_t> buf) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImage(src, src);
  string bufferType = ".jpg";
  vector<uchar> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  return processedBuf;
}

vector<uint8_t> warpAndGetOriginalImageBufCustonContourInBuf(vector<uint8_t> buf, struct Contour contour) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImageCustomContour(src, src, contour);
  string bufferType = ".jpg";
  vector<uchar> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  return processedBuf;
}

char* warpAndGetBWImageSaveFileInBuf(vector<uint8_t> buf, char* savePath, char* ext) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetBWImageSaveFileCustomContourInBuf(vector<uint8_t> buf, char* savePath, struct Contour contour, char* ext) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImageCustomContour(src, src, contour);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}


vector<uint8_t> warpAndGetBWImageSaveBufInBuf(vector<uint8_t> buf) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uchar> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  return processedBuf;
}

vector<uint8_t> warpAndGetBWImageBufCustomContourInBuf(vector<uint8_t> buf, struct Contour contour) {
  cv::Mat src = cv::imdecode(buf, cv::IMREAD_COLOR);
  warpImageCustomContour(src, src, contour);
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

void warpImageCustomContour(cv::Mat src, cv::Mat dst, struct Contour contour) {
  cv::Mat draw = src.clone();

  cv::Mat imgCopy1 = draw.clone();
  cv::Mat imgCopy2 = draw.clone();

  // Convert image to gray scale and detect edges
  edgeDetectionFilter1(imgCopy1, imgCopy1);
  edgeDetectionFilter2(imgCopy2, imgCopy2);

  cv::Point2f inputQuad[4];
  inputQuad[0] = cv::Point2f(contour.topLeft.x, contour.topRight.y);
  inputQuad[1] = cv::Point2f(contour.bottomLeft.x, contour.bottomLeft.y);
  inputQuad[2] = cv::Point2f(contour.bottomRight.x, contour.bottomRight.y);
  inputQuad[3] = cv::Point2f(contour.topRight.x, contour.topRight.y);
  cv::Point2f outputQuad[4];
  getOrigin(src, outputQuad);
  cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad, outputQuad);
  cv::warpPerspective(src, dst, warpPerspective, src.size());
}

Contour findContourFromImagePath(char* src) {
  cv::Mat draw = cv::imread(src);
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

  struct Contour contour;
  if(!biggest.empty()) {
    contour = {
      {
        (float) biggest[0][0].x,
        (float) biggest[0][0].y,
      },
      {
        (float) biggest[0][1].x,
        (float) biggest[0][1].y,
      },
      {
        (float) biggest[0][2].x,
        (float) biggest[0][2].y,
      },
      {
        (float) biggest[0][3].x,
        (float) biggest[0][3].y,
      },
    };
  }
  return contour;
}

Contour findContourFromImageBuffer(vector<uint8_t> buf) {
  cv::Mat draw = cv::imdecode(buf, cv::IMREAD_COLOR);
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

  struct Contour contour;
  if(!biggest.empty()) {
    contour = {
      {
        (float) biggest[0][0].x,
        (float) biggest[0][0].y,
      },
      {
        (float) biggest[0][1].x,
        (float) biggest[0][1].y,
      },
      {
        (float) biggest[0][2].x,
        (float) biggest[0][2].y,
      },
      {
        (float) biggest[0][3].x,
        (float) biggest[0][3].y,
      },
    };
  }
  return contour;
}

char* getFileName(char* ext) {
  uint64_t timeSinceEpochMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
  ).count();

  char *fileName = (char *) malloc(sizeof(char)*20);
  sprintf(fileName, "%ld%s", timeSinceEpochMilliseconds, ext);
  return fileName; 
}

vector<uint8_t> uint8_t_list_to_vector(ImgBuffer buf) {
  vector<uint8_t> uintBuf;
  for(long unsigned int i=0; i<buf.size; i++) {
    uintBuf.push_back(buf.buffer[i]);
  }
  cout << "vector size: " << uintBuf.size() << endl;
  return uintBuf;
}

int main() {
  string image_path = "images/original/1.jpg";
  string output_path = "images/processed/";

  char image_path_c[] = "images/original/1.jpg";
  char output_path_c[] = "images/processed/";

  cv::Mat sourceImage = cv::imread(image_path);
  vector<uint8_t> sourceImageBuffer;
  cv::imencode(".jpg", sourceImage, sourceImageBuffer);

  Contour contour = {
    {0, 0},
    {0, 150},
    {150, 150},
    {150, 0},
  };

  char file_extension[] = ".jpg";

  uint8_t* testBuffer = (uint8_t*) calloc(sourceImageBuffer.size() + 1, sizeof(uint8_t));

  for(int i=0; i<sourceImageBuffer.size(); i++) {
    testBuffer[i] = sourceImageBuffer.at(i);
  }
  
  cout << "Buffer size: " << sourceImageBuffer.size() << endl;
  cout << "Uint size: " << sizeof(testBuffer) << endl;
  warpAndGetOriginalImageSaveFileInbuf({testBuffer, sourceImageBuffer.size()}, output_path_c, file_extension);

  //
  /**
   * @brief gets image buffer
   * string byteStr = ".jpg"; 
   * vector<uchar> buf;
   * cv::imencode(byteStr, draw, buf);
   * 
   */
  free(testBuffer);
  return 0;
}
