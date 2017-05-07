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
#include "display/DrawingBuffer.h"

namespace cyder {

    class OSWindow;

    class ScreenBuffer : public DrawingBuffer {
    public:
        ScreenBuffer(OSWindow* window);
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

        void setWidth(int value) override;

        /**
         * Indicates the height of the ScreenBuffer, in pixels. It includes the scaleFactor property.
         */
        int height() const override {
            return _height;
        }

        void setHeight(int value) override;

        /**
         * Return a canvas that will draw into this drawing buffer.
         * Note: Do not cache the return value of surface(), it may change when DrawingBuffer resizes.
         */
        SkCanvas* getCanvas() override;

        /**
         * Draws this buffer directly into another canvas.
         */
        void draw(SkCanvas* canvas, SkScalar x, SkScalar y, const SkPaint* paint) override;

        Image* makeImageSnapshot() override;

        /**
         * Call to ensure all drawing to the surface has been applied to the Window.
         */
        void present();

        /**
         * Updates the size of the ScreenBuffer. Then destroys the backend context, and creates a new one.
         * @param width The width of the ScreenBuffer, in pixels. It includes the scaleFactor property.
         * @param height The height of the ScreenBuffer, in pixels. It includes the scaleFactor property.
         */
        void reset(NSView* view);

        /**
         * Destroys the backend context, and marks the ScreenBuffer as invalid. When the dispose() method is called on a
         * ScreenBuffer, all subsequent calls to methods of this ScreenBuffer instance are ignored.
         */
        void dispose() {
            isValid = false;
        }

    private:
        bool contentChanged = false;
        OSWindow* window;
        NSOpenGLContext* openGLContext = nullptr;
        GrContext* grContext = nullptr;
        SkSurface* _surface = nullptr;
        SkSurface* _screen = nullptr;
        int sampleCount = 0;
        int stencilBits = 0;
        bool isValid = true;
        int _width = 0;
        int _height = 0;

        SkSurface* getSurface();

        void invalidateSize() {
            if (_surface) {
                SkSafeUnref(_surface);
                _surface = nullptr;
            }
            if (_screen) {
                SkSafeUnref(_screen);
                _screen = nullptr;
            }
        }
    };

}  // namespace cyder

#endif //CYDER_SCREENUBFFER_H
