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



#ifndef CYDER_OSSCREEN_H
#define CYDER_OSSCREEN_H

#include <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>
#include <skia.h>
#include <platform/GPUScreen.h>

namespace cyder {

    class OSScreen : public GPUScreen {
    public:
        OSScreen(NSView* view);
        ~OSScreen();

        /**
         * Updates the size of the screen.
         * @param width The width of the screen, in pixels. It does not contain the scaleFactor property.
         * @param height The height of the screen, in pixels. It does not contain the scaleFactor property.
         * @param scaleFactor  The effective pixel scaling factor of the screen. This value is 1 on standard screens and doubled on
         * HiDPI (Retina display) screens.
         */
        void updateSize(float width, float height, float scaleFactor) override;

        /**
        * Updates the size of the screen. Then destroys the backend context, and creates a new one.
        * @param width The width of the screen, in pixels. It does not contain the scaleFactor property.
        * @param height The height of the screen, in pixels. It does not contain the scaleFactor property.
        * @param scaleFactor  The effective pixel scaling factor of the screen. This value is 1 on standard screens and doubled on
        * HiDPI (Retina display) screens.
        */
        void reset(float width, float height, float scaleFactor) override;

        /**
         * Destroys the backend context, and marks the screen as invalid. When the dispose() method is called on a screen,
         * all subsequent calls to methods of this screen instance are ignored.
         */
        void dispose() override {
            isValid = false;
        }

        /**
        * Indicates the width of the screen, in pixels. It does not contain the scaleFactor property.
        */
        float width() const override {
            return _width;
        }

        /**
         * Indicates the height of the screen, in pixels. It does not contain the scaleFactor property.
         */
        float height() const override {
            return _height;
        }

        /**
         * Specifies the effective pixel scaling factor of the screen. This value is 1 on standard screens and doubled on
         * HiDPI (Retina display) screens.
         */
        float scaleFactor() const override {
            return _scaleFactor;
        }

        /**
         * A skia surface instance associated with the screen. Anything drew to it will show on the screen. <br/>
         * Note: Do not cache the return value of getSurface(), it may change when screen resizes.
         */
        SkSurface* surface() const override {
            return _surface;
        }

        /**
         * Make the screen become current drawing target. This method is usually called at the begin of one drawing session.
         */
        void makeCurrent() override;

        /**
         * Call to ensure all drawing to the surface has been applied to the screen. This method is usually called at the end
         * of one drawing session
         */
        void present() override;

        /**
         * Makes the screen ignore all previous drawing to the surface.
         */
        void discard() override;

        /**
         * A value of true indicates all the skia surface created by this screen are invalid. Normally it would happen
         * when the previous opengl context of the screen has been replaced with a new one. It will return false after
         * the first calling of the present method.
         */
        bool isReset() const override {
            return hasBeenReset;
        }


    private:
        NSView* view = nullptr;
        NSOpenGLContext* openGLContext = nullptr;
        GrContext* grContext = nullptr;
        const GrGLInterface* glInterface = nullptr;
        SkSurface* _surface = nullptr;
        int sampleCount = 0;
        int stencilBits = 0;
        bool hasBeenReset = false;
        bool isValid = true;
        float _width = 0;
        float _height = 0;
        float _scaleFactor = 1;
    };

}  // namespace cyder

#endif //CYDER_OSSCREEN_H
