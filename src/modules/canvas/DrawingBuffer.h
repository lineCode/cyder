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

#ifndef CYDER_DRAWINGBUFFER_H
#define CYDER_DRAWINGBUFFER_H

#include <skia.h>
#include "modules/image/Image.h"

namespace cyder {

    class DrawingBuffer {
    public:

        virtual ~DrawingBuffer() {}

        /**
         * Indicates the width of the DrawingBuffer, in pixels.
         */
        virtual int width() const = 0;

        virtual void setWidth(int value) = 0;

        /**
         * Indicates the height of the DrawingBuffer, in pixels.
         */
        virtual int height() const = 0;

        virtual void setHeight(int value) = 0;

        /**
         * Return a canvas that will draw into this drawing buffer.
         * Note: Do not cache the return value of surface(), it may change when DrawingBuffer resizes.
         */
        virtual SkCanvas* getCanvas() = 0;

        /**
         * Draws this buffer directly into another canvas.
         */
        virtual void draw(SkCanvas* canvas, SkScalar x, SkScalar y, const SkPaint* paint) = 0;

        /**
         * Returns an image of the current state of the buffer pixels up to this point. Subsequent changes to the buffer
         * will not be reflected in this image.
         */
        virtual Image* makeImageSnapshot() = 0;
    };

}

#endif //CYDER_DRAWINGBUFFER_H
