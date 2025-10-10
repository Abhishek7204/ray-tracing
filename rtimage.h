#ifndef RTW_STB_IMAGE_H
#define RTW_STB_IMAGE_H

#include "stb_image.h"

#include <cstdlib>
#include <iostream>

class rtwimage {
public:
  rtwimage() {}

  rtwimage(const char *image_filename);

  ~rtwimage();

  bool load(const std::string &filename);

  int width() const { return (fdata == nullptr) ? 0 : image_width; }
  int height() const { return (fdata == nullptr) ? 0 : image_height; }

  const unsigned char *pixel_data(int x, int y) const;

private:
  const int bytes_per_pixel = 3;
  float *fdata = nullptr;         // Linear floating point pixel data
  unsigned char *bdata = nullptr; // Linear 8-bit pixel data
  int image_width = 0;            // Loaded image width
  int image_height = 0;           // Loaded image height
  int bytes_per_scanline = 0;

  static int clamp(int x, int low, int high) {
    // Return the value clamped to the range [low, high).
    if (x < low)
      return low;
    if (x < high)
      return x;
    return high - 1;
  }

  static unsigned char float_to_byte(float value) {
    if (value <= 0.0)
      return 0;
    if (1.0 <= value)
      return 255;
    return static_cast<unsigned char>(256.0 * value);
  }

  void convert_to_bytes() {
    // Convert the linear floating point pixel data to bytes, storing the
    // resulting byte data in the `bdata` member.

    int total_bytes = image_width * image_height * bytes_per_pixel;
    bdata = new unsigned char[total_bytes];

    // Iterate through all pixel components, converting from [0.0, 1.0] float
    // values to unsigned [0, 255] byte values.

    auto *bptr = bdata;
    auto *fptr = fdata;
    for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
      *bptr = float_to_byte(*fptr);
  }
};

#endif
