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

#include "V8EventListener.h"
#include "ToV8.h"
#include "utils/USE.h"

namespace cyder {
    V8EventListener::V8EventListener(const ScriptState* scriptState,
                                     const v8::Local<v8::Function> callback,
                                     const v8::Local<v8::Value>& thisArg) : scriptState(scriptState) {
        this->callback.Reset(scriptState->isolate(), callback);
        this->callback.SetWeak();
        this->thisArg.Reset(scriptState->isolate(), thisArg);
        this->thisArg.SetWeak();
    }

    V8EventListener::~V8EventListener() {
        callback.ClearWeak();
        callback.Reset();
        thisArg.ClearWeak();
        thisArg.Reset();
    }

    void V8EventListener::handleEvent(Event* event) const {
        auto isolate = scriptState->isolate();
        auto context = scriptState->context();
        auto eventObject = ToV8(isolate, context->Global(), event);
        v8::Local<v8::Value> argv[] = {eventObject};
        auto function = callback.Get(isolate);
        auto receiver = thisArg.Get(isolate);
        auto result = function->Call(context, receiver, 1, argv);
        USE(result);
    }

    bool V8EventListener::equals(const EventListener* target) const {
        auto listener = static_cast<const V8EventListener*>(target);
        auto isolate = scriptState->isolate();
        return callback.Get(isolate)->StrictEquals(listener->callback.Get(isolate)) &&
               thisArg.Get(isolate)->StrictEquals(listener->thisArg.Get(isolate));
    }
}