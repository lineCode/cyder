//////////////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017-present, Cyder.org
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


#ifndef CYDER_GPUSCREEN_H
#define CYDER_GPUSCREEN_H

#include <skia.h>

namespace cyder {

    class GPUScreen {
    public:

        /**
         * Indicates the width of the screen, in pixels. It does not contain the scaleFactor property.
         */
        virtual float width() const = 0;

        /**
         * Indicates the height of the screen, in pixels. It does not contain the scaleFactor property.
         */
        virtual float height() const = 0;

        /**
         * Specifies the effective pixel scaling factor of the screen. This value is 1 on standard screens and doubled on
         * HiDPI (Retina display) screens.
         */
        virtual float scaleFactor() const = 0;

        /**
         * Updates the size of the screen.
         * @param width The width of the screen, in pixels. It does not contain the scaleFactor property.
         * @param height The height of the screen, in pixels. It does not contain the scaleFactor property.
         * @param scaleFactor  The effective pixel scaling factor of the screen. This value is 1 on standard screens and doubled on
         * HiDPI (Retina display) screens.
         */
        virtual void updateSize(float width, float height, float scaleFactor) = 0;

        /**
         * Updates the size of the screen. Then destroys the backend context, and creates a new one. Normally, we should
         * call this mehtod when dragging the window of this screen to another monitor. Otherwise, the window may occur
         * a flicker of blank screen.
         * @param width The width of the screen, in pixels. It does not contain the scaleFactor property.
         * @param height The height of the screen, in pixels. It does not contain the scaleFactor property.
         * @param scaleFactor  The effective pixel scaling factor of the screen. This value is 1 on standard screens and doubled on
         * HiDPI (Retina display) screens.
         */
        virtual void reset(float width, float height, float scaleFactor) = 0;

        /**
         * Marks the screen as invalid. When the dispose() method is called on a screen, all subsequent calls to methods
         * of this screen instance are ignored. We should call this method when the window of this screen is about to
         * close, that prevents errors caused by drawing something to the invalid backend context.
         */
        virtual void dispose() = 0;

        virtual ~GPUScreen() {};

        /**
         * A skia surface instance associated with the screen. Anything drew to it will show on the screen. <br/>
         * Note: Do not cache the return value of surface(), it may change when screen resizes.
         */
        virtual SkSurface* surface() const = 0;

        /**
         * Make the screen become current drawing target. This method is usually called at the begin of one drawing session.
         */
        virtual void makeCurrent() = 0;

        /**
         * Call to ensure all drawing to the surface has been applied to the screen. This method is usually called at the end
         * of one drawing session
         */
        virtual void present() = 0;

        /**
         * Makes the screen ignore all previous drawing to the surface.
         */
        virtual void discard() = 0;

        /**
         * A value of true indicates all the skia surface created by this screen are invalid. Normally it would happen
         * when the previous opengl context of the screen has been replaced with a new one. It will return false after
         * the first calling of the present method.
         */
        virtual bool isReset() const = 0;
    };

}  // namespace cyder

#endif //CYDER_GPUSCREEN_H
