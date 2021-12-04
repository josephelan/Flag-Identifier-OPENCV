/*********************************************************************
 * @file       ColorBucket.h
 * @brief      ColorBucket holds histogram bucket information for a given image
 *              for its RBG values.
 *
 * @author Joseph Lan
 *
 * @date 2021 December 4
 *
 * FLAG IDENTIIFIER
 * CSS 487 Final Project
 * Prof. Clark Olson
 *********************************************************************/
#pragma once


/**
 * @class ColorBucket holds histogram bucket information for a given image for 
 *        RBG values. It also holds the count of the most common bucket and its
 *        ratio to the total pixels in the image
 */
class ColorBucket {
  public:

  /**
 * @brief Constructor initializes all values to 0 and count to -
 */
  ColorBucket();

  /**
 * @brief Get the red dimensional bucket
 *
 * @return red dimensional bucket 0-4
 */
  int getRedBucket() const;

  /**
 * @brief Get the green dimensional bucket
 *
 * @return green dimensional bucket 0-4
 */
  int getGreenBucket() const;

  /**
 * @brief Get the blue dimensional bucket
 *
 * @return blue dimensional bucket 0-4
 */
  int getBlueBucket() const;

  /**
 * @brief Get the count from the most common color in the histogram
 *
 * @return Count of the bucket that had most common color
 */
  int getCount() const;

  /**
 * @brief getter for most common color ratio
 * @return mostCommonColorRatio
 */
  float getCommonColorRatio() const;

  /**
 * @brief setter for red_bucket_
 *
 * @param bucket to set
 */
  void setRedBucket(int bucket);

  /**
 * @brief setter for green_bucket_
 *
 * @param bucket to set
 */
  void setGreenBucket(int bucket);

  /**
 * @brief setter for blue_bucket_
 *
 * @param bucket to set
 */
  void setBlueBucket(int bucket);

  /**
 * @brief setter for count_
 *
 * @param count input to set count to
 */
  void setCount(int count);

  /**
 * @brief Setter for most common color ratio
 * @param ratio to set data member to
 */
  void setCommonColorRatio(float ratio);

  private:

  // Bucket information for an image where each bucket holds the bucket of
  // The most common RBG values, count holds the count of that bucket, and
  // ratio holds the ratio of that bucket to the total pixels int the image
  int red_bucket_;
  int green_bucket_;
  int blue_bucket_;
  int count_;
  float mostCommonColorRatio_;
};