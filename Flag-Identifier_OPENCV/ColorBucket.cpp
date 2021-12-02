#include "ColorBucket.h"

/**
 * @brief Constructor initializes all values to 0 and count to -
 */
ColorBucket::ColorBucket() : red_bucket_(0), green_bucket_(0), blue_bucket_(0), count_(-1) {}

/**
 * @brief Get the red dimensional bucket
 * 
 * @return red dimensional bucket 0-4
 */
int ColorBucket::getRedBucket() const {
  return this->red_bucket_;
}

/**
 * @brief Get the green dimensional bucket
 *
 * @return green dimensional bucket 0-4
 */
int ColorBucket::getGreenBucket() const {
  return this->green_bucket_;
}

/**
 * @brief Get the blue dimensional bucket
 *
 * @return blue dimensional bucket 0-4
 */
int ColorBucket::getBlueBucket() const {
  return this->blue_bucket_;
}

/**
 * @brief Get the count from the most common color in the histogram
 *
 * @return Count of the bucket that had most common color
 */
int ColorBucket::getCount() const {
  return this->count_;
}

/**
 * @brief setter for red_bucket_
 * 
 * @param bucket to set
 */
void ColorBucket::setRedBucket(int bucket) {
  this->red_bucket_ = bucket;
}

/**
 * @brief setter for green_bucket_
 * 
 * @param bucket to set
 */
void ColorBucket::setGreenBucket(int bucket) {
  this->green_bucket_ = bucket;
}

/**
 * @brief setter for blue_bucket_
 * 
 * @param bucket to set
 */
void ColorBucket::setBlueBucket(int bucket) {
  this->blue_bucket_ = bucket;
}

/**
 * @brief setter for count_
 * 
 * @param count input to set count to
 */
void ColorBucket::setCount(int count) {
  this->count_ = count;
}
