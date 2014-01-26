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

