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
#include "NativeWindow.h"

namespace cyder {
    NativeWindow::NativeWindow(const v8::FunctionCallbackInfo<v8::Value>& args) {
        env = Environment::GetCurrent(args);
        persistent.Reset(env->isolate(),args.This());
        persistent.SetWeak();
        persistent.MarkIndependent();
        WindowInitOptions options;
        window = Window::New(options);
        window->setX(300);
        window->setY(150);
        window->setContentSize(800, 600);
        window->setTitle("CYDER");
        window->setDelegate(this);

        auto self = args.This();
        auto eventEmitterClass = env->readGlobalFunction("cyder.EventEmitter");
        env->call(eventEmitterClass, self); // call the super class function.
        auto CanvasClass = env->readGlobalFunction("Canvas");
        auto canvas = env->newInstance(CanvasClass,
                                       env->makeExternal(window->screenBuffer())).ToLocalChecked();
        env->setObjectProperty(self, "canvas", canvas, true);

        onResized();
    }

    NativeWindow::~NativeWindow() {
        persistent.ClearWeak();
        persistent.Reset();
        delete window;
    }

    void NativeWindow::activate() {
        if(window){
            window->activate();
        }
        if(!opened){
            opened = true;
            // add to NativeApplication.openedWindows

        }
    }

    void NativeWindow::onResized() {
        auto c = window->screenBuffer()->getCanvas();
        SkPaint paint;
        paint.setColor(SK_ColorGREEN);
        paint.setAntiAlias(true);
        c->clear(0XFFECECEC);
        c->drawRoundRect(SkRect::MakeXYWH(200, 200, 400, 400), 20, 20, paint);
//        LOG("onWindowResized");
    }

    void NativeWindow::onScaleFactorChanged() {
    }

    void NativeWindow::onActivated() {

    }

    bool NativeWindow::onClosing() {
        return true;
    }

    void NativeWindow::onClosed() {
        // remove from NativeApplication.openedWindows

        delete window;
        window = nullptr;
    }
}