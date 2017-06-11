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

#ifndef CYDER_TOV8_H
#define CYDER_TOV8_H

#include <string>
#include "ScriptWrappable.h"

namespace cyder {


    inline v8::Local<v8::Value> ToV8(v8::Isolate* isolate,
                                     const v8::Local<v8::Object>& creationContext,
                                     ScriptWrappable* impl) {
        if (!impl) {
            return v8::Null(isolate);
        }
        if (impl->containsWrapper()) {
            return impl->getWrapper(isolate);
        }
        return impl->wrap(isolate, creationContext);
    }

    template<size_t sizeOfValue>
    inline v8::Local<v8::Value> ToV8SignedIntegerInternal(v8::Isolate*, int64_t value);

    template<>
    inline v8::Local<v8::Value> ToV8SignedIntegerInternal<4>(v8::Isolate* isolate, int64_t value) {
        return v8::Integer::New(isolate, static_cast<int32_t>(value));
    }

    template<>
    inline v8::Local<v8::Value> ToV8SignedIntegerInternal<8>(v8::Isolate* isolate, int64_t value) {
        int32_t value_in32_bit = static_cast<int32_t>(value);
        if (value_in32_bit == value) {
            return v8::Integer::New(isolate, value);
        }
        // V8 doesn't have a 64-bit integer implementation.
        return v8::Number::New(isolate, value);
    }

    template<size_t sizeOfValue>
    inline v8::Local<v8::Value> ToV8UnsignedIntegerInternal(v8::Isolate*, uint64_t value);

    template<>
    inline v8::Local<v8::Value> ToV8UnsignedIntegerInternal<4>(v8::Isolate* isolate, uint64_t value) {
        return v8::Integer::NewFromUnsigned(isolate, static_cast<uint32_t>(value));
    }

    template<>
    inline v8::Local<v8::Value> ToV8UnsignedIntegerInternal<8>(v8::Isolate* isolate, uint64_t value) {
        uint32_t value_in32_bit = static_cast<uint32_t>(value);
        if (value_in32_bit == value)
            return v8::Integer::NewFromUnsigned(isolate, value);
        // V8 doesn't have a 64-bit integer implementation.
        return v8::Number::New(isolate, value);
    }

    inline v8::Local<v8::Value> ToV8(v8::Isolate* isolate, int value) {
        return ToV8SignedIntegerInternal<sizeof value>(isolate, value);
    }

    inline v8::Local<v8::Value> ToV8(v8::Isolate* isolate, long value) {
        return ToV8SignedIntegerInternal<sizeof value>(isolate, value);
    }

    inline v8::Local<v8::Value> ToV8(v8::Isolate* isolate, long long value) {
        return ToV8SignedIntegerInternal<sizeof value>(isolate, value);
    }

    inline v8::Local<v8::Value> ToV8(v8::Isolate* isolate, unsigned value) {
        return ToV8UnsignedIntegerInternal<sizeof value>(isolate, value);
    }

    inline v8::Local<v8::Value> ToV8(v8::Isolate* isolate, unsigned long value) {
        return ToV8UnsignedIntegerInternal<sizeof value>(isolate, value);
    }

    inline v8::Local<v8::Value> ToV8(v8::Isolate* isolate, unsigned long long value) {
        return ToV8UnsignedIntegerInternal<sizeof value>(isolate, value);
    }

    inline v8::Local<v8::Number> ToV8(v8::Isolate* isolate, double value) {
        return v8::Number::New(isolate, value);
    }

    inline v8::Local<v8::Boolean> ToV8(v8::Isolate* isolate, bool value) {
        return v8::Boolean::New(isolate, value);
    }

    inline v8::Local<v8::String> ToV8(v8::Isolate* isolate, const char* text) {
        static auto type = v8::NewStringType::kNormal;
        return v8::String::NewFromUtf8(isolate, text, type).ToLocalChecked();
    }

    inline v8::Local<v8::String> ToV8(v8::Isolate* isolate, const std::string& text) {
        return ToV8(isolate, text.c_str());
    }
}

#endif //CYDER_TOV8_H
