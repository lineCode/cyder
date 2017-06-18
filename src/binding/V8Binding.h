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
#include "ToV8.h"
#include "ToNative.h"
#include "PerIsolateData.h"

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


    template<typename CallbackInfo, typename S>
    inline void SetReturnValue(const CallbackInfo& info,
                               const v8::Persistent<S>& handle) {
        info.GetReturnValue().Set(handle);
    }

    template<typename CallbackInfo, typename S>
    inline void SetReturnValue(const CallbackInfo& info,
                               const v8::Local<S> handle) {
        info.GetReturnValue().Set(handle);
    }

    template<typename CallbackInfo, typename S>
    inline void SetReturnValue(const CallbackInfo& info,
                               v8::MaybeLocal<S> maybe) {
        if (!maybe.IsEmpty())
            info.GetReturnValue().Set(maybe.ToLocalChecked());
    }

    template<typename CallbackInfo>
    inline void SetReturnValue(const CallbackInfo& info, bool value) {
        info.GetReturnValue().Set(value);
    }

    template<typename CallbackInfo>
    inline void SetReturnValue(const CallbackInfo& info, double value) {
        info.GetReturnValue().Set(value);
    }

    template<typename CallbackInfo>
    inline void SetReturnValue(const CallbackInfo& info, int32_t value) {
        info.GetReturnValue().Set(value);
    }

    template<typename CallbackInfo>
    inline void SetReturnValue(const CallbackInfo& info, uint32_t value) {
        info.GetReturnValue().Set(value);
    }

    template<typename CallbackInfo>
    inline void SetReturnValueNull(const CallbackInfo& info) {
        info.GetReturnValue().SetNull();
    }

    template<typename CallbackInfo>
    inline void SetReturnValueUndefined(const CallbackInfo& info) {
        info.GetReturnValue().SetUndefined();
    }

    template<typename CallbackInfo>
    inline void SetReturnValueEmptyString(const CallbackInfo& info) {
        info.GetReturnValue().SetEmptyString();
    }

    template<typename CallbackInfo>
    inline void SetReturnValue(const CallbackInfo& info,
                               const std::string& string) {
        if (!string.length()) {
            SetReturnValueEmptyString(info);
            return;
        }
        auto text = PerIsolateData::From(info.GetIsolate())->getStringFromCache(string);
        info.GetReturnValue().Set(text);
    }

    template<typename CallbackInfo>
    inline void SetReturnValueStringOrNull(const CallbackInfo& info,
                                           const std::string& string,
                                           v8::Isolate* isolate) {
        if (!string.length()) {
            SetReturnValueNull(info);
            return;
        }
        auto text = PerIsolateData::From(isolate)->getStringFromCache(string);
        info.GetReturnValue().Set(text);
    }

    template<typename CallbackInfo>
    inline void SetReturnValue(const CallbackInfo& info,
                               ScriptWrappable* impl,
                               v8::Local<v8::Object> creationContext) {
        auto isolate = info.GetIsolate();
        if (!impl) {
            SetReturnValueNull(info);
            return;
        }
        if (impl->containsWrapper()) {
            auto wrapper = impl->getWrapper(isolate);
            info.GetReturnValue().Set(wrapper);
            return;
        }
        auto wrapper = impl->wrap(isolate, creationContext);
        info.GetReturnValue().Set(wrapper);
    }

    template<typename CallbackInfo>
    inline void SetReturnValue(const CallbackInfo& info,
                               ScriptWrappable* impl) {
        SetReturnValue(info, impl, info.Holder());
    }

    inline bool IsUndefinedOrNull(v8::Local<v8::Value> value) {
        return value.IsEmpty() || value->IsNullOrUndefined();
    }

    // These methods store hidden values into an array that is stored in the internal field of a v8 wrapper.
    bool AddHiddenValueToTarget(v8::Isolate* isolate, v8::Local<v8::Object> target, v8::Local<v8::Value> value);
    void RemoveHiddenValueFromTarget(v8::Isolate* isolate, v8::Local<v8::Object> target, v8::Local<v8::Value> value);
}

#endif //CYDER_V8BINDING_H
