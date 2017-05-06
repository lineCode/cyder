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

#ifndef CYDER_WINDOW_H
#define CYDER_WINDOW_H

#include <string>
#include <functional>
#include "WindowInitOptions.h"
#include "display/DrawingBuffer.h"
#include "WindowDelegate.h"

namespace cyder {
    class Window {
    public:

        static Window* New(const WindowInitOptions& initOptions);

        virtual ~Window() {};

        virtual std::string title() = 0;
        virtual void setTitle(const std::string& title) = 0;

        virtual float x() const = 0;
        virtual void setX(float value) = 0;

        virtual float y() const = 0;
        virtual void setY(float value) = 0;

        virtual float width() const = 0;

        virtual float height() const = 0;

        virtual float contentWidth() const = 0;
        virtual float contentHeight() const = 0;
        virtual void setContentSize(float width, float height) = 0;
        virtual float scaleFactor() const = 0;
        virtual void setDelegate(WindowDelegate* delegate) = 0;
        virtual DrawingBuffer* screenBuffer() = 0;

        virtual void activate() = 0;
        virtual void close() = 0;
    };

} // namespace cyder

#endif //CYDER_WINDOW_H
