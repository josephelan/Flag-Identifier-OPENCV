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
#include <list>
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
  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>> flag_map;

  // flag_map maps red bucket in ints to a corresponding map of blue bucket
  // next layer maps blue bucket int to a corresponding map of green bucket
  // green bucket maps int bucket to a string
  



  for (int i = 0; i < images.size(); ++i) {
    
    // Get ColorBucket object, which holds the bucket for the most common color
    // For image at position (i)
    ColorBucket current_image = CommonColorFinder::getCommonColorBucket(images.at(i));
    
    // If hashmap doesn't have an image with this red bucket, add it
    if (flag_map.find(current_image.getRedBucket()) == flag_map.end()) {
      
      // Add the colorbucket and its name

      // Create final map with string of flag names
      std::unordered_map<int, std::list<std::string>> final_map;
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(index_files[i]); // Push this state name onto flag_name list
      final_map.emplace(current_image.getGreenBucket(), flag_names); // Push flag_name list and list of names onto final_map

      // Create mid map with blue bucket and final_map
      std::unordered_map<int, std::unordered_map<int, std::list<std::string>>> mid_map;
      mid_map.emplace(current_image.getBlueBucket(), final_map); // Put in blue bucket and final map
      flag_map.emplace(current_image.getRedBucket(), mid_map); // Put in red bucket with blue bucket map

    // Red bucket exists, check blue
    } else if (flag_map.at(current_image.getRedBucket()).find(current_image.getBlueBucket()) ==
               flag_map.at(current_image.getRedBucket()).end()) {

      // Add the colorbucket and its name

      // Create final map with string of flag names
      std::unordered_map<int, std::list<std::string>> final_map;
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(index_files[i]); // Push this state name onto flag_name list
      final_map.emplace(flag_names, flag_names); // Push flag_name list and list of names onto final_map

      flag_map.at(current_image.getRedBucket()).emplace(current_image.getBlueBucket(), final_map);

    // Red and blue bucket exist, check green
    } else if (flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).find(current_image.getGreenBucket()) ==
               flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).end()) {

      // Add the colorbucket and its name
      std::unordered_map<int, std::list<std::string>> final_map;
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(index_files[i]); // Push this state name onto flag_name list
      flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).emplace(current_image.getGreenBucket(), final_map);
    

    // The RBG bucket combination exists for a flag
    } else {
      flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).at(current_image.getGreenBucket()).push_back(index_files[i]);
    }
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