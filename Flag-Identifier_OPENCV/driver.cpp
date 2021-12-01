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
using namespace cv;

/**
 * @brief Contains bucket information and value (number of votes) in that bucket
 */
struct bin {
  int r_bucket;
  int g_bucket;
  int b_bucket;
  int value;

  // Initialize bin to 0
  bin() : r_bucket(0), g_bucket(0), b_bucket(0), value(0) {}
};

/**
 * @brief Returns a histogram with 3 dimensions and 4 buckets, resulting in
 *        64 bins to determine color patterns in the image.
 *
 * @pre   img not null
 * @post  No change to input
 *
 * @param img image to create histogram for
 * @return histogram with 3 dimensions and 4 buckets, where each bucket size is
 *          64 (0-63, 64-127, 128-191, 192-255) in rgb dimensions.
 */
Mat populateHistogram(const Mat& img) {

  // Use color histogram to find the most common color in the foreground image
  // Create an array of the histogram dimensions
  // Size is a constant - the # of buckets in each dimension
  int dims[] = { 4, 4, 4 };

  // Create 3D histogram of integers initialized to 0
  Mat histogram(3, dims, CV_32S, Scalar::all(0));

  // Histogram with 4 buckets
  // Each bucket has size 64
  // 0 - 63, 64 - 127, 128 - 191, 192 - 255
  int bucket_size = 256 / 4; // 64 (Size of 1 bucket)

  // Access each pixel and assign them to the histogram
  for (int row = 0; row < img.rows; ++row) {

    for (int col = 0; col < img.cols; ++col) {

      // Decides which bucket to increment in histogram
      // Will result in 0, 1, 2, or 3 respectively for buckets:
      // 0 - 63, 54 - 127, 128 - 191, 192 - 255
      int blue_bucket = img.at<Vec3b>(row, col)[0] / bucket_size;
      int green_bucket = img.at<Vec3b>(row, col)[1] / bucket_size;
      int red_bucket = img.at<Vec3b>(row, col)[2] / bucket_size;

      // Increment the count at the calculated buckets for histogram
      ++histogram.at<int>(red_bucket, green_bucket, blue_bucket);
    }
  }
  return histogram;
}

/**
 * @brief Finds the bucket r_bucket, g_bucket, b_bucket signifying the most
 *        common color given a histogram img.
 *
 * @pre   img not null
 * @post  no change to img. returns a bin object
 *
 * @param img
 * @return bin object with values initialized to the bucket r_bucket, g_bucket,
 *              blue_bucket, and value showing integer which is the number
 *              of counts at that bin.
 */
bin mostCommonColorFinder(const Mat& img) {

  // Store max bucket information
  bin max;

  // Loop through red buckets
  for (int r = 0; r < 4; ++r) {

    // Loop through green buckets
    for (int b = 0; b < 4; ++b) {

      // Loop through blue buckets
      for (int g = 0; g < 4; ++g) {

        if (img.at<int>(r, g, b) > max.value) {
          max.r_bucket = r;
          max.g_bucket = g;
          max.b_bucket = b;
          max.value = img.at<int>(r, g, b);
        }
      }
    }
  }
  return max;
}

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

int main(int argc, char* argv[]) {

  // Read 2 images from the disk
  // Image names should be "foreground.jpg" and "background.jpg"
  Mat foreground = imread("foreground.jpg");
  Mat background = imread("background.jpg");

  // Create the 3D histogram of integers
  Mat histogram = populateHistogram(foreground);
  bin common_color_bin = mostCommonColorFinder(histogram);

  // Get most common color for color overlay
  int bucketSize = 64;
  int common_red = common_color_bin.r_bucket * bucketSize + (bucketSize / 2);
  int common_green = common_color_bin.g_bucket * bucketSize + (bucketSize / 2);
  int common_blue = common_color_bin.b_bucket * bucketSize + (bucketSize / 2);
  overlayImgWithCommonColor(foreground, background, common_red, common_green, common_blue);

  // Print out overlaid image
  namedWindow("overlay", WINDOW_NORMAL);
  resizeWindow("overlay", foreground.cols / 4, foreground.rows / 4);
  imshow("overlay", foreground);
  imwrite("overlay.jpg", foreground);
  waitKey(0);

  // FLIPPING, GREYSCALE, BLUR---------------------------------------------

  // Flip image
  Mat flipped;
  flip(background, flipped, 1);

  // Convert image to greyscale
  Mat greyed;
  cvtColor(flipped, greyed, COLOR_BGR2GRAY, 1);

  // Blue image
  Mat blurred;
  GaussianBlur(greyed, blurred, Size(7, 7), 2.0, 2.0);

  Mat edged;
  Canny(blurred, edged, 20, 60);

  //Printing out and writing output.jpg image
  namedWindow("output");
  imshow("output", edged);
  imwrite("output.jpg", edged);
  waitKey(0);

  // TRANSPOSE, REPEAT, COLORMAP------------------------------------------

  // Transpose image
  Mat transposed;
  transpose(background, transposed);

  // Repeat
  Mat repeated;
  repeat(transposed, 2, 3, repeated);

  // Colormap
  Mat colored;
  applyColorMap(repeated, colored, COLORMAP_TURBO);

  // Print out and write final image for opencv methods
  namedWindow("my-output", WINDOW_NORMAL);
  resizeWindow("my-output", colored.cols / 3, colored.rows / 3);
  imshow("my-output", colored);
  imwrite("myoutput.jpg", colored);
  waitKey(0);

  return 0;
}