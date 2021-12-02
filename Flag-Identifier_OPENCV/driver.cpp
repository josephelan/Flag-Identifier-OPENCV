/*********************************************************************
 * @file       driver.cpp
 * @brief      driver runs a program t...
 * @details    ...
 *
 * @author Joseph Lan
 * @author Andy Tran
 * @author Kevin Xu
 *             
 * @date 2021 December 1
 *
 * FLAG IDENTIIFIER
 * CSS 487 Final Project
 * Prof. Clark Olson
 *********************************************************************/

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <unordered_map>
#include <string>

#include "ColorBucket.h"
#include "CommonColorFinder.h"

using namespace cv;

/**
 * @brief main method drives the program through a series of steps in order
 *        to determine what flag is being input into the picture.
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char* argv[]) {

  //File Names without extensions
  std::string index_files[] = {
    "ak", "al", "ar", "az", "ca",
    "co", "ct", "de", "fl", "ga",
    "hi", "ia", "id", "il", "in",
    "ks", "ky", "la", "ma", "md",
    "me", "mi", "mn", "mo", "ms",
    "mt", "nc", "nd", "ne", "nh",
    "nj", "nm", "nv", "ny", "oh",
    "ok", "or", "pa", "ri", "sc",
    "sd", "tn", "tx", "ut", "va",
    "vt", "wa", "wi", "wv", "wy"
  };

  // Create vector of 50 state flag images to store files in
  std::vector<Mat> images;

  // Read 50 flag images and store in "vector"
  for (int i = 0; i < 50; ++i) {
    std::string file_name = index_files[i] + ".jpg";
    Mat index_file = imread(file_name);
    images.push_back(index_file);
  }

  // Create "vector" to store final histogram values for each index image
  // Pull images from images[] and calculate most common color histogram for each
  // Store histogram in histogram "vector

  // 3 Layered Map
  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::string>>> flag_map;

  // flag_map maps red bucket in ints to a corresponding map of blue bucket
  // next layer maps blue bucket int to a corresponding map of green bucket
  // green bucket maps int bucket to a string
  



  for (int i = 0; i < 50; ++i) {

  }


  // Mat foreground = imread("foreground.jpg");
  // Mat background = imread("background.jpg");

  // // Print out and write final image for opencv methods
  // namedWindow("my-output", WINDOW_NORMAL);
  // resizeWindow("my-output", colored.cols / 3, colored.rows / 3);
  // imshow("my-output", colored);
  // imwrite("myoutput.jpg", colored);
  // waitKey(0);

  return 0;
}