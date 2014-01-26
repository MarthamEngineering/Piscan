/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <math.h>
#include "image.h"

namespace scanDraiD {

Image::Image()
    : width_(0), height_(0), buffer_(0)
{
}

Image::Image(const int width, const int height)
    : width_(width), height_(height), buffer_(new unsigned char[width*height*3])
{
}

Image::~Image()
{
    delete[] buffer_;
}

int Image::getWidth() const
{
    return width_;
}

int Image::getHeight() const
{
    return height_;
}

unsigned char* Image::getPixels () const
{
    return buffer_;
}

unsigned int Image::getPixelArea(const float x1, const float y1,
                                 const float x2, const float y2,
                                 const unsigned int keyColour) const
{
    if (x2-x1 <= 0.0 || y2-y1 <= 0.0)
        return 0x00000000;

  float area_tot = (x2-x1) * (y2-y1);
  float r_tot = 0.0;
  float g_tot = 0.0;
  float b_tot = 0.0;
  float rgb_area = 0.0;

  for (int i = static_cast<int>(floor(x1)); i <= static_cast<int>(ceil(x2)); ++i)
    for (int j = static_cast<int>(floor(y1)); j <= static_cast<int>(ceil(y2)); ++j)
    {
      if(i < 0 || i >= width_ || j < 0 || j >= height_ || getPixel(i, j) == keyColour)
        continue;

      float xa =(x1 > static_cast<float>(i))   ? x1 : static_cast<float>(i);
      float xb =(x2 < static_cast<float>(i+1)) ? x2 : static_cast<float>(i+1);
      float ya =(y1 > static_cast<float>(j))   ? y1 : static_cast<float>(j);
      float yb =(y2 < static_cast<float>(j+1)) ? y2 : static_cast<float>(j+1);

      if(xb-xa <= 0.0 || yb-ya <= 0.0)
        continue;

      float area = (xb-xa) * (yb-ya);

      rgb_area += area;

      r_tot += getPixelRed  (i, j) * area;
      g_tot += getPixelGreen(i, j) * area;
      b_tot += getPixelBlue (i, j) * area;
    }

  if(rgb_area <= 0.0)
    return 0x00000000;

  r_tot /= rgb_area;
  g_tot /= rgb_area;
  b_tot /= rgb_area;

  float a_tot = rgb_area * 255.0 / area_tot;

  if(r_tot > 255.0) r_tot = 255.0;
  if(g_tot > 255.0) g_tot = 255.0;
  if(b_tot > 255.0) b_tot = 255.0;
  if(a_tot > 255.0) a_tot = 255.0;

  if(r_tot <= 0.0) r_tot = 0.0;
  if(g_tot <= 0.0) g_tot = 0.0;
  if(b_tot <= 0.0) b_tot = 0.0;
  if(a_tot <= 0.0) a_tot = 0.0;

  return((unsigned int) r_tot << 24) +
        ((unsigned int) g_tot << 16) +
        ((unsigned int) b_tot << 8 ) +
        ((unsigned int) a_tot << 0);
}


unsigned char Image::getPixelRed(const int x, const int y) const
{
    return (unsigned int) buffer_[(y * width_ + x) * 3 + 0];
}

unsigned char Image::getPixelGreen(const int x, const int y) const
{
    return (unsigned int) buffer_[(y * width_ + x) * 3 + 1];
}

unsigned char Image::getPixelBlue(const int x, const int y) const
{
    return (unsigned int) buffer_[(y * width_ + x) * 3 + 2];
}

unsigned int Image::getPixel(const float x, const float y) const
{
    return getPixel(static_cast<int>(x), static_cast<int>(y));
}

unsigned int Image::getPixel(const int x, const int y) const
{
    int p = (y * width_ + x) * 3;
    return((unsigned int) buffer_[p + 0] << 24) +
          ((unsigned int) buffer_[p + 1] << 16) +
          ((unsigned int) buffer_[p + 2] <<  8) + 255;
}

/**
* @todo is float as parameter here apropriate ?
*/
void Image::setPixel(const float x, const float y, const unsigned int rgba)
{
    setPixel(static_cast<int>(x), static_cast<int>(y), rgba);
}

void Image::setPixel(const int x, const int y, const unsigned int rgba)
{
    int p = (y * width_ + x) * 3;

    if(( rgba & 0xFF) == 0)
      return;

    if(( rgba & 0xFF) == 255)
    {
        buffer_[p + 0] = (rgba >> 24) & 0xFF;
        buffer_[p + 1] = (rgba >> 16) & 0xFF;
        buffer_[p + 2] = (rgba >>  8) & 0xFF;
    }
    else
    {
        unsigned int r = (rgba >> 24) & 0xFF;
        unsigned int g = (rgba >> 16) & 0xFF;
        unsigned int b = (rgba >>  8) & 0xFF;
        unsigned int a = (rgba >>  0) & 0xFF;
        unsigned int ac = 255 - a;

        buffer_[p + 0] = (unsigned char)((int)(buffer_[p + 0]) * ac / 255 + r * a / 255);
        buffer_[p + 1] = (unsigned char)((int)(buffer_[p + 1]) * ac / 255 + g * a / 255);
        buffer_[p + 2] = (unsigned char)((int)(buffer_[p + 2]) * ac / 255 + b * a / 255);
    }
}
} // namespace scanDraiD
