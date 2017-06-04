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



#include "V8NativeApplication.h"
#include <iostream>
#include "modules/desktop/NativeWindow.h"

namespace cyder {

    static void stdoutWriteMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto text = env->toStdString(args[0]);
        std::cout << text;
    }

    static void stderrWriteMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto text = env->toStdString(args[0]);
        std::cerr << text;
    }

    static void activeWindowGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        auto window = NativeWindow::activeWindow;
        if (window) {
            args.GetReturnValue().Set(window->getHandle());
        } else {
            args.GetReturnValue().Set(env->makeNull());
        }
    }

    static void openedWindowsGetter(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        auto windows = NativeWindow::openedWindows;
        auto array = env->makeArray(static_cast<int>(windows->size()));
        unsigned int index = 0;
        for (auto& window : *NativeWindow::openedWindows) {
            array->Set(index, window->getHandle());
            index++;
        }
        args.GetReturnValue().Set(array);
    }

    void V8NativeApplication::install(const v8::Local<v8::Object>& parent, Environment* env) {
        auto EventEmitter = env->readGlobalFunction("cyder.EventEmitter");
        auto application = env->newInstance(EventEmitter).ToLocalChecked();
        env->setObjectProperty(parent, "nativeApplication", application);
        auto stdoutObject = env->makeObject();
        env->setObjectProperty(stdoutObject, "write", stdoutWriteMethod);
        env->setObjectProperty(application, "standardOutput", stdoutObject);
        auto stderrObject = env->makeObject();
        env->setObjectProperty(stderrObject, "write", stderrWriteMethod);
        env->setObjectProperty(application, "standardError", stderrObject);
        env->setObjectAccessor(application, "activeWindow", activeWindowGetter);
        env->setObjectAccessor(application, "openedWindows", openedWindowsGetter);
    }

}// namespace cyder