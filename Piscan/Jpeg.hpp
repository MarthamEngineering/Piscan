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

#ifndef ___SCANDRAID___JPEG___
#define ___SCANDRAID___JPEG___

#include "Image.hpp"

namespace scanDraiD {

/**
* Handles images in jpeg format.
* Implements the interfaces of the base class Image().
* @see Image
*/
class Jpeg : public Image
{
public:
    Jpeg();
    Jpeg(const int width, const int height);
    ~Jpeg();
    virtual int load(const char* filename);
};


} // namespace scanDraiD
#endif // ___SCANDRAID___JPEG___

