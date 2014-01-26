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

#include <libintl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern "C"
{
#include <jpeglib.h>
#include <jerror.h>
}
#include "Jpeg.hpp"

namespace scanDraiD {

Jpeg::Jpeg()
    : Image()
{;}

Jpeg::Jpeg(const int width, const int height)
    : Image(width, height)
{;}

Jpeg::~Jpeg()
{;}

int Jpeg::load(const char * filename)
{
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr         jerr;

    FILE       *fd ;
    JSAMPARRAY  linebuffer;
    int         row_stride;

    if(( fd  = fopen(filename, "rb")) == NULL)
    {
        /** @todo throw exception here ! */
        perror (gettext("readJPEG"));
        fprintf(stderr, gettext("readJPEG: Can't open %s for reading\n"), filename);
        return 0;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress    (&cinfo);
    jpeg_stdio_src            (&cinfo, fd);
    jpeg_read_header          (&cinfo, TRUE);
    jpeg_start_decompress     (&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;

    if (cinfo.output_components != 3)
    {
        fprintf (stderr, gettext("readJPEG: %s has %d components?!?\n"), filename, cinfo.output_components);
        return 0;
    }

    linebuffer = (*cinfo.mem->alloc_sarray)
              ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    delete buffer_;

    height_ = cinfo.output_height;
    width_  = cinfo.output_width;
    buffer_ = new unsigned char [width_ * height_ * 3];

    while (cinfo.output_scanline < cinfo.output_height)
    {
        int y = cinfo.output_scanline;
        /*
          jpeg_read_scanlines expects an array of pointers to scanlines.
          Here the array is only one element long, but you could ask for
          more than one scanline at a time if that's more convenient.
        */
        jpeg_read_scanlines(&cinfo, linebuffer, 1);
        memcpy (&buffer_[y * row_stride], linebuffer[0], row_stride);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fd);
    return 1;
}

} // namespace scanDraiD

