/*********************************************************************
 * @file       driver.cpp
 * @brief       driver runs a program that takes in n number of input 
 *              test images that are an image of a U.S. state flag and tries to
 *              find what the flag is based on different filters.
 *
 * @details     The program takes N input images from command line (or bat file)
 *              located in a folder flags/ that should be in the same directory
 *              as the code files.
 * 
 *              The command line takes in at least 2 arguments
 *              [program name] [number of arguments] [file1] [file2] [file3],
 *                where:
 *                -program name is the executable,
 *                -number of arguments is the number of test images to attempt
 *                -file1, file2, and file3 are the names of jpg images in the
 *                -flag directory
 *
 *              The program takes each input image and runs through filters
 *                sequentially until it comes up with the name of the image, OR
 *                it returns a number of images that are the closest.
 *
 * @author Joseph Lan
 * @author Andy Tran
 * @author Kevin Xu
 *
 * @date 2021 December 10
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
 * @param   flag_map is the multi-dimensional map containing a list of strings
 *            with state names in their appropriate bucket based on the most
 *            common color present.
 * @param   image_bucket is the color bucket for the input image.
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
          std::list<std::string> flag_list = flag_map.at(r).at(b).at(g);

          // Merges list<string> of all closest flags in adjacent buckets
          for (std::string x : flag_list) {
            result.push_back(x);
          }

          // If an adjacent bucket combination does not exist
        } catch (std::exception e2) {
          continue; // Go to next adjacent bucket
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
 * @pre   none of the parameters are null
 * @post  list is updated after applying filter
 *
 * @param list is a list of possible flags remaining
 * @param color_buckets is a map of colorbuckets for index images
 * @param image_bucket is the colorbucket for the input image
 */
void filterRatios(std::list<std::string>& list,
                  const std::unordered_map<std::string, ColorBucket>& color_buckets,
                  const ColorBucket& image_bucket) {
  if (list.size() <= 1) {
    return;
  }

  // Ratio of most common color bucket to all pixels in the image
  float image_ratio = image_bucket.getCommonColorRatio();
  const float acceptable_error = 0.006f;
  float min_ratio = image_ratio - acceptable_error;
  float max_ratio = image_ratio + acceptable_error;
  std::cout << "Test image_ratio: " << image_ratio << std::endl;
  std::cout << "min: " << min_ratio << std::endl;
  std::cout << "max: " << max_ratio << std::endl;

  std::list<std::string>::iterator it = list.begin();
  while (it != list.end()) {
    std::string s = *it;

    // Get ratio of most common color bucket for each flag in list
    float index_ratio = color_buckets.at(s).getCommonColorRatio();
    std::cout << "index_ratio: " << index_ratio << std::endl;
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
 * @brief   Returns number of edge pixels in an image through canny edge
 *          detection
 *
 * @pre     img is not null
 * @post    no change to image
 *
 * @param   img is the image to count
 * @return  count of edge pixels
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
 * @pre   list not empty, images is filled with flag templates, test_image not null
 * @post  list changed to remove flags not within range of canny edge counts
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
  const float acceptable_error = 0.006f;
  float ratio = (float)test_edge_counts / (float)num_pix;
  float min_ratio = ratio - acceptable_error;
  float max_ratio = ratio + acceptable_error;

  std::cout << "Test ratio: " << ratio << std::endl;
  std::cout << "min: " << min_ratio << std::endl;
  std::cout << "max: " << max_ratio << std::endl;

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
    std::cout << "ratio: " << edged_ratio << std::endl;
    possible_ratios.push_back(edged_ratio);
  }

  // Compare count ratio in images to count ratio for the test file
  int num_possible = (int)list.size();
  for (int i = 0; i < num_possible; ++i) {

    std::string flag = list.front();

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

/**
 * @brief Builds a 3 tier layered unordered map that stores a map of flags based
 *        on color bucket in RBG order and the string name of a flag
 * 
 * @pre   Pass in non null objects
 * @post  assigns parameter objects to values
 * 
 * @param index_files vector of flag names in directory to create metadata for
 * @param flag_map map of flag string names based on colorbucket
 * @param images map of images to use to populate flag map
 * @param index_color_buckets map of color bucket information
 */
void buildFlagMap(const std::vector<std::string>& index_files,
                  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>>& flag_map,
                  const std::unordered_map<std::string, Mat>& images,
                  std::unordered_map<std::string, ColorBucket>& index_color_buckets) {

  // flag_map maps red bucket in ints to a corresponding map of blue bucket
  // next layer maps blue bucket int to a corresponding map of green bucket
  // green bucket maps int bucket to a string
  for (std::string name : index_files) {
    // Get ColorBucket object, which holds the bucket for the most common color
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

      // Red bucket exists, check blue
    } else if (flag_map.at(current_image.getRedBucket()).find(current_image.getBlueBucket()) ==
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

      // Red and blue bucket exist, check green
    } else if (flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).find(current_image.getGreenBucket()) ==
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

      // The RBG bucket combination exists for a flag
    } else {
      flag_map.at(current_image.getRedBucket()).at(current_image.getBlueBucket()).at(current_image.getGreenBucket()).push_back(name);

    }
  }
}

