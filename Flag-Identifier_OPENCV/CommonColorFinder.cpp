#include "CommonColorFinder.h"

/**
 * @brief Default constructor is private and doesn't allow calling
 */
CommonColorFinder::CommonColorFinder() {
  // Do nothing
}

/**
 * @brief Returns a ColorBucket of RBG space with 4 dimensions from a histrogram
 *        of the input image
 * 
 * @param img Image to create histrogram and generate color bucket from
 * @return ColorBucket representing red,blue,green bucket with most counts
 */
ColorBucket CommonColorFinder::getCommonColorBucket(const Mat& img) {
  return findMostCommonColor(populateHistogram(img));
}

/**
 * @brief Creates a histogram for a given image with 4x4x4 dimensions
 * 
 * @param img Input image to test
 * @return 
 */
Mat CommonColorFinder::populateHistogram(const Mat& img){
  // Create histogram picture
  // Create an array of the histogram dimensions
  // Size is a constant - the # of buckets in each dimension
  int dims[] = { 4, 4, 4 };

  // Create 3D histogram of integers initialized to 0
  Mat histogram(3, dims, CV_32S, Scalar::all(0));

  // Histogram with 4 buckets
  // Each bucket has size 64
  // 0 - 63
  // 64 - 127
  // 128 - 191
  // 192 - 255
  int bucket_size = 256 / 4;

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
 * @brief Returns a ColorBucket object that contains the most common color
 *        bucket for the given histogram
 * 
 * @param img Histogram to test on
 * @return Most common color bucket object
 */
ColorBucket CommonColorFinder::findMostCommonColor(const Mat& img) {
  
  // Store max bucket information
  ColorBucket max;

  // Loop through red buckets
  for (int r = 0; r < 4; ++r) {

    // Loop through green buckets
    for (int b = 0; b < 4; ++b) {

      // Loop through blue buckets
      for (int g = 0; g < 4; ++g) {

        if (img.at<int>(r, g, b) > max.getCount()) {
          max.setRedBucket(r);
          max.setGreenBucket(g);
          max.setBlueBucket(b);
          max.setCount(img.at<int>(r, g, b));
        }
      }
    }
  }
  return max;
}