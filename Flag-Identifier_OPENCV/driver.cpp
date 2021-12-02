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
#include <string>

#include "ColorBucket.h"
#include "CommonColorFinder.h"

using namespace cv;

struct RGBHolder {
  int red;
  int blue;
  int green;
  RGBHolder() : red(0), blue(0), green(0) {}
  RGBHolder(int r, int b, int g) : red(r), blue(b), green(g) {}
};

/**
 * @brief Overlay img with pixel from background if img pixel color is within
 *        +-64 rgb color units for all channels r, g, and b
 *
 * @pre   img and background non null
 * @post  img pixels close to common color red, green, blue changed to pixel
 *        // in background img
 *
 * @param img - image to channge
 * @param background - image to take pixel from
 * @param red common red
 * @param green common green
 * @param blue common blue
 * @return new image with colors taken from corresponding images
 */
void overlayImgWithCommonColor(Mat& img, const Mat& background, int red, int green, int blue) {

  int row;
  int col;
  try {
    // Row each row of img
    for (row = 0; row < img.rows; ++row) {

      // For each column of img
      for (col = 0; col < img.cols; ++col) {

        int back_row = row;
        int back_col = col;
        if (back_row >= background.rows) {
          back_row = row % background.rows;
        }
        if (back_col >= background.cols) {
          back_col = col % background.cols;
        }

        // If pixel color in foreground image is close enough to most common
        // color (r,g,b) in foreground image, replace it with a corresponding pixel
        // From the background image
        //
        // Logic: If blue is within common blue +- 64 && 
        //           green is within common green +-64 &&
        //           red is within common red +- 64,
        //        Replace foreground img pixel with background img pixel
        if ((img.at<Vec3b>(row, col)[0] < (blue + 64) && img.at<Vec3b>(row, col)[0] > (blue - 64)) &&
            (img.at<Vec3b>(row, col)[1] < (green + 64) && img.at<Vec3b>(row, col)[0] > (green - 64)) &&
            (img.at<Vec3b>(row, col)[2] < (red + 64) && img.at<Vec3b>(row, col)[0] > (green - 64))) {

          // Copy color from background image
          img.at<Vec3b>(row, col) = background.at<Vec3b>(back_row, back_col);
        }
      }
    }
  } catch (Exception e) {
    std::cout << "Row: " << row << std::endl;
    std::cout << "Col: " << col << std::endl;
  }
  std::cout << "Exited overlay loops" << std::endl;
}


//main method
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
  //Create "vector" of images to store files in
  Mat images[50];
  //Read images and store in "vector"
  for (int i = 0; i < images.size(); i++) {
    std::string file_name = index_files[i] + ".jpg";
    Mat index_file = imread(file_name);
    images[i] = index_file;
  }
  
  //Create "vector" to store final histogram values for each index image
  //Pull images from images[] and calculate most common color histogram for each
  //Store histogram in histogram "vector"





  // // Read 2 images from the disk
  // // Image names should be "foreground.jpg" and "background.jpg"
  // Mat foreground = imread("foreground.jpg");
  // Mat background = imread("background.jpg");

  // // Create the 3D histogram of integers
  // Mat histogram = populateHistogram(foreground);
  // bin common_color_bin = mostCommonColorFinder(histogram);

  // // Get most common color for color overlay
  // int bucketSize = 64;
  // int common_red = common_color_bin.r_bucket * bucketSize + (bucketSize / 2);
  // int common_green = common_color_bin.g_bucket * bucketSize + (bucketSize / 2);
  // int common_blue = common_color_bin.b_bucket * bucketSize + (bucketSize / 2);
  // overlayImgWithCommonColor(foreground, background, common_red, common_green, common_blue);


  // // Print out and write final image for opencv methods
  // namedWindow("my-output", WINDOW_NORMAL);
  // resizeWindow("my-output", colored.cols / 3, colored.rows / 3);
  // imshow("my-output", colored);
  // imwrite("myoutput.jpg", colored);
  // waitKey(0);

  return 0;
}