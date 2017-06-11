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

#ifndef CYDER_SCRIPTWRAPPABLE_H
#define CYDER_SCRIPTWRAPPABLE_H

#include <v8.h>
#include "WrapperTypeInfo.h"


#define DEFINE_WRAPPERTYPEINFO()                               \
 public:                                                       \
  const WrapperTypeInfo* getWrapperTypeInfo() const override { \
    return &wrapperTypeInfo;                                \
  }                                                            \
                                                               \
 private:                                                      \
  static const WrapperTypeInfo& wrapperTypeInfo


namespace cyder {

    struct InternalFields {
        static const int WrapperObjectIndex = 0;
        static const int HiddenValuesIndex = 1;
        static const int DefaultInternalFieldCount = 2;
    };

    class ScriptWrappable {
    public:

        ScriptWrappable() {
        }

        virtual ~ScriptWrappable();

        virtual const WrapperTypeInfo* getWrapperTypeInfo() const = 0;

        bool containsWrapper() const {
            return !persistent.IsEmpty();
        }

        v8::Local<v8::Object> getWrapper(v8::Isolate* isolate) {
            return v8::Local<v8::Object>::New(isolate, persistent);
        }

        /**
         * Creates and returns a new wrapper object. Automatically associates this instance with the new wrapper object
         * if this instance is not yet associated with any wrapper.
         */
        v8::Local<v8::Object> wrap(v8::Isolate* isolate, v8::Local<v8::Object> creation_context);

        /**
         * Associates this instance with the given |wrapper| if this instance is not yet associated with any wrapper.
         * Returns true if the given wrapper is associated with this instance, or false if this instance is already
         * associated with a wrapper.
         */
        bool setWrapper(v8::Isolate* isolate, v8::Local<v8::Object> wrapper);

        template<class T>
        T* toImpl() {
            return static_cast<T*>(this);
        }

    private:
        static void WeakCallback(const v8::WeakCallbackInfo<ScriptWrappable>& data) {
            ScriptWrappable* nativeObject = data.GetParameter();
            nativeObject->persistent.Reset();
            delete nativeObject;
        }

        v8::Persistent<v8::Object> persistent;
    };

}

#endif //CYDER_SCRIPTWRAPPABLE_H
