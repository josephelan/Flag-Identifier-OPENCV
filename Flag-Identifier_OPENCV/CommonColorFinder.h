#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ColorBucket.h"

using namespace cv;

/**
   * @brief RGBHolder is a struct which holds a unit of RGB values
   */
struct RGBHolder {
  int red;
  int blue;
  int green;
  RGBHolder() : red(0), blue(0), green(0) {}
  RGBHolder(int r, int g, int b) : red(r), blue(b), green(g) {}
};

/**
 * @brief 
 */
class CommonColorFinder {

  public:
  
  static RGBHolder getCommonColor(const ColorBucket);

  /**
   * @brief Returns a ColorBucket of RBG space with 4 dimensions from a histrogram
   *        of the input image
   *
   * @param img Image to create histrogram and generate color bucket from
   * @return ColorBucket representing red,blue,green bucket with most counts
   */
  static ColorBucket getCommonColorBucket(const Mat& img);

  /**
   * @brief Creates a histogram for a given image with 4x4x4 dimensions
   *
   * @param img Input image to test
   * @return
   */
  static Mat populateHistogram(const Mat& img);

  private:

  /**
   * @brief Returns a ColorBucket of RBG space with 4 dimensions from a histrogram
   *        of the input image
   *
   * @param img Image to create histrogram and generate color bucket from
   * @return ColorBucket representing red,blue,green bucket with most counts
   */
  static ColorBucket findMostCommonBucket(const Mat& img);

  /**
   * @brief Default constructor is private and doesn't allow calling
   */
  CommonColorFinder();
};