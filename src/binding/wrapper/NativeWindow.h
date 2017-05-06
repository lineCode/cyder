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

#ifndef CYDER_NATIVEWINDOW_H
#define CYDER_NATIVEWINDOW_H

#include "platform/WindowDelegate.h"
#include <vector>
#include "platform/Window.h"
#include "base/Environment.h"


namespace cyder {

    class NativeWindow : public WindowDelegate {
    public:
        static std::vector<NativeWindow*>* openedWindows;
        static NativeWindow* activeWindow;
        static NativeWindow* GetCurrent(const v8::FunctionCallbackInfo<v8::Value>& args) {
            return static_cast<NativeWindow*>(args.This()->GetAlignedPointerFromInternalField(0));
        }

        explicit NativeWindow(const v8::FunctionCallbackInfo<v8::Value>& args);
        ~NativeWindow() override;

        void activate();

        void onResized() override;
        void onFocusIn() override;
        void onClosed() override;
        bool onClosing() override;
        void onScaleFactorChanged() override;

        v8::Local<v8::Object> getHandle() {
            return env->toLocal(weakHandle);
        }

    private:
        Window* window;
        Environment* env;
        v8::Persistent<v8::Object> weakHandle;
        v8::Persistent<v8::Object> persistent;
        bool opened = false;

        void updateAsActiveWindow();
    };

}

#endif //CYDER_NATIVEWINDOW_H
