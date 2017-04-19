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


#include <libplatform/libplatform.h>
#include "JSMain.h"
#include "V8Performance.h"
#include "V8NativeApplication.h"
#include "V8NativeWindow.h"
#include "utils/GetTimer.h"

namespace cyder {

    JSMain::JSMain(const std::string& nativeJSPath, Environment* env) : env(env) {
        attachJS(nativeJSPath);
        installTemplates(env);
    }

    JSMain::~JSMain() {
        env = nullptr;
    }

    void JSMain::installTemplates(Environment* env) {
        v8::HandleScope scope(env->isolate());
        auto global = env->global();
        V8Performance::install(global, env);
        V8NativeApplication::install(global, env);
        V8NativeWindow::install(global, env);
    }

    void JSMain::attachJS(const std::string& path) {
        v8::HandleScope scope(env->isolate());
        auto result = env->executeScript(path);
        ASSERT(!result.IsEmpty());
        if (result.IsEmpty()) {
            return;
        }
    }

    void JSMain::start(int argc, const char** argv) {
        auto isolate = env->isolate();
        v8::HandleScope scope(isolate);
        auto initCyderFunction = env->readGlobalFunction("cyder.initialize", false);
        auto context = env->context();
        auto args = v8::Array::New(isolate, argc);
        for (unsigned int i = 0; i < argc; i++) {
            auto maybeArg = env->makeString(argv[i]);
            ASSERT(!maybeArg.IsEmpty());
            if (maybeArg.IsEmpty()) {
                return;
            }
            auto result = args->Set(context, i, maybeArg.ToLocalChecked());
            USE(result);
        }
        v8::TryCatch tryCatch(isolate);
        auto callResult = env->call(initCyderFunction, env->makeNull(), args);
        if (callResult.IsEmpty()) {
            env->printStackTrace(tryCatch);
        }
    }

    void JSMain::update() {
        auto isolate = env->isolate();
        // Create a stack-allocated handle scope each frame.
        v8::HandleScope scope(isolate);
        v8::Context::Scope contextScope(env->context());
        v8::TryCatch tryCatch(isolate);
        auto updateFunction = env->readGlobalFunction("cyder.updateFrame");
        auto timeStamp = env->makeValue(getTimer());
        auto result = env->call(updateFunction, env->makeNull(), timeStamp);
        if (result.IsEmpty()) {
            env->printStackTrace(tryCatch);
            abort();
        }
    }


}  // namespace cyder