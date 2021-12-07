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
 * @brief   findClosestFlag method will analyze an input image and determine
 *            similar looking flags based on the most common color present.
 *
 * @param   input is a Mat representing the input image.
 * @param   flag_map is the multi-dimensional map containing a list of strings
 *            with state names in their appropriate bucket based on the most
 *            common color present.
 *
 * @return  a list of similar looking flags based on the input image.
 */
std::list<std::string> findClosestFlag(const std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>>& flag_map,
                                       const ColorBucket& image_bucket) {

  // Return the key,value pair found at the bottom of hashmap
  std::list<std::string> result;

  // Variables for readability
  int red = image_bucket.getRedBucket();
  int blue = image_bucket.getBlueBucket();
  int green = image_bucket.getGreenBucket();
  const int max_bucket = 7;

  // Debug                                                          REMOVE LATER
  std::cout << "r: " << red << std::endl;
  std::cout << "b: " << blue << std::endl;
  std::cout << "g: " << green << std::endl;

  /*
   * Try to find the exact r,b,g bucket in the hash structure
   */
  try {

    //Look for flags in the exact same bucket as the input image
    result = flag_map.at(red).at(blue).at(green);

    // If r,b,g not found in hash structure, look for flags in closest
    // color direction +/- 1
  } catch (std::exception e1) {

    //Look for flags in adjacent buckets as the input image
    for (int r = red - 1; r <= red + 1; ++r) {
      if (r < 0 || r > max_bucket) {
        continue;
      }
      for (int b = blue - 1; b <= blue + 1; ++b) {
        if (b < 0 || b > max_bucket) {
          continue;
        }
        for (int g = green - 1; g <= green + 1; ++g) {
          if (g < 0 || g > max_bucket) {
            continue;
          }

          // Merge all adjacent flag_list buckets
          try {
            std::list<std::string> flag_list = flag_map.at(red).at(blue).at(green);

            // Merges list<string> of all closest flags in adjacent buckets
            result.merge(flag_list);

            // If an adjacent bucket combination does not exist
          } catch (std::exception e2) {
            continue; // Go to next adjacent bucket
          }
        }
      }
    }
  }
  return result;
}

/**
 * @brief filterRatios gets closer to the target flag by filtering out images
 *        based on histogram ratios in aand color bucket information
 *
 * @pre
 * @post
 *
 * @param list
 * @param histogram
 */
void filterRatios(std::list<std::string>& list,
                  const std::unordered_map<std::string, ColorBucket>& color_buckets,
                  const ColorBucket& image_bucket) {
  if (list.size() <= 1) {
    return;
  }

  // Ratio of most common color bucket to all pixels in the image
  float image_ratio = image_bucket.getCommonColorRatio();
  const float acceptable_error = image_ratio * 0.05f;
  float min_ratio = image_ratio - acceptable_error;
  float max_ratio = image_ratio + acceptable_error;

  std::list<std::string>::iterator it = list.begin();
  while (it != list.end()) {
    std::string s = *it;

    // Get ratio of most common color bucket for each flag in list
    float index_ratio = color_buckets.at(s).getCommonColorRatio();

    // Check if ratio is within acceptable range
    if (index_ratio < min_ratio || index_ratio > max_ratio) {

      // Delete if not in acceptable range
      it = list.erase(it);
    } else {
      ++it;
    }
  }
  return;
}

/**
 * @brief Returns number of edge pixels in an image through canny edge
 *        detection
 *
 * @pre img not null
 * @post no change to image
 *
 * @param img image to count
 * @return count of edge pixels
 */
int countEdgePixels(const Mat& img) {
  int count = 0;
  for (int row = 0; row < img.rows; ++row) {
    for (int col = 0; col < img.cols; ++col) {

      // If this is an edge, increment count
      if (img.at<uchar>(row, col) != 0) {
        ++count;
      }
    }
  }
  return count;
}

/**
 * @brief Filters our flags from list using canny edge count
 *
 * @pre list not empty, images is filled with flag templates, test_image not null
 * @post list changed to remove flags not within range of canny edge counts
 *
 * @param list possible flags that match
 * @param images map or images kept as image database
 * @param test_image image being tested
 */
