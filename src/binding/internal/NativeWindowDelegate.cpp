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

#include <utils/WeakWrapper.h>
#include "NativeWindowDelegate.h"

namespace cyder {
    NativeWindowDelegate::NativeWindowDelegate(Environment* env, v8::Local<v8::Object>& handle) :
            env(env), persistent(env->isolate(), handle) {
        persistent.SetWeak();
        persistent.MarkIndependent();
        window = static_cast<Window*>(handle->GetAlignedPointerFromInternalField(0));
    }

    NativeWindowDelegate::~NativeWindowDelegate() {
        persistent.ClearWeak();
        persistent.Reset();
    }

    void NativeWindowDelegate::onResized() {
        auto c = window->screenBuffer()->getCanvas();
        SkPaint paint;
        paint.setColor(SK_ColorGREEN);
        paint.setAntiAlias(true);
        c->clear(0XFFECECEC);
        c->drawRoundRect(SkRect::MakeXYWH(200, 200, 400, 400), 20, 20, paint);
        LOG("onWindowResized");
    }

    void NativeWindowDelegate::onScaleFactorChanged() {
    }

    void NativeWindowDelegate::onOpened() {
        // add to NativeApplication.openedWindows

        // Once a window is opened, it can't be deleted until closed.
        auto handle = env->toLocal(persistent);
        auto weakHandle = static_cast<WeakWrapper*>(handle->GetAlignedPointerFromInternalField(1));
        delete weakHandle;
        handle->SetAlignedPointerInInternalField(1, nullptr);
    }

    bool NativeWindowDelegate::onClosing() {
        return true;
    }

    void NativeWindowDelegate::onClosed() {
        // remove from NativeApplication.openedWindows
        auto handle = env->toLocal(persistent);
        handle->SetAlignedPointerInInternalField(0, nullptr);
        delete window;
        delete this;
    }
}