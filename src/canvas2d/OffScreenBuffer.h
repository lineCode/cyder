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

#ifndef CYDER_OFFSCREENBUFFER_H
#define CYDER_OFFSCREENBUFFER_H

#include "display/DrawingBuffer.h"
#include <skia.h>

namespace cyder {

    class OffScreenBuffer : public DrawingBuffer {
    public:
        OffScreenBuffer(int width, int height, bool alpha = true, bool useGPU = true);

        ~OffScreenBuffer() override;

        int width() const override {
            return _width;
        }

        void setWidth(int value) override {
            if(value < 0){
                return;
            }
            sizeChanged = true;
            _width = value;
        }

        int height() const override {
            return _height;
        }

        void setHeight(int value) override {
            if(value < 0){
                return;
            }
            sizeChanged = true;
            _height = value;
        }

        SkCanvas* getCanvas() override;

        void draw(SkCanvas* canvas, SkScalar x, SkScalar y, const SkPaint* paint) override;

        bool readPixels(const SkImageInfo& dstInfo, void* dstPixels, size_t dstRowBytes, int srcX, int srcY) override;

    private:
        int _width;
        int _height;
        bool useGPU;
        bool alpha;
        bool sizeChanged = true;
        bool contentChanged = false;
        SkSurface* surface = nullptr;

        SkSurface* getSurface();
    };

}

#endif //CYDER_OFFSCREENBUFFER_H
