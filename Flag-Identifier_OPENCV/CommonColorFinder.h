#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ColorBucket.h"

using namespace cv;

/**
 * @brief 
 */
class CommonColorFinder {
  public:

  /**
   * @brief Returns a ColorBucket of RBG space with 4 dimensions from a histrogram
   *        of the input image
   *
   * @param img Image to create histrogram and generate color bucket from
   * @return ColorBucket representing red,blue,green bucket with most counts
   */
  static ColorBucket getCommonColorBucket(const Mat& img);
  
  private:

  /**
   * @brief Creates a histogram for a given image with 4x4x4 dimensions
   *
   * @param img Input image to test
   * @return
   */
  static Mat populateHistogram(const Mat& img);

  /**
   * @brief Returns a ColorBucket of RBG space with 4 dimensions from a histrogram
   *        of the input image
   *
   * @param img Image to create histrogram and generate color bucket from
   * @return ColorBucket representing red,blue,green bucket with most counts
   */
  static ColorBucket findMostCommonColor(const Mat& img);

  /**
   * @brief Default constructor is private and doesn't allow calling
   */
  CommonColorFinder();
};