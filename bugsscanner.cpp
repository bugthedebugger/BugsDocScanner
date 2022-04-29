#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <chrono>
#include "bugsscanner.hpp"
#include "bugsscanner_cpp.hpp"
#include <string.h>
#include <stdlib.h>

#define CONTOUR_THRESHOLD 5000

using namespace std; 

cv::Size getNewSizeFromContour(Contour contour) {
  vector<cv::Point> contours;
  contours.push_back(cv::Point(contour.topLeft.x, contour.topLeft.y));
  contours.push_back(cv::Point(contour.bottomLeft.x, contour.bottomLeft.y));
  contours.push_back(cv::Point(contour.bottomRight.x, contour.bottomRight.y));
  contours.push_back(cv::Point(contour.topRight.x, contour.topRight.y));

  double minx = contour.topLeft.x;
  double miny = contour.topLeft.y;
  double maxx = contour.topLeft.x;
  double maxy = contour.topLeft.y;

  for(int i=1; i<3; i++) {
    double tempX = contours.at(i).x;
    double tempY = contours.at(i).y;

    if(tempX < minx) {
      minx = tempX;
    }
    if(tempX > maxx) {
      maxx = tempX;
    }
    if(tempY > maxy) {
      maxy = tempY;
    }
    if(tempY < miny) {
      miny = tempY;
    }
  }

  return cv::Size(maxx-minx, maxy-miny);
}

double distanceBetweenPoints(cv::Point p1, cv::Point p2) {
  double distance = cv::norm(p1-p2);
  return distance;
}

vector<cv::Point> rearrangeContour(vector<cv::Point> contour, cv::Size imgSize) {
  cv::Point topLeftLimit = cv::Point(0,0);
  cv::Point bottomLeftLimit = cv::Point(0, imgSize.height);
  cv::Point bottomRightLimit = cv::Point(imgSize.width, imgSize.height);
  cv::Point topRightLimit = cv::Point(imgSize.width, 0);

  vector<cv::Point> controlPoints;
  controlPoints.push_back(topLeftLimit);
  controlPoints.push_back(bottomLeftLimit);
  controlPoints.push_back(bottomRightLimit);
  controlPoints.push_back(topRightLimit);

  vector<cv::Point> rearrangedContour;

  for(int i=0; i<3; i++) {
    double distance = distanceBetweenPoints(controlPoints[i], contour[0]);
    int usedIndex = 0;

    for(int j=1; j<contour.size(); j++) {
      double tempDistance = distanceBetweenPoints(controlPoints[i], contour[j]);
      if(tempDistance < distance ) {
        distance = tempDistance;
        usedIndex = j;
      }
    }
    rearrangedContour.push_back(contour.at(usedIndex));
    contour.erase(contour.begin() + usedIndex);
  }
  rearrangedContour.push_back(contour[0]);

  return rearrangedContour;
}

