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

  // State file names without extensions
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

  // State names
  std::string index_files2[] = {
    "alaska", "alabama", "arkansas", "arizona", "california",
    "colorado", "connecticut", "delaware", "florida", "georgia",
    "hawaii", "iowa", "idaho", "illinois", "indiana",
    "kansas", "kentucky", "louisiana", "massachusetts", "maryland",
    "maine", "michigan", "minnesota", "missouri", "mississippi",
    "montana", "north_carolina", "north_dakota", "nebraska", "new_hampshire",
    "new_jersey", "new_mexico", "nevada", "new_york", "ohio",
    "oklahoma", "oregon", "pennsylvania", "rhode_island", "south_carolina",
    "south_dakota", "tennessee", "texas", "utah", "virginia",
    "vermont", "washington", "wisconsin", "west_virginia", "wyoming"
  };

  // Create vector of 50 state flag images to store files in
  std::vector<Mat> images;

  // Read 50 flag images and store in vector images
  for (int i = 0; i < 50; ++i) {
    std::cout << "Adding flag: " << index_files2[i] << " to bank." << std::endl;
    std::string file_name = index_files[i] + ".jpg";
    Mat index_file = imread("flags/" + file_name);
    images.push_back(index_file);
  }

  // Status
  std::cout << "-------------------------------------------------" << std::endl;
  std::cout << "Creating map" << std::endl;

  // 3 Layered Map structure Stores flag names
  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>> flag_map;

  std::vector<std::string> flag_debug;

  std::vector<Mat> histograms;
  for (int i = 0; i < 50; ++i) {
    histograms.push_back(CommonColorFinder::populateHistogram(images.at(i)));
  }

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
      std::unordered_map<int, std::list<std::string>> final_map; // map
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(index_files[i]); // Push this state name onto flag_name list

      // Create the green bucket int to flag name and put into map
      std::pair<int, std::list<std::string>> green_to_flags(current_image.getGreenBucket(), flag_names);
      final_map.insert(green_to_flags);

      // Create mid map with blue bucket and final_map
      std::unordered_map<int, std::unordered_map<int, std::list<std::string>>> mid_map;

      // Create pair of blue bucket int to final map and put into map
      std::pair<int, std::unordered_map<int, std::list<std::string>>> blue_to_green(current_image.getBlueBucket(), final_map);
      mid_map.insert(blue_to_green);

      // Insert map made into red
      std::pair<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>> red_to_blue(current_image.getRedBucket(), mid_map);
      flag_map.insert(red_to_blue);

      std::cout << "Storing flag in new red bucket location for flag : " << index_files2[i] <<
        " at location: " << current_image.getRedBucket() << ", " << current_image.getBlueBucket() <<
        ", " << current_image.getGreenBucket() << std::endl;
      
      std::string location = std::to_string(current_image.getRedBucket()) + "," +
        std::to_string(current_image.getBlueBucket()) + "," +
        std::to_string(current_image.getGreenBucket()) + ": " +
        index_files2[i];
      flag_debug.push_back(location);

      // Red bucket exists, check blue
    } else if (flag_map.at(current_image.getRedBucket()).find(current_image.getBlueBucket()) ==
               flag_map.at(current_image.getRedBucket()).end()) {

      // Add the colorbucket and its name

      // Create final map with string of flag names
      std::unordered_map<int, std::list<std::string>> final_map; // map
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(index_files[i]); // Push this state name onto flag_name list

      // Create the green bucket int to flag name and put into map
      std::pair<int, std::list<std::string>> green_to_flags(current_image.getGreenBucket(), flag_names);
      final_map.insert(green_to_flags);

      // Create mid map with blue bucket and final_map
      std::unordered_map<int, std::unordered_map<int, std::list<std::string>>> mid_map;

      std::pair<int, std::unordered_map<int, std::list<std::string>>> blue_to_green(current_image.getBlueBucket(), final_map);
      mid_map.insert(blue_to_green);

      flag_map.at(current_image.getRedBucket()).insert(blue_to_green);

      std::cout << "Storing flag at existing red bucket but new blue bucket location for flag : " << index_files2[i] <<
        " at location: " << current_image.getRedBucket() << ", " << current_image.getBlueBucket() <<
        ", " << current_image.getGreenBucket() << std::endl;

      std::string location = std::to_string(current_image.getRedBucket()) + "," +
        std::to_string(current_image.getBlueBucket()) + "," +
        std::to_string(current_image.getGreenBucket()) + ": " +
        index_files2[i];
      flag_debug.push_back(location);

      // Red and blue bucket exist, check green
    } else if (flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).find(current_image.getGreenBucket()) ==
               flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).end()) {

      // Add the colorbucket and its name

      // Create final map with string of flag names
      std::unordered_map<int, std::list<std::string>> final_map; // map
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(index_files[i]); // Push this state name onto flag_name list

      // Create the green bucket int to flag name and put into map
      std::pair<int, std::list<std::string>> green_to_flags(current_image.getGreenBucket(), flag_names);
      final_map.insert(green_to_flags);

      flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).insert(green_to_flags);

      std::cout << "Storing flag at existing red and blue bucket but new green bucket location for flag : " << index_files2[i] <<
        " at location: " << current_image.getRedBucket() << ", " << current_image.getBlueBucket() <<
        ", " << current_image.getGreenBucket() << std::endl;

      std::string location = std::to_string(current_image.getRedBucket()) + "," +
        std::to_string(current_image.getBlueBucket()) + "," +
        std::to_string(current_image.getGreenBucket()) + ": " +
        index_files2[i];
      flag_debug.push_back(location);

      // The RBG bucket combination exists for a flag
    } else {
      flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).at(current_image.getGreenBucket()).push_back(index_files[i]);
      std::cout << "Storing flag in existing location for flag : " << index_files2[i] <<
        " at location: " << current_image.getRedBucket() << ", " << current_image.getBlueBucket() <<
        ", " << current_image.getGreenBucket() << std::endl;

      std::string location = std::to_string(current_image.getRedBucket()) + "," +
        std::to_string(current_image.getBlueBucket()) + "," +
        std::to_string(current_image.getGreenBucket()) + ": " +
        index_files2[i];
      flag_debug.push_back(location);
    }
  }

  std::cout << "----------------------------------------------------------------" << std::endl;
  std::cout << "Flag positions: " << std::endl;

  for (int i = 0; i < 50; ++i) {
    std::cout << flag_debug.at(i) << std::endl;
  }

  std::list<std::string>::iterator it = flag_map.at(7).at(7).at(7).begin();
  while (it != flag_map.at(7).at(7).at(7).end()) {
    std::cout << *(it) << std::endl;
    ++it;
  }

  return 0;
}