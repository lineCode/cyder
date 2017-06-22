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

#include <platform/SurfaceFactory.h>
#include "NativeWindow.h"

namespace cyder {

    std::vector<NativeWindow*>* NativeWindow::openedWindows = nullptr;
    NativeWindow* NativeWindow::activeWindow = nullptr;

    NativeWindow::NativeWindow(const v8::FunctionCallbackInfo<v8::Value>& args) {
        args.Holder()->CreationContext();
        env = Environment::GetCurrent(args);
        weakHandle.Reset(env->isolate(), args.This());
        weakHandle.SetWeak();
        weakHandle.MarkIndependent();
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
        weakHandle.ClearWeak();
        weakHandle.Reset();
        delete window;
    }

    void NativeWindow::activate() {
        if (window) {
            window->activate();
        }
        if (!opened) {
            opened = true;
            // Once the window is opened, it can't be garbage collected until it is closed.
            auto handle = env->toLocal(weakHandle);
            persistent.Reset(env->isolate(), handle);
        }
        updateAsActiveWindow();
    }

    void NativeWindow::updateAsActiveWindow() {
        // Add to NativeWindow::openedWindows
        auto windows = NativeWindow::openedWindows;
        if (!windows) {
            windows = NativeWindow::openedWindows = new std::vector<NativeWindow*>();
        }
        auto result = std::find(windows->begin(), windows->end(), this);
        if (result != windows->end() - 1) {
            if (result != windows->end()) {
                windows->erase(result);
            }
            windows->push_back(this);
            NativeWindow::activeWindow = this;
        }
    }


    void NativeWindow::onResized() {
        auto surface = SurfaceFactory::MakeGPU(500, 500);
        auto c = surface->getCanvas();
        SkPaint paint;
        paint.setColor(SK_ColorGREEN);
        paint.setAntiAlias(true);
        c->drawRoundRect(SkRect::MakeXYWH(0, 0, 400, 400), 20, 20, paint);
        auto surface2 = SurfaceFactory::MakeGPU(500, 500);
        auto canvass = surface2->getCanvas();
        surface->draw(canvass, 0, 0, nullptr);
        auto screenCanvas = window->screenBuffer()->getCanvas();
        surface2->draw(screenCanvas, 0, 0, nullptr);
        LOG("onWindowResized");
    }

    void NativeWindow::onScaleFactorChanged() {
    }

    void NativeWindow::onFocusIn() {
        updateAsActiveWindow();
    }

    bool NativeWindow::onClosing() {
        return true;
    }

    void NativeWindow::onClosed() {
        // Remove from NativeWindow::openedWindows
        auto windows = NativeWindow::openedWindows;
        auto result = std::find(windows->begin(), windows->end(), this);
        if (result != windows->end()) {
            if (result == windows->end() - 1) {
                auto size = windows->size();
                NativeWindow::activeWindow = size > 1 ? (*windows)[size - 2] : nullptr;
            }
            windows->erase(result);
        }

        if (windows->size() == 0) {
            delete NativeWindow::openedWindows;
            NativeWindow::openedWindows = nullptr;
        }

        // After window is closed, allow handle to be garbage collected again.
        persistent.Reset();
        delete window;
        window = nullptr;
    }
}