vector<vector<cv::Point>> biggestContour(std::vector<std::vector<cv::Point>> contours, cv::Size imgSize) {
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
    vector <cv::Point> tlbrContour = rearrangeContour(_biggestContour, imgSize);
    result.push_back(tlbrContour);
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

void getOrigin(cv::Size imgsize, cv::Point2f (&dst)[4]) {
  if(!imgsize.empty()) {
    dst[0] = cv::Point(0,0);
    dst[1] = cv::Point(0,imgsize.height);
    dst[2] = cv::Point(imgsize.width,imgsize.height);
    dst[3] = cv::Point(imgsize.width,0);
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
  src = warpImage(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetOriginalImageSaveFileCustomContour(char* filePath, char* savePath, struct Contour contour,  char* ext) {
  cv::Mat src = cv::imread(filePath);
  src = warpImageCustomContour(src, src, contour);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}



ImgBuffer warpAndGetOriginalImageBuf(char* filePath) {
  cv::Mat src = cv::imread(filePath);
  src = warpImage(src, src);
  string bufferType = ".jpg";
  vector<uint8_t> buf;
  cv::imencode(bufferType, src, buf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(buf.size(), sizeof(uint8_t));
  for(int i=0; i<buf.size(); i++) {
    buffer[i] = buf.at(i);
  }
  return {buffer, buf.size()};
}

ImgBuffer warpAndGetOriginalImageBufCustomContour(char* filePath, struct Contour contour) {
  cv::Mat src = cv::imread(filePath);
  src = warpImageCustomContour(src, src, contour);
  string bufferType = ".jpg";
  vector<uint8_t> buf;
  cv::imencode(bufferType, src, buf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(buf.size(), sizeof(uint8_t));
  for(int i=0; i<buf.size(); i++) {
    buffer[i] = buf.at(i);
  }
  return {buffer, buf.size()};
}

char* warpAndGetBWImageSaveFile(char* filePath, char* savePath, char* ext) {
  cv::Mat src = cv::imread(filePath);
  src = warpImage(src, src);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetBWImageSaveFileCustomContour(char* filePath, char* savePath, struct Contour contour, char* ext) {
  cv::Mat src = cv::imread(filePath);
  src = warpImageCustomContour(src, src, contour);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

ImgBuffer warpAndGetBWImageBuf(char* filePath) {
  cv::Mat src = cv::imread(filePath);
  src = warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uint8_t> buf;
  cv::imencode(bufferType, src, buf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(buf.size(), sizeof(uint8_t));
  for(int i=0; i<buf.size(); i++) {
    buffer[i] = buf.at(i);
  }
  return {buffer, buf.size()};
}

ImgBuffer warpAndGetBWImageBufCustomContour(char* filePath, struct Contour contour) {
  cv::Mat src = cv::imread(filePath);
  src = warpImageCustomContour(src, src, contour);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uint8_t> buf;
  cv::imencode(bufferType, src, buf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(buf.size(), sizeof(uint8_t));
  for(int i=0; i<buf.size(); i++) {
    buffer[i] = buf.at(i);
  }
  return {buffer, buf.size()};
}

char* warpAndGetOriginalImageSaveFileInbuf(uchar* buf,  uint64 bufSize, char* savePath, char* ext) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImage(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetOriginalImageSaveFileCustomContourInBuf(uchar* buf,  uint64 bufSize, char* savePath, struct Contour contour, char* ext) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImageCustomContour(src, src, contour);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

ImgBuffer warpAndGetOriginalImageSaveBufInBuf(uchar* buf,  uint64 bufSize) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImage(src, src);
  string bufferType = ".jpg";
  vector<uint8_t> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(processedBuf.size(), sizeof(uint8_t));
  for(int i=0; i<processedBuf.size(); i++) {
    buffer[i] = processedBuf.at(i);
  }
  return {buffer, processedBuf.size()};
}

ImgBuffer warpAndGetOriginalImageBufCustonContourInBuf(uchar* buf,  uint64 bufSize, struct Contour contour) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImageCustomContour(src, src, contour);
  string bufferType = ".jpg";
  vector<uint8_t> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(processedBuf.size(), sizeof(uint8_t));
  for(int i=0; i<processedBuf.size(); i++) {
    buffer[i] = processedBuf.at(i);
  }
  return {buffer, processedBuf.size()};
}

char* warpAndGetBWImageSaveFileInBuf(uchar* buf,  uint64 bufSize, char* savePath, char* ext) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImage(src, src);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}

char* warpAndGetBWImageSaveFileCustomContourInBuf(uchar* buf,  uint64 bufSize, char* savePath, struct Contour contour, char* ext) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImageCustomContour(src, src, contour);
  edgeDetectionFilter1(src, src);
  char* imageSavepath = (char*) malloc(sizeof(savePath) + sizeof(ext) + 4);
  sprintf(imageSavepath, "%s%s", savePath, getFileName(ext));
  cv::imwrite(imageSavepath, src);
  return imageSavepath;
}


ImgBuffer warpAndGetBWImageSaveBufInBuf(uchar* buf,  uint64 bufSize) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImage(src, src);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uint8_t> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(processedBuf.size(), sizeof(uint8_t));
  for(int i=0; i<processedBuf.size(); i++) {
    buffer[i] = processedBuf.at(i);
  }
  return {buffer, processedBuf.size()};
}

ImgBuffer warpAndGetBWImageBufCustomContourInBuf(uchar* buf,  uint64 bufSize, struct Contour contour) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat src = cv::imdecode(data, cv::IMREAD_COLOR);
  src = warpImageCustomContour(src, src, contour);
  edgeDetectionFilter1(src, src);
  string bufferType = ".jpg";
  vector<uint8_t> processedBuf;
  cv::imencode(bufferType, src, processedBuf);
  ImgBuffer ImgBuffer;
  uint8_t* buffer = (uint8_t*) calloc(processedBuf.size(), sizeof(uint8_t));
  for(int i=0; i<processedBuf.size(); i++) {
    buffer[i] = processedBuf.at(i);
  }

  uint64 processedBufSize = processedBuf.size();
  return {buffer, processedBufSize};
}

cv::Mat warpImage(cv::Mat src, cv::Mat dst) {
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

  vector<vector<cv::Point>> biggest1  = biggestContour(contour1, src.size());
  vector<vector<cv::Point>> biggest2 = biggestContour(contour2, src.size());
  vector<vector<cv::Point>> biggest;

  if(biggest2.empty()) {
    biggest = biggest1;
  } else if(biggest1.empty()) {
    biggest  = biggest2;
  } else if(cv::contourArea(biggest1[0]) > cv::contourArea(biggest2[0])){
    biggest = biggest1;
  } else {
    biggest = biggest2;
  }

  if(!biggest.empty()) {
    cv::Size newimgsize = getNewSizeFromContour({
      {biggest[0][0].x, biggest[0][0].y},
      {biggest[0][1].x, biggest[0][1].y},
      {biggest[0][2].x, biggest[0][2].y},
      {biggest[0][3].x, biggest[0][3].y}
    });
    cv::Mat newImage = cv::Mat(newimgsize, 3);

    cv::Point2f inputQuad[4];
    getContour(biggest, inputQuad);
    cv::Point2f outputQuad[4];
    getOrigin(newimgsize, outputQuad);
    cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad, outputQuad);
    
    cv::warpPerspective(src, newImage, warpPerspective, newimgsize);
    return newImage;
    // cv::drawContours(src, biggest, -1, cv::Scalar(255, 0, 0), 20);
  }
  return src;
}

cv::Mat warpImageCustomContour(cv::Mat src, cv::Mat dst, struct Contour contour) {
  cv::Mat draw = src.clone();

  cv::Point2f inputQuad[4];
  inputQuad[0] = cv::Point2f(contour.topLeft.x, contour.topLeft.y);
  inputQuad[1] = cv::Point2f(contour.bottomLeft.x, contour.bottomLeft.y);
  inputQuad[2] = cv::Point2f(contour.bottomRight.x, contour.bottomRight.y);
  inputQuad[3] = cv::Point2f(contour.topRight.x, contour.topRight.y);
  cv::Point2f outputQuad[4];

  cv::Size newimgsize = getNewSizeFromContour(contour);
  cv::Mat newImage = cv::Mat(newimgsize, 3);

  getOrigin(newimgsize, outputQuad);
  cv::Mat warpPerspective = cv::getPerspectiveTransform(inputQuad, outputQuad);
  cv::warpPerspective(src, newImage, warpPerspective, newimgsize);
  return newImage;
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

  vector<vector<cv::Point>> biggest1  = biggestContour(contour1, draw.size());
  vector<vector<cv::Point>> biggest2 = biggestContour(contour2, draw.size());
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
         biggest[0][0].x,
         biggest[0][0].y,
      },
      {
         biggest[0][1].x,
         biggest[0][1].y,
      },
      {
         biggest[0][2].x,
         biggest[0][2].y,
      },
      {
         biggest[0][3].x,
         biggest[0][3].y,
      },
    };
  }
  return contour;
}

Contour findContourFromImageBuffer(uchar* buf,  uint64 bufSize) {
  cv::Mat data = cv::Mat(1,bufSize, CV_8U, buf);
  cv::Mat draw = cv::imdecode(data, cv::IMREAD_COLOR);
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

  vector<vector<cv::Point>> biggest1  = biggestContour(contour1, draw.size());
  vector<vector<cv::Point>> biggest2 = biggestContour(contour2, draw.size());
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
         biggest[0][0].x,
         biggest[0][0].y,
      },
      {
         biggest[0][1].x,
         biggest[0][1].y,
      },
      {
         biggest[0][2].x,
         biggest[0][2].y,
      },
      {
         biggest[0][3].x,
         biggest[0][3].y,
      },
    };
  } else {
    contour = {{ 0,0 }, { 0,0 }, { 0,0 }, { 0,0 }};
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
  for(uint64 i=0; i<buf.size; i++) {
    uintBuf.push_back(buf.buffer[i]);
  }
  return uintBuf;
}

