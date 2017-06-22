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

#include "V8EventEmitter.h"
#include "V8Event.h"

namespace cyder {
    void V8EventEmitter::onMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        ExceptionState exceptionState(info.GetIsolate(), ExceptionState::ExecutionContext, "EventEmitter", "on");
        auto holder = info.Holder();
        auto impl = V8EventEmitter::toImpl(holder);
        if (info.Length() < 3) {
            exceptionState.throwTypeError(ExceptionMessages::NotEnoughArguments(3, info.Length()));
            return;
        }
        auto isolate = info.GetIsolate();
        auto type = ToStdString(isolate, info[0], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        auto listener = ToEventListener(isolate, info[1], info[2], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        impl->on(type, listener);
        onMethodEpilogueCustom(info, impl);
    }

    void V8EventEmitter::onceMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        ExceptionState exceptionState(info.GetIsolate(), ExceptionState::ExecutionContext, "EventEmitter", "once");
        auto holder = info.Holder();
        auto impl = V8EventEmitter::toImpl(holder);
        if (info.Length() < 3) {
            exceptionState.throwTypeError(ExceptionMessages::NotEnoughArguments(3, info.Length()));
            return;
        }
        auto isolate = info.GetIsolate();
        auto type = ToStdString(isolate, info[0], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        auto listener = ToEventListener(isolate, info[1], info[2], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        impl->once(type, listener);
        onceMethodEpilogueCustom(info, impl);
    }

    void V8EventEmitter::removeListenerMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        ExceptionState exceptionState(info.GetIsolate(), ExceptionState::ExecutionContext,
                                      "EventEmitter", "removeListener");
        auto holder = info.Holder();
        auto impl = V8EventEmitter::toImpl(holder);
        if (info.Length() < 3) {
            exceptionState.throwTypeError(ExceptionMessages::NotEnoughArguments(3, info.Length()));
            return;
        }
        auto isolate = info.GetIsolate();
        auto type = ToStdString(isolate, info[0], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        auto listener = ToEventListener(isolate, info[1], info[2], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        impl->removeListener(type, listener);
        removeListenerMethodEpilogueCustom(info, impl);
    }

    void V8EventEmitter::hasListenerMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        ExceptionState exceptionState(info.GetIsolate(), ExceptionState::ExecutionContext,
                                      "EventEmitter", "hasListener");
        auto impl = V8EventEmitter::toImpl(info.Holder());
        if (info.Length() < 1) {
            exceptionState.throwTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
            return;
        }
        auto type = ToStdString(info.GetIsolate(), info[0], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        auto result = impl->hasListener(type);
        SetReturnValue(info, result);
    }

    void V8EventEmitter::emitMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        ExceptionState exceptionState(info.GetIsolate(), ExceptionState::ExecutionContext,
                                      "EventEmitter", "emit");
        auto impl = V8EventEmitter::toImpl(info.Holder());
        if (info.Length() < 1) {
            exceptionState.throwTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
            return;
        }
        auto event = V8Event::toImplWithTypeCheck(info.GetIsolate(), info[0]);
        if (!event) {
            exceptionState.throwTypeError(ExceptionMessages::ArgumentNullOrIncorrectType(0, "Event"));
            return;
        }
        auto result = impl->emit(event);
        SetReturnValue(info, result);
    }

    void V8EventEmitter::emitWithMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        ExceptionState exceptionState(info.GetIsolate(), ExceptionState::ExecutionContext,
                                      "EventEmitter", "emitWith");
        auto impl = V8EventEmitter::toImpl(info.Holder());
        if (info.Length() < 1) {
            exceptionState.throwTypeError(ExceptionMessages::NotEnoughArguments(1, info.Length()));
            return;
        }
        auto type = ToStdString(info.GetIsolate(), info[0], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        if (info.Length() <= 1) {
            auto result = impl->emitWith(type);
            SetReturnValue(info, result);
            return;
        }
        auto cancelable = ToBoolean(info.GetIsolate(), info[1], exceptionState);
        if (exceptionState.hadException()) {
            return;
        }
        auto result = impl->emitWith(type, cancelable);
        SetReturnValue(info, result);
    }

    EventEmitter* V8EventEmitter::toImplWithTypeCheck(v8::Isolate* isolate, v8::Local<v8::Value> value) {
        return V8Binding::HasInstance(isolate, &wrapperTypeInfo, value) ?
               toImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
    }


    static const MethodConfiguration V8EventMethods[] = {
            {"on",             V8EventEmitter::onMethodCallback,             3, v8::None, InstallOnPrototype},
            {"once",           V8EventEmitter::onceMethodCallback,           3, v8::None, InstallOnPrototype},
            {"removeListener", V8EventEmitter::removeListenerMethodCallback, 3, v8::None, InstallOnPrototype},
            {"hasListener",    V8EventEmitter::hasListenerMethodCallback,    1, v8::None, InstallOnPrototype},
            {"emit",           V8EventEmitter::emitMethodCallback,           1, v8::None, InstallOnPrototype},
            {"emitWith",       V8EventEmitter::emitWithMethodCallback,       2, v8::None, InstallOnPrototype},
    };

    const WrapperTypeInfo V8EventEmitter::wrapperTypeInfo = {nullptr, "EventEmitter",
                                                             nullptr, 0,
                                                             nullptr, 0,
                                                             V8EventMethods, 6,
                                                             nullptr, 0,
                                                             nullptr, 0};

    const WrapperTypeInfo& EventEmitter::wrapperTypeInfo = V8EventEmitter::wrapperTypeInfo;
}