/**
 * @brief Prints out list of options options in a list of strings
 * 
 * @pre options not null
 * @post no change to objects
 * 
 * @param options list of possible flags
 */
void print_options(std::list<std::string> options, std::string operation) {
  int index = 0;
  std::list<std::string>::iterator it = options.begin();

  std::cout << "Possible flags after " << operation << ": " << std::endl;
  while (it != options.end()) {
    std::cout << "[" << index << "]: " << *(it) << std::endl;
    ++it;
    ++index;
  }
}

/**
 * @brief Find flags goes through sequential algorithm steps to find a flag given
 *        a mapping of existing flags
 *          [1]: Calculates color information for test flag
 *          [2]: Narrows down possible flags based on most common color
 *          [3]: Narrows down possible flags based on MCC ratios
 *          [4]: Calculates edge information for test flags and possible flags
 *          [5]: Narrows down possible flags based on edge information
 *          [6]: Calculates quadrant test flag and possible flag information
 *          [7]: Narrowd down possible flags using quadrant information
 * 
 * @pre   No input objects null
 * @post  No change to objects
 * 
 * @param flag_map      map of flag string names based on colorbucket
 * @param color_buckets map of color bucket information
 * @param images        map of images to use to populate flag map
 * @param filename      name of the input image
 * @return a result string with the name of the determined possible flag
 */
std::list<std::string> findFlag(const std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>>& flag_map,
                 const std::unordered_map<std::string, ColorBucket>& color_buckets,
                 const std::unordered_map<std::string, Mat>& images,
                 const std::string filename) {

  std::cout << "Testing: " << filename << " in program." << std::endl;

  Mat test_file = imread(filename);

  // Print out test image
  namedWindow("Test Image", WINDOW_NORMAL);
  resizeWindow("Test Image", test_file.cols, test_file.rows);
  imshow("Test Image", test_file);
  waitKey(0);

  // ColorBucket for the input image (image we're looking for)
  ColorBucket image_bucket = CommonColorFinder::getCommonColorBucket(test_file);
  std::cout << "Test flag RBG bucket information: " << image_bucket.getRedBucket() <<
    ", " << image_bucket.getBlueBucket() << ", " << image_bucket.getGreenBucket() << std::endl;
  std::cout << std::endl; // Line clear

  // Step 1: findClosestFlag to narrow down colorBuckets
  std::string operation = "MCC filter";
  std::cout << "--" << operation << "--" << std::endl;
  std::list<std::string> possible_flags = findClosestFlag(flag_map, image_bucket);

  // Print out remaining options
  print_options(possible_flags, operation);

  // Early exit
  if (possible_flags.size() <= 1) {
    std::cout << "Result found after " << operation << "." << std::endl;
    return possible_flags;
  }
  std::cout << std::endl; // Line break

  // Step 2: filterRatios to get closer to flag
  operation = "MCC Ratio Filter";
  std::cout << "--" << operation << "--" << std::endl;
  filterRatios(possible_flags, color_buckets, image_bucket);

  // Print out remaining options
  print_options(possible_flags, operation);

  // Early exit
  if (possible_flags.size() <= 1) {
    std::cout << "Result found after " << operation << "." << std::endl;
    return possible_flags;
  }
  std::cout << std::endl; // Line break

  // Resize for img dims
  float change = (float)test_file.rows / 240;
  float width = (float)test_file.cols / change;
  Size resizing((int)width, 240);
  resize(test_file, test_file, resizing, INTER_LINEAR);

  // Step 3: filterCannyEdge count to get closer to flag
  //std::cout << "Filter with canny edge detection" << std::endl;
  operation = "Canny Edge Filter";
  std::cout << "--" << operation << "--" << std::endl;
  filterCannyEdgeCount(possible_flags, images, test_file);

  // Print out remaining options
  print_options(possible_flags, operation);

  // Early exit
  if (possible_flags.size() <= 1) {
    std::cout << "Result found after " << operation << "." << std::endl;
    return possible_flags;
  }
  std::cout << std::endl; // Line break

  // Step 4: Repeat steps 1 thru 3 with the upper-left quadrant of the image
  operation = "Upper Left Quadrant modular filter";
  std::cout << "--" << operation << "--" << std::endl;

  // Upper left image
  Mat ul_quadrant = test_file(Range(0, test_file.rows / 2), Range(0, test_file.cols / 2));

  // Upper left color bucket
  ColorBucket ul_bucket = CommonColorFinder::getCommonColorBucket(ul_quadrant);

  // New maps for upper left quadrants
  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>> ul_flag_map;
  std::unordered_map<std::string, ColorBucket> ul_index_color_buckets;
  std::unordered_map<std::string, Mat> ul_images;
  std::vector<std::string> poss_flags;

  // Create upper left metadata
  for (std::string s : possible_flags) {
    poss_flags.push_back(s);
    Mat image = images.at(s);
    Mat ul_image = image(Range(0, image.rows / 2), Range(0, image.cols / 2));
    std::pair<std::string, Mat> ul_image_pair(s, ul_image);
    ul_images.insert(ul_image_pair);
  }

  // Build map with metadata
  buildFlagMap(poss_flags, ul_flag_map, ul_images, ul_index_color_buckets);

  // Filtered search through upper left quadrant
  possible_flags = findClosestFlag(ul_flag_map, ul_bucket);
  std::cout << "Size of possible flags after MCC: " << possible_flags.size() << std::endl;

  // Early exit if possiblity found
  if (possible_flags.size() == 1) {
    std::cout << "Result found after " << operation << "." << std::endl;
    return possible_flags;
  }
  
  //Filter out the flags based on the correct color ratio
  filterRatios(possible_flags, ul_index_color_buckets, ul_bucket);
  std::cout << "Size of possible flags after MCC ratio: " << possible_flags.size() << std::endl;

  // Early exit
  if (possible_flags.size() == 1) {
    std::cout << "Result found after " << operation << "." << std::endl;
    return possible_flags;
  }

  //Filter out possible flags based on the canny edge algorithm
  filterCannyEdgeCount(possible_flags, ul_images, ul_quadrant);
  std::cout << "Size of possible flags after Edge Ratio: " << possible_flags.size() << std::endl;
  std::cout << "Result found after " << operation << "." << std::endl;

  return possible_flags;
}

