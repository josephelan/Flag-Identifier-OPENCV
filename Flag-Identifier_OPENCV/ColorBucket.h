#pragma once
class ColorBucket {
  public:
  ColorBucket();

  int getRedBucket() const;
  int getGreenBucket() const;
  int getBlueBucket() const;
  int getCount() const;

  void setRedBucket(int bucket);
  void setGreenBucket(int bucket);
  void setBlueBucket(int bucket);
  void setCount(int count);

  private:
  int red_bucket_;
  int green_bucket_;
  int blue_bucket_;
  int count_;
};