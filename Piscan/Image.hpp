/*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 3 as published by
* the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU LESSER GENERAL PUBLIC LICENSE Version 3 for more details.
*
* Changelog:
*
* 2007 The original source was created by Steve Baker:
*    http://www.sjbaker.org/wiki/index.php?title=A_Simple_3D_Scanner
* 2009-02-20 <a.w@quirxi.net>:
*   Rewrite of code so that it can be used as a command line tool.
*/

#ifndef ___SCANDRAID___IMAGE___
#define ___SCANDRAID___IMAGE___

namespace scanDraiD {

/**
* Base class for all classes that have to read/write images.
* Forces derived classes to use this interface.
*/
class Image
{
public:
    Image ();
    Image (const int width, const int height);
    virtual ~Image ();
    virtual int load(const char *fname)=0;

    int getWidth() const;
    int getHeight() const;
    unsigned char* getPixels() const;
    unsigned int getPixelArea(const float x1, const float y1,
                              const float x2, const float y2,
                              const unsigned int keyColour) const;
    unsigned char getPixelRed(const int x, const int y) const;
    unsigned char getPixelGreen(const int x, const int y) const;
    unsigned char getPixelBlue(const int x, const int y) const;
    unsigned int getPixel(const float x, const float y) const;
    unsigned int getPixel(const int x, const int y) const;
    void setPixel(float x, float y, unsigned int rgba);
    void setPixel(int x, int y, unsigned int rgba);

protected:
    int width_;
    int height_;
    unsigned char *buffer_;
};


} // namespace scanDraiD
#endif // ___SCANDRAID___IMAGE___
