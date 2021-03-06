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


#ifndef CYDER_OSWINDOW_H
#define CYDER_OSWINDOW_H

#include <string>
#include "OSWindowDelegate.h"
#include "platform/WindowInitOptions.h"
#include "platform/Window.h"
#include "ScreenBuffer.h"

namespace cyder {

    class OSWindow : public Window {
    public:
        OSWindow(const WindowInitOptions &initOptions);

        ~OSWindow() override;


        void activate() override;

        void close() override;

        std::string title() override;

        void setTitle(const std::string &title) override;

        float x() const override;

        void setX(float value) override;

        float y() const override;

        void setY(float value) override;

        float width() const override;

        float height() const override;


        float contentWidth() const override;

        float contentHeight() const override;

        void setContentSize(float width, float height) override;

        float scaleFactor() const override;

        void setDelegate(WindowDelegate* delegate) override;

        ScreenBuffer* screenBuffer() override {
            return _screenBuffer;
        }

        bool windowShouldClose();
        void windowWillClose();
        void windowDidBecomeKey();
        void windowDidResize();
        void windowDidChangeBackingProperties();


    private:
        bool opened = false;
        WindowDelegate* delegate = nullptr;
        ScreenBuffer* _screenBuffer;
        NSWindow* nsWindow;
        NSView* nsView;
        OSWindowDelegate* osWindowDelegate;

        NSWindow* createNSWindow(const WindowInitOptions &options);
    };

} // namespace cyder

#endif //CYDER_OSWINDOW_H
