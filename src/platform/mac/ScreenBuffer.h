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



#ifndef CYDER_SCREENUBFFER_H
#define CYDER_SCREENUBFFER_H

#include <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>
#include <skia.h>
#include "base/DrawingBuffer.h"

namespace cyder {

    class ScreenBuffer : public DrawingBuffer {
    public:
        ScreenBuffer(NSView* view);
        ~ScreenBuffer();

        /**
         * Updates the size of the ScreenBuffer.
         * @param width The width of the ScreenBuffer, in pixels. It includes the scaleFactor property.
         * @param height The height of the ScreenBuffer, in pixels. It includes the scaleFactor property.
         */
        void updateSize(int width, int height);
        /**
        * Indicates the width of the ScreenBuffer, in pixels. It includes the scaleFactor property.
        */
        int width() const override {
            return _width;
        }

        void setWidth(int value) override {
            sizeChanged = true;
            _width = value;
        }

        /**
         * Indicates the height of the ScreenBuffer, in pixels. It includes the scaleFactor property.
         */
        int height() const override {
            return _height;
        }

        void setHeight(int value) override {
            sizeChanged = true;
            _height = value;
        }

        /**
         * A skia surface instance associated with the ScreenBuffer. Anything drew to it will show on the native window.<br/>
         * Note: Do not cache the return value of surface(), it may change when ScreenBuffer resizes.
         */
        SkSurface* surface() override;

        /**
         * Call to ensure all drawing to the surface has been applied to the ScreenBuffer. This method is usually called
         * at the end of one drawing session
         */
        void flush() override;

       /**
        * Updates the size of the ScreenBuffer. Then destroys the backend context, and creates a new one.
        * @param width The width of the ScreenBuffer, in pixels. It includes the scaleFactor property.
        * @param height The height of the ScreenBuffer, in pixels. It includes the scaleFactor property.
        */
        void reset(int width, int height);

        /**
         * Destroys the backend context, and marks the ScreenBuffer as invalid. When the dispose() method is called on a
         * ScreenBuffer, all subsequent calls to methods of this ScreenBuffer instance are ignored.
         */
        void dispose() {
            isValid = false;
        }

    private:
        bool sizeChanged = true;
        NSView* view = nullptr;
        NSOpenGLContext* openGLContext = nullptr;
        GrContext* grContext = nullptr;
        SkSurface* _surface = nullptr;
        int sampleCount = 0;
        int stencilBits = 0;
        bool isValid = true;
        int _width = 0;
        int _height = 0;
    };

}  // namespace cyder

#endif //CYDER_SCREENUBFFER_H