/**
 * @brief main method drives the program through a series of steps in order
 *        to determine what flag is being input into the picture.
 *
 *        The program takes in a folder of flags to create a database
 *          of flag data using color histograms.
 *        1. The program reads in number of test files to test and the names
 *            of those test files.
 *        2. The program tests each input test image against the metadata
 *            in the program created database.
 *        3. The program outputs the output image and name of the state flag
 *            or a list of the cloest flags
 *
 * @param argc number of files to test
 * @param argv array containing the names of the files to be tested
 * @return
 */
int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cout << "Minimum number of arguments: 3" << std::endl;
    std::cout << "<number of files N to test> <file 1> <file 2> ... <file N>" << std::endl;
    return 0;
  }

  // file names
  std::vector<std::string> index_filenames = {
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

  // Map of file names to images for creating image metadata
  std::unordered_map<std::string, Mat> images;

  // Read 50 flag images and store map
  for (std::string x : index_filenames) {
    Mat index_file = imread("flags/" + x + ".jpg");
    std::pair<std::string, Mat> index_entry(x, index_file);
    images.insert(index_entry);
  }

  // 3 Layered Map structure Stores flag names
  std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::list<std::string>>>> flag_map;

  // Histograms of 50 flag images for image metadata
  std::unordered_map<std::string, Mat> histograms;
  for (std::string s : index_filenames) {
    std::pair<std::string, Mat> histogram_entry(s, CommonColorFinder::populateHistogram(images.at(s)));
    histograms.insert(histogram_entry);
  }

  // Map of color buckets created for flag metadata
  std::unordered_map<std::string, ColorBucket> index_color_buckets;

  // flag_map maps red bucket in ints to a corresponding map of blue bucket
  // next layer maps blue bucket int to a corresponding map of green bucket
  // green bucket maps int bucket to a string
  buildFlagMap(index_filenames, flag_map, images, index_color_buckets);

  //Check to see if we have valid input, else throw an error
  unsigned int num_args = -1;
  try {
    num_args = atoi(argv[1]);
  } catch (std::exception e) {
    std::cout << "Error in arguments: \"" << argv[1] << "\" cannot be converted into an integer." << std::endl;
    return 0;
  }
  //Create a place to store our file names
  std::vector<std::string> test_file_names;

  // Convert character array input arguments to double
  for (unsigned int i = 2; i <= num_args + 1; ++i) {  
    std::string val(argv[i]);
    test_file_names.push_back(val);
  };

  // Loop findFlag num_args times
  for (unsigned int i = 0; i < num_args; ++i) {

    // test file name
    std::string x = test_file_names.at(i);
    std::string filename = "";
    filename = "flags/" + x + ".jpg";

    // Algorithmic runner
    std::list<std::string> flag_result = findFlag(flag_map, index_color_buckets, images, filename);

    // No matches found
    if (flag_result.size() == 0) {
      std::cout << "No matches found for \"" << filename << "\"" << std::endl;
      waitKey(0);
      continue;
    }

    for (std::string flag_name : flag_result) {
      // Result printer
      std::string result = "Result: " + flag_name;
      std::cout << "The flag in the image is from: " << flag_name << std::endl;

      // Show result flag
      namedWindow(result, WINDOW_NORMAL);
      resizeWindow(result, images.at(flag_name).cols, images.at(flag_name).rows);
      imshow(result, images.at(flag_name));
      waitKey(0);
    }
  }

  return 0;
}