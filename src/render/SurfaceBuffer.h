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

#ifndef CYDER_SURFACEBUFFER_H
#define CYDER_SURFACEBUFFER_H

#include "RenderBuffer.h"
#include <skia.h>

namespace cyder {

    class SurfaceBuffer : public RenderBuffer {
    public:
        SurfaceBuffer(int width, int height, bool alpha = true, bool useGPU = true);

        ~SurfaceBuffer() override;

        int width() const override {
            return _width;
        }

        void setWidth(int value) override {
            sizeChanged = true;
            _width = value;
        }

        int height() const override {
            return _height;
        }

        void setHeight(int value) override {
            sizeChanged = true;
            _height = value;
        }

        SkSurface* surface();

    private:
        int _width;
        int _height;
        bool sizeChanged = true;
        bool useGPU;
        bool alpha;
        SkSurface* _surface;
    };

}

#endif //CYDER_SURFACEBUFFER_H