void filterCannyEdgeCount(std::list<std::string>& list,
                          const std::unordered_map<std::string, Mat>& images,
                          const Mat& test_image) {

  // If only one item in list, end
  if (list.size() <= 1) {
    return;
  }

  // Define variables for edge counting
  const int gaussian_kernel = 7;
  const double gaussian_deviation = 2.0;
  const int thresh1 = 20;
  const int thresh2 = 60;

  // Count test image edge counts
  Mat test_clone = test_image.clone();
  cvtColor(test_clone, test_clone, COLOR_BGR2GRAY);
  GaussianBlur(test_clone, test_clone, Size(gaussian_kernel, gaussian_kernel), gaussian_deviation, gaussian_deviation);
  Canny(test_clone, test_clone, thresh1, thresh2);
  int test_edge_counts = countEdgePixels(test_clone);

  // Convert to ratio and set boundaries
  int num_pix = test_image.rows * test_image.cols;
  float ratio = (float)test_edge_counts / (float)num_pix;
  const float acceptable_error = ratio * 0.05f;
  float min_ratio = ratio - acceptable_error;
  float max_ratio = ratio + acceptable_error;

  //std::cout << "Test image ratio for edge: " << ratio << std::endl;
  //std::cout << "min_ratio for edge: " << min_ratio << std::endl;
  //std::cout << "max_ratio for edge: " << max_ratio << std::endl;

  // For each flag in list
  std::vector<float> possible_ratios;
  for (std::string x : list) {

    // Convert image into canny edge image
    Mat edged_image = images.at(x).clone();
    cvtColor(edged_image, edged_image, COLOR_BGR2GRAY);
    GaussianBlur(edged_image, edged_image, Size(gaussian_kernel, gaussian_kernel), gaussian_deviation, gaussian_deviation);
    Canny(edged_image, edged_image, thresh1, thresh2);

    // Count edges
    int edged_count = countEdgePixels(edged_image);
    int edged_pix = edged_image.rows * edged_image.cols;
    float edged_ratio = (float)edged_count / (float)edged_pix;
    possible_ratios.push_back(edged_ratio);
  }

  // Compare count ratio in images to count ratio for the test file
  int num_possible = (int)list.size();
  for (int i = 0; i < num_possible; ++i) {

    std::string flag = list.front();
    //std::cout << "Testing edge ratio: " << flag << std::endl;
    //std::cout << "Flag has count: " << possible_ratios.at(i) << std::endl;

    // If head of list ratio is within bounds, add it back in
    if (possible_ratios.at(i) >= min_ratio &&
        possible_ratios.at(i) <= max_ratio) {
      //std::cout << "Keeping this flag" << std::endl;
      list.push_back(flag);
    } else {
      //std::cout << "Removing this flag" << std::endl;
    }

    // Destroy first element
    list.pop_front();
  }
}

void buildFlagMap(std::list<std::string>& index_files,
                  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>>& flag_map,
                  const std::unordered_map<std::string, Mat>& images,
                  std::unordered_map<std::string, ColorBucket>& index_color_buckets) {
  // flag_map maps red bucket in ints to a corresponding map of blue bucket
  // next layer maps blue bucket int to a corresponding map of green bucket
  // green bucket maps int bucket to a string
  std::list<std::string>::iterator it = index_files.begin();
  while (it != index_files.end()) {
    std::string name = *it;

    // Get ColorBucket object, which holds the bucket for the most common color
    // For image at position (i)
    ColorBucket current_image = CommonColorFinder::getCommonColorBucket(images.at(name));

    //Add the colorbucket to the map of colorbuckets for index images
    std::pair<std::string, ColorBucket> index_colorbucket(name, current_image);
    index_color_buckets.insert(index_colorbucket);

    // If hashmap doesn't have an image with this red bucket, add it
    if (flag_map.find(current_image.getRedBucket()) == flag_map.end()) {

      // Add the colorbucket and its name

      // Create final map with string of flag names
      std::unordered_map<int, std::list<std::string>> final_map; // map
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(name); // Push this state name onto flag_name list

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

      /*std::string location = std::to_string(current_image.getRedBucket()) + "," +
          std::to_string(current_image.getBlueBucket()) + "," +
          std::to_string(current_image.getGreenBucket()) + ": " +
          index_files2[i];
      flag_debug.push_back(location);*/

      // Red bucket exists, check blue
    }         else if (flag_map.at(current_image.getRedBucket()).find(current_image.getBlueBucket()) ==
              flag_map.at(current_image.getRedBucket()).end()) {

      // Add the colorbucket and its name

      // Create final map with string of flag names
      std::unordered_map<int, std::list<std::string>> final_map; // map
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(name); // Push this state name onto flag_name list

      // Create the green bucket int to flag name and put into map
      std::pair<int, std::list<std::string>> green_to_flags(current_image.getGreenBucket(), flag_names);
      final_map.insert(green_to_flags);

      // Create mid map with blue bucket and final_map
      std::unordered_map<int, std::unordered_map<int, std::list<std::string>>> mid_map;

      std::pair<int, std::unordered_map<int, std::list<std::string>>> blue_to_green(current_image.getBlueBucket(), final_map);
      mid_map.insert(blue_to_green);

      flag_map.at(current_image.getRedBucket()).insert(blue_to_green);

      /*std::string location = std::to_string(current_image.getRedBucket()) + "," +
          std::to_string(current_image.getBlueBucket()) + "," +
          std::to_string(current_image.getGreenBucket()) + ": " +
          index_files2[i];
      flag_debug.push_back(location);*/

      // Red and blue bucket exist, check green
    }         else if (flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).find(current_image.getGreenBucket()) ==
              flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).end()) {

      // Add the colorbucket and its name

      // Create final map with string of flag names
      std::unordered_map<int, std::list<std::string>> final_map; // map
      std::list<std::string> flag_names; // Create flag name list
      flag_names.push_back(name); // Push this state name onto flag_name list

      // Create the green bucket int to flag name and put into map
      std::pair<int, std::list<std::string>> green_to_flags(current_image.getGreenBucket(), flag_names);
      final_map.insert(green_to_flags);

      flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).insert(green_to_flags);

      /*std::string location = std::to_string(current_image.getRedBucket()) + "," +
          std::to_string(current_image.getBlueBucket()) + "," +
          std::to_string(current_image.getGreenBucket()) + ": " +
          index_files2[i];
      flag_debug.push_back(location);*/

      // The RBG bucket combination exists for a flag
    }         else {
      flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).at(current_image.getGreenBucket()).push_back(name);

      /*std::string location = std::to_string(current_image.getRedBucket()) + "," +
          std::to_string(current_image.getBlueBucket()) + "," +
          std::to_string(current_image.getGreenBucket()) + ": " +
          index_files2[i];
      flag_debug.push_back(location);*/
    }
    ++it;
  }
}