ImgBuffer createImgBuffer(uint8_t* buffer, uint64 size) {
  return {buffer, size};
}

Coordinates createCoordinates(int64 x, int64 y) {
  return {x, y};
}

Contour createContour(
  Coordinates topLeft,
  Coordinates bottomLeft,
  Coordinates bottomRight,
  Coordinates topRight
) {
  return {
    topLeft, bottomLeft, bottomRight, topRight
  };
}

int main() {
  string image_path = "images/original/5.jpg";
  string output_path = "images/processed/";

  char image_path_c[] = "images/original/5.jpg";
  char output_path_c[] = "images/processed/";

  cv::Mat sourceImage = cv::imread(image_path);
  vector<uint8_t> sourceImageBuffer;
  cv::imencode(".jpg", sourceImage, sourceImageBuffer);

  Contour contour = {
    {0, 0},
    {0, 3456},
    {4608, 3456},
    {4608, 0},
  };

  char file_extension[] = ".jpg";

  uint8_t* testBuffer = (uint8_t*) calloc(sourceImageBuffer.size(), sizeof(uint8_t));

  for(int i=0; i<sourceImageBuffer.size(); i++) {
    testBuffer[i] = sourceImageBuffer.at(i);
  }
  cout << "Buffer size: " << sourceImageBuffer.size() << endl;
  cout << "Uint size: " << sizeof(testBuffer) << endl;
  // ImgBuffer bufferFromFunc = warpAndGetBWImageSaveBufInBuf({testBuffer, sourceImageBuffer.size()});
  // ImgBuffer bufferFromFunc = warpAndGetBWImageBuf(image_path_c);
  // ImgBuffer bufferFromFunc = warpAndGetOriginalImageBuf(image_path_c);
  // ImgBuffer bufferFromFunc = warpAndGetOriginalImageSaveBufInBuf(testBuffer, sourceImageBuffer.size());
  // ImgBuffer bufferFromFunc = warpAndGetOriginalImageBufCustomContour(image_path_c, contour);
  // ImgBuffer bufferFromFunc = warpAndGetBWImageBufCustomContour(image_path_c, contour);
  ImgBuffer bufferFromFunc = warpAndGetOriginalImageBufCustonContourInBuf(testBuffer, sourceImageBuffer.size(), contour);
  // distanceBetweenPoints(cv::Point(100,0), cv::Point(0, 0));
  // ImgBuffer bufferFromFunc = warpAndGetBWImageBufCustomContourInBuf(testBuffer, sourceImageBuffer.size(), contour);
  
  cv::Mat data = cv::Mat(1, bufferFromFunc.size, CV_8U, bufferFromFunc.buffer);
  cv::Mat img = cv::imdecode(data, cv::IMREAD_COLOR);
  // cv::resizeWindow("Image", cv::Size(300, 600));
  // cv::imshow("Image", img);

  cv::namedWindow("custom window", cv::WINDOW_KEEPRATIO);
  cv::imshow("custom window", img);
  cv::resizeWindow("custom window", 600, 800);

  cv::waitKey();
  

  //
  /**
   * @brief gets image buffer
   * string byteStr = ".jpg"; 
   * vector<uint8_t> buf;
   * cv::imencode(byteStr, draw, buf);
   * 
   */
  free(testBuffer);
  return 0;
}
