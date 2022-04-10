// #include "bugs_scanner.hpp"

// #include <vector>

// using namespace std;

// vector<vector<cv::Point>> biggestContour(std::vector<std::vector<cv::Point>> contours) {
//   vector <cv::Point> _biggestContour; 
//   double maxArea = 5000.0;

//   for(int i=0; i<contours.size(); i++) {
//     vector<cv::Point> contour = contours[i];
//     double area = cv::contourArea(contour);
    
//     if(area > maxArea) {
//       double peri = cv::arcLength(contour, true);
//       cv::Mat outputArray;
//       cv::approxPolyDP(contour, outputArray, 0.02 * peri, true);
//       if(outputArray.size().height == 4) {
//         _biggestContour = outputArray;
//         maxArea = area;
//       }
//     }
//   }

//   vector<vector<cv::Point>> result;
//   cout << "Biggest contour: " << _biggestContour << endl;
//   if(!_biggestContour.empty()) {
//     result.push_back(_biggestContour);
//   }

//   return result;
// }
