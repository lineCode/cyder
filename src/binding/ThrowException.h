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

#ifndef CYDER_THROWEXCEPTION_H
#define CYDER_THROWEXCEPTION_H

#include <v8.h>
#include "ExceptionMessages.h"

namespace cyder {

    class ThrowException {
    public:
        static void Throw(v8::Isolate* isolate, v8::Local<v8::Value> exception) {
            if (!isolate->IsExecutionTerminating()) {
                isolate->ThrowException(exception);
            }
        }

        static v8::Local<v8::Value> CreateError(v8::Isolate* isolate, const std::string& message);
        static v8::Local<v8::Value> CreateRangeError(v8::Isolate* isolate, const std::string& message);
        static v8::Local<v8::Value> CreateReferenceError(v8::Isolate* isolate, const std::string& message);
        static v8::Local<v8::Value> CreateSyntaxError(v8::Isolate* isolate, const std::string& message);
        static v8::Local<v8::Value> CreateTypeError(v8::Isolate* isolate, const std::string& message);

        static void ThrowError(v8::Isolate* isolate, const std::string& message);
        static void ThrowRangeError(v8::Isolate* isolate, const std::string& message);
        static void ThrowReferenceError(v8::Isolate* isolate, const std::string& message);
        static void ThrowSyntaxError(v8::Isolate* isolate, const std::string& message);
        static void ThrowTypeError(v8::Isolate* isolate, const std::string& message);
    };

}

#endif //CYDER_THROWEXCEPTION_H
