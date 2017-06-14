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

#ifndef CYDER_V8BINDING_H
#define CYDER_V8BINDING_H

#include <v8.h>
#include "WrapperTypeInfo.h"
#include "ToV8.h"
#include "ToNative.h"
#include "SetReturnValue.h"

namespace cyder {

    class V8Binding {
    public:
        static v8::Local<v8::FunctionTemplate> ClassTemplate(v8::Isolate* isolate,
                                                             const WrapperTypeInfo* typeInfo);
        static bool HasInstance(v8::Isolate* isolate,
                                const WrapperTypeInfo* typeInfo,
                                const v8::Local<v8::Value>& value);
    private:
        static void InstallClassTemplate(v8::Isolate* isolate,
                                         const WrapperTypeInfo* typeInfo,
                                         v8::Local<v8::FunctionTemplate> classTemplate);

        static void InstallAccessor(v8::Isolate* isolate,
                                    v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                    v8::Local<v8::Signature> signature,
                                    const AccessorConfiguration& accessor);

        static void InstallMethod(v8::Isolate* isolate,
                                  v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                  v8::Local<v8::Signature> signature,
                                  const MethodConfiguration& method);

        static void InstallConstant(v8::Isolate* isolate,
                                    v8::Local<v8::FunctionTemplate> classTemplate,
                                    v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                    const ConstantConfiguration& constant);

        static void InstallLazyAttribute(v8::Isolate* isolate,
                                         v8::Local<v8::ObjectTemplate> instanceTemplate,
                                         const LazyAttributeConfiguration& lazyAttribute);

        static v8::Local<v8::FunctionTemplate> CreateAccessorTemplate(v8::Isolate* isolate,
                                                                      v8::FunctionCallback callback,
                                                                      v8::Local<v8::Signature> signature,
                                                                      int length);
    };

}

#endif //CYDER_V8BINDING_H
