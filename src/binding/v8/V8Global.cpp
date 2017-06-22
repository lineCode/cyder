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

#include "V8Global.h"
#include "V8Event.h"
#include "V8EventEmitter.h"

namespace cyder {
    void V8Global::globalAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {

    }

    void V8Global::performanceAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {

    }

    void V8Global::nativeApplicationAttributeGetterCallback(const v8::FunctionCallbackInfo<v8::Value>& info) {

    }

    Global* V8Global::toImplWithTypeCheck(v8::Isolate* isolate, v8::Local<v8::Value> value) {
        return V8Binding::HasInstance(isolate, &wrapperTypeInfo, value) ?
               toImpl(v8::Local<v8::Object>::Cast(value)) : nullptr;
    }

    static const AccessorConfiguration V8GlobalAccessors[] = {
            {"global",            V8Global::globalAttributeGetterCallback,            nullptr, v8::ReadOnly, InstallOnInstance},
            {"performance",       V8Global::performanceAttributeGetterCallback,       nullptr, v8::ReadOnly, InstallOnInstance},
            {"nativeApplication", V8Global::nativeApplicationAttributeGetterCallback, nullptr, v8::ReadOnly, InstallOnInstance}
    };

    static const LazyAttributeConfiguration V8GlobalLazyAttributes[] = {
            {"Event",        V8ConstructorAttributeGetter, nullptr, static_cast<v8::PropertyAttribute>(v8::DontEnum)},
            {"EventEmitter", V8ConstructorAttributeGetter, nullptr, static_cast<v8::PropertyAttribute>(v8::DontEnum)}
    };


    const WrapperTypeInfo V8Global::wrapperTypeInfo = {nullptr, "Global",
                                                       nullptr, 0,
                                                       V8GlobalAccessors, 3,
                                                       nullptr, 0,
                                                       nullptr, 0,
                                                       V8GlobalLazyAttributes, 2};

    const WrapperTypeInfo& Global::wrapperTypeInfo = V8Global::wrapperTypeInfo;
}