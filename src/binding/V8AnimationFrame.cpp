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

#include "V8AnimationFrame.h"
#include "platform/AnimationFrame.h"

namespace cyder {

    static void update(Environment* env, double timestamp) {
        auto isolate = env->isolate();
        // Create a stack-allocated handle scope each frame.
        v8::HandleScope scope(isolate);
        v8::Context::Scope contextScope(env->context());
        v8::TryCatch tryCatch(isolate);
        auto updateFunction = env->readGlobalFunction("cyder.updateFrame");
        auto result = env->call(updateFunction, env->makeNull(), env->makeValue(timestamp));
        if (result.IsEmpty()) {
            env->printStackTrace(tryCatch);
            abort();
        }
    }

    static void requestAnimationFrameMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        AnimationFrame::Request(std::bind(update, env, std::placeholders::_1));
    }


    void V8AnimationFrame::install(v8::Local<v8::Object> parent, Environment* env) {
        auto cyderScope = env->readGlobalObject("cyder");
        env->setObjectProperty(cyderScope, "requestFrame", requestAnimationFrameMethod);
    }
}