/**
 * @brief Test function to test for any certain flag during code test
 */
void test(const std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>>& flag_map,
          const std::unordered_map<std::string, ColorBucket>& color_buckets,
          const std::unordered_map<std::string, Mat>& images,
          const std::string filename) {

  std::cout << "Testing: " << filename << " in program." << std::endl;

  Mat test_file = imread(filename);

  // Print out test image
  namedWindow("test", WINDOW_NORMAL);
  resizeWindow("test", test_file.cols, test_file.rows);
  imshow("test", test_file);
  imwrite("test.jpg", test_file);
  waitKey(0);

  // ColorBucket for the input image (image we're looking for)
  ColorBucket image_bucket = CommonColorFinder::getCommonColorBucket(test_file);

  // Step 1: findClosestFlag to narrow down colorBuckets
  //std::cout << "Finding closest flags by bucket" << std::endl;
  std::list<std::string> possible_flags = findClosestFlag(flag_map, image_bucket);

  for (std::string x : possible_flags) {
    namedWindow("Possilibity after MCC", WINDOW_NORMAL);
    resizeWindow("Possilibity after MCC", images.at(x).cols, images.at(x).rows);
    imshow("Possilibity after MCC", images.at(x));
    waitKey(0);
  }

  // Step 2: filterRatios to get closer to flag
  //std::cout << "Filter with ratios" << std::endl;
  filterRatios(possible_flags, color_buckets, image_bucket);

  for (std::string x : possible_flags) {
    namedWindow("Possilibity after MCC ratio", WINDOW_NORMAL);
    resizeWindow("Possilibity after MCC ratio", images.at(x).cols, images.at(x).rows);
    imshow("Possilibity after MCC ratio", images.at(x));
    waitKey(0);
  }

  // Resize for img dims
  float change = (float)test_file.rows / 240;
  float width = (float)test_file.cols / change;
  Size resizing((int)width, 240);
  resize(test_file, test_file, resizing, INTER_LINEAR);

  // Step 3: filterCannyEdge count to get closer to flag
  //std::cout << "Filter with canny edge detection" << std::endl;
  filterCannyEdgeCount(possible_flags, images, test_file);

  for (std::string x : possible_flags) {
    namedWindow("Possilibity after Edge Counting", WINDOW_NORMAL);
    resizeWindow("Possilibity after Edge Counting", images.at(x).cols, images.at(x).rows);
    imshow("Possilibity after Edge Counting", images.at(x));
    waitKey(0);
  }

  std::cout << std::endl; // Line clear

  // Step 4: Repeat steps 1 thru 3 with the upper-left quadrant of the image
  Mat ul_quadrant = test_file(Range(0, test_file.rows / 2), Range(0, test_file.cols / 2));
  ColorBucket ul_bucket = CommonColorFinder::getCommonColorBucket(ul_quadrant);
  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>> ul_flag_map;
  std::unordered_map<std::string, ColorBucket> ul_index_color_buckets;

  std::unordered_map<std::string, Mat> ul_images;
  for (std::string s : possible_flags) {
    Mat image = images.at(s);
    Mat ul_image = image(Range(0, image.rows / 2), Range(0, image.cols / 2));
    namedWindow("Quadrant Image", WINDOW_NORMAL);
    resizeWindow("Quadrant Image", ul_image.cols, ul_image.rows);
    imshow("Quadrant Image", ul_image);
    waitKey(0);
    std::pair<std::string, Mat> ul_image_pair(s, ul_image);
    ul_images.insert(ul_image_pair);
  }

  buildFlagMap(possible_flags, ul_flag_map, ul_images, ul_index_color_buckets);
  possible_flags = findClosestFlag(ul_flag_map, ul_bucket);
  filterRatios(possible_flags, ul_index_color_buckets, ul_bucket);
  filterCannyEdgeCount(possible_flags, ul_images, ul_quadrant);

  // Print out current possible flags
  std::cout << "POSSIBLE FLAGS" << std::endl;
  std::list<std::string>::iterator it2 = possible_flags.begin();
  while (it2 != possible_flags.end()) {
    std::cout << *(it2) << std::endl;
    ++it2;
  }
  std::string result_flag = possible_flags.front();
  std::string result = "Result: " + result_flag;
  namedWindow(result, WINDOW_NORMAL);
  resizeWindow(result, images.at(result_flag).cols, images.at(result_flag).rows);
  imshow(result, images.at(result_flag));
  waitKey(0);
}

