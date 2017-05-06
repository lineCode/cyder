//////////////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017-present, cyder.org
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//      The above copyright notice and this permission notice shall be included in all
//      copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef CYDER_IMAGE_H
#define CYDER_IMAGE_H

#include <skia.h>
#include "ImageFormat.h"

namespace cyder {

    /**
     * A wrapper for SkImage.
     */
    class Image {
    public:
        static Image* Decode(const void* bytes, size_t length);
        explicit Image(SkImage* pixels);
        ~Image();

        SkImage* pixels() const {
            return _pixels;
        }

        SkIRect* subset() const {
            return _subset;
        }

        int width() const {
            if (_subset) {
                return _subset->width();
            }
            return _pixels->width();
        }

        int height() const {
            if (_subset) {
                return _subset->height();
            }
            return _pixels->height();
        }

        bool transparent() const {
            return !_pixels->isOpaque();
        }

        /**
         * Encode the image's pixels and return the result as a new SkData, which the caller must manage (i.e. call
         * unref() when they are done).
         *
         * If the image type cannot be encoded, or the requested encoder type is not supported, this will return nullptr.
         */
        SkData* encode(ImageFormat type, double quality);

        /**
         * Returns a new image that is a subset of this image. The underlying implementation may share the pixels, or it
         * may make a copy. It depends on the value passed in the sharePixels parameter.
         * @param x The x coordinate of the upper left corner of the subset rectangle.
         * @param y The y coordinate of the upper left corner of the subset rectangle.
         * @param width The width of the subset rectangle.
         * @param height The height of the subset rectangle.
         * @param sharePixels Indicates whether the underlying implementation should share the pixels, otherwise make a
         * copy. The default value is true.
         * @returns If subset does not intersect the bounds of this image, or the copy/share cannot be made, nullptr will
         * be returned.
         */
        Image* makeSubset(int x, int y, int width, int height, bool sharePixels = true);

        /**
         * Reads a rectangular region of the bitmap's pixels into the specified buffer. Writes four uint8 values (RGBA)
         * for each pixel into the buffer. All pixels in the rectangular region are read as unmultiplied color values.
         * Returns false if this call fails.
         */
        bool readPixels(void* buffer, int x, int y, int width, int height);

    private:
        SkImage* _pixels;
        SkIRect* _subset;

        Image(SkImage* image, const SkIRect& subset);
    };

}

#endif //CYDER_IMAGE_H