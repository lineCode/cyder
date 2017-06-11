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

#include "ThrowException.h"

namespace cyder {
    v8::Local<v8::Value> ThrowException::CreateError(v8::Isolate* isolate, const std::string& message) {
        return v8::Exception::Error(ToV8(isolate, message));
    }

    v8::Local<v8::Value> ThrowException::CreateRangeError(v8::Isolate* isolate, const std::string& message) {
        return v8::Exception::RangeError(ToV8(isolate, message));
    }

    v8::Local<v8::Value> ThrowException::CreateReferenceError(v8::Isolate* isolate, const std::string& message) {
        return v8::Exception::ReferenceError(ToV8(isolate, message));
    }

    v8::Local<v8::Value> ThrowException::CreateSyntaxError(v8::Isolate* isolate, const std::string& message) {
        return v8::Exception::SyntaxError(ToV8(isolate, message));
    }

    v8::Local<v8::Value> ThrowException::CreateTypeError(v8::Isolate* isolate, const std::string& message) {
        return v8::Exception::TypeError(ToV8(isolate, message));
    }

    void ThrowException::ThrowError(v8::Isolate* isolate, const std::string& message) {
        Throw(isolate, CreateError(isolate, message));
    }

    void ThrowException::ThrowRangeError(v8::Isolate* isolate, const std::string& message) {
        Throw(isolate, CreateRangeError(isolate, message));
    }

    void ThrowException::ThrowReferenceError(v8::Isolate* isolate, const std::string& message) {
        Throw(isolate, CreateReferenceError(isolate, message));
    }

    void ThrowException::ThrowSyntaxError(v8::Isolate* isolate, const std::string& message) {
        Throw(isolate, CreateSyntaxError(isolate, message));
    }

    void ThrowException::ThrowTypeError(v8::Isolate* isolate, const std::string& message) {
        Throw(isolate, CreateTypeError(isolate, message));
    }
}