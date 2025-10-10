#include "rtimage.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

rtwimage::rtwimage(const char *image_filename) {
  // Loads image data from the specified file. If the RTW_IMAGES environment
  // variable is defined, looks only in that directory for the image file. If
  // the image was not found, searches for the specified image file first from
  // the current directory, then in the images/ subdirectory, then the
  // _parent's_ images/ subdirectory, and then _that_ parent, on so on, for
  // six levels up. If the image was not loaded successfully, width() and
  // height() will return 0.

  auto filename = std::string(image_filename);
  auto imagedir = getenv("RTW_IMAGES");

  // Hunt for the image file in some likely locations.
  if (imagedir && load(std::string(imagedir) + "/" + image_filename))
    return;
  if (load(filename))
    return;
  if (load("images/" + filename))
    return;
  if (load("../images/" + filename))
    return;
  if (load("../../images/" + filename))
    return;
  if (load("../../../images/" + filename))
    return;
  if (load("../../../../images/" + filename))
    return;
  if (load("../../../../../images/" + filename))
    return;
  if (load("../../../../../../images/" + filename))
    return;

  std::clog << "ERROR: Could not load image file '" << image_filename << "'.\n";
}
rtwimage::~rtwimage() {
  delete[] bdata;
  STBI_FREE(fdata);
}

bool rtwimage::load(const std::string &filename) {
  // Loads the linear (gamma=1) image data from the given file name. Returns
  // true if the load succeeded. The resulting data buffer contains the three
  // [0.0, 1.0] floating-point values for the first pixel (red, then green,
  // then blue). Pixels are contiguous, going left to right for the width of
  // the image, followed by the next row below, for the full height of the
  // image.

  auto n =
      bytes_per_pixel; // Dummy out parameter: original components per pixel
  fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n,
                     bytes_per_pixel);
  if (fdata == nullptr)
    return false;

  bytes_per_scanline = image_width * bytes_per_pixel;
  convert_to_bytes();
  return true;
}
const unsigned char *rtwimage::pixel_data(int x, int y) const {
  // Return the address of the three RGB bytes of the pixel at x,y. If there
  // is no image data, returns magenta.
  static unsigned char magenta[] = {255, 0, 255};
  if (bdata == nullptr)
    return magenta;

  x = clamp(x, 0, image_width);
  y = clamp(y, 0, image_height);

  return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
}