/**
 * @brief main method drives the program through a series of steps in order
 *        to determine what flag is being input into the picture.
 *
 *        TENT-KEEP: The program takes in a folder of flags to create a database
 *          of flag data using color histograms.
 *        1. The program reads in a filename from the user.
 *        2. The program accesses the flag picture in the program directory.
 *        3. The program
 *
 * @param argc n/a
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {

  std::list<std::string> index_filenames = {
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

  // State names formatted
  std::string index_files3[] = {
    "Alaska", "Alabama", "Arkansas", "Arizona", "California",
    "Colorado", "Connecticut", "Delaware", "Florida", "Georgia",
    "Hawaii", "Iowa", "Idaho", "Illinois", "Indiana",
    "Kansas", "Kentucky", "Louisiana", "Massachusetts", "Maryland",
    "Maine", "Michigan", "Minnesota", "Missouri", "Mississippi",
    "Montana", "North Carolina", "North Dakota", "Nebraska", "New Hampshire",
    "New Jersey", "New Mexico", "Nevada", "New York", "Ohio",
    "Oklahoma", "Oregon", "Pennsylvania", "Rhode Island", "South Carolina",
    "South Dakota", "Tennessee", "Texas", "Utah", "Virginia",
    "Vermont", "Washington", "Wisconsin", "West Virginia", "Wyoming"
  };

  // Create vector of 50 state flag images to store files in
  // std::vector<Mat> images;
  std::unordered_map<std::string, Mat> images;

  // Read 50 flag images and store in vector images
  std::list<std::string>::iterator it = index_filenames.begin();
  while (it != index_filenames.end()) {
    std::string file_name = *it;
    Mat index_file = imread("flags/" + file_name + ".jpg");
    std::pair<std::string, Mat> index_entry(file_name, index_file);
    images.insert(index_entry);
    ++it;
  }

  // 3 Layered Map structure Stores flag names
  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>> flag_map;

  std::vector<std::string> flag_debug;

  std::unordered_map<std::string, Mat> histograms;
  for (std::string s : index_filenames) {
    std::pair<std::string, Mat> histogram_entry(s, CommonColorFinder::populateHistogram(images.at(s)));
    histograms.insert(histogram_entry);
  }

  std::unordered_map<std::string, ColorBucket> index_color_buckets;

  // flag_map maps red bucket in ints to a corresponding map of blue bucket
  // next layer maps blue bucket int to a corresponding map of green bucket
  // green bucket maps int bucket to a string
  buildFlagMap(index_filenames, flag_map, images, index_color_buckets);

  std::cout << "Type in flag to find: " << std::endl;
  std::string x;
  std::cin >> x;
  std::string filename = "";
  filename = "flags/" + x + ".jpg";
  test(flag_map, index_color_buckets, images, filename);

  //test(flag_map, index_color_buckets, images,filename);

  return 0;
}
