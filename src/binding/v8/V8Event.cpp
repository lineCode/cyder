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

#include "V8Event.h"
#include "modules/events/EventEmitter.h"

namespace cyder {

    void V8Event::constructorCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {

    }

    void V8Event::typeAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto impl = V8Event::toImpl(info.Holder());
        auto type = ToV8(info.GetIsolate(), impl->type());
        SetReturnValue(info, impl->type());
        info.GetReturnValue().Set(type);
    }

    void V8Event::targetAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto impl = V8Event::toImpl(info.Holder());
        SetReturnValue(info, impl->target());
    }

    void V8Event::cancelableAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto impl = V8Event::toImpl(info.Holder());
        SetReturnValue(info, impl->cancelable());
    }

    void V8Event::isDefaultPreventedAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto impl = V8Event::toImpl(info.Holder());
        SetReturnValue(info, impl->isDefaultPrevented());
    }

    void V8Event::preventDefaultMethodCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {
        auto impl = V8Event::toImpl(info.Holder());
        impl->preventDefault();
    }

    Event* V8Event::toImplWithTypeCheck(v8::Isolate* isolate, v8::Local<v8::Value> value) {
        return V8Binding::HasInstance(isolate, &wrapperTypeInfo, value) ?
               toImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
    }

    static const AccessorConfiguration V8EventAccessors[] = {
            {"type",               V8Event::typeAttributeGetterCallback,               nullptr, v8::ReadOnly},
            {"target",             V8Event::targetAttributeGetterCallback,             nullptr, v8::ReadOnly},
            {"cancelable",         V8Event::cancelableAttributeGetterCallback,         nullptr, v8::ReadOnly},
            {"isDefaultPrevented", V8Event::isDefaultPreventedAttributeGetterCallback, nullptr, v8::ReadOnly}
    };

    static const MethodConfiguration V8EventMethods[] = {
            {"preventDefault", V8Event::preventDefaultMethodCallback, 0, v8::None},
    };

    static const ConstantConfiguration V8EventConstants[]{
            {"ACTIVATE",   ConstantValue("activate")},
            {"DEACTIVATE", ConstantValue("deactivate")},
            {"RESIZE",     ConstantValue("resize")},
            {"RESIZING",   ConstantValue("resizing")},
            {"CHANGE",     ConstantValue("change")},
            {"CHANGING",   ConstantValue("changing")},
            {"COMPLETE",   ConstantValue("complete")},
    };

    const WrapperTypeInfo V8Event::wrapperTypeInfo = {nullptr, "Event",
                                                      V8Event::constructorCallback, 1,
                                                      V8EventAccessors, 4,
                                                      V8EventMethods, 1,
                                                      V8EventConstants, 7,
                                                      nullptr, 0};

    const WrapperTypeInfo& Event::wrapperTypeInfo = V8Event::wrapperTypeInfo;
}