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

#ifndef CYDER_OBJECTCONSTRUCTOR_H
#define CYDER_OBJECTCONSTRUCTOR_H

#include <v8.h>
#include "PerIsolateData.h"

namespace cyder {

    class ConstructorScope {
    public:

        ConstructorScope(v8::Isolate* isolate) : isolate(isolate) {
            PerIsolateData* data = PerIsolateData::From(isolate);
            previous = data->allowNewConstructor;
            data->allowNewConstructor = true;
        }

        ~ConstructorScope() {
            PerIsolateData* data = PerIsolateData::From(isolate);
            data->allowNewConstructor = previous;
        }

        static bool IsValid(v8::Isolate* isolate) {
            return PerIsolateData::From(isolate)->allowNewConstructor;
        }

    private:
        v8::Isolate* isolate;
        bool previous;
    };

    class ObjectConstructor {
    public:
        static v8::MaybeLocal<v8::Object> NewInstance(v8::Isolate* isolate, v8::Local<v8::Function> classFunction,
                                                      int argc = 0, v8::Local<v8::Value> argv[] = nullptr);

        static void IsValidConstructorMode(const v8::FunctionCallbackInfo<v8::Value>&);
    };

}

#endif //CYDER_OBJECTCONSTRUCTOR_H
