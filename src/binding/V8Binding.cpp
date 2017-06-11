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

#include "V8Binding.h"
#include "PerIsolateData.h"
#include "ObjectConstructor.h"
#include "ToV8.h"

namespace cyder {
    v8::Local<v8::FunctionTemplate> V8Binding::ClassTemplate(v8::Isolate* isolate,
                                                             const WrapperTypeInfo* typeInfo) {
        auto data = PerIsolateData::From(isolate);
        auto result = data->findClassTemplate(typeInfo);
        if (!result.IsEmpty()) {
            return result.ToLocalChecked();
        }
        auto classTemplate = v8::FunctionTemplate::New(isolate, ObjectConstructor::IsValidConstructorMode);
        InstallClassTemplate(isolate, typeInfo, classTemplate);
        data->setInterfaceTemplate(typeInfo, classTemplate);
        return classTemplate;
    }

    bool V8Binding::HasInstance(v8::Isolate* isolate,
                                const WrapperTypeInfo* typeInfo,
                                const v8::Local<v8::Value>& value) {
        auto data = PerIsolateData::From(isolate);
        auto result = data->findClassTemplate(typeInfo);
        if (result.IsEmpty()) {
            return false;
        }
        return result.ToLocalChecked()->HasInstance(value);
    }

    void V8Binding::InstallClassTemplate(v8::Isolate* isolate,
                                         const WrapperTypeInfo* typeInfo,
                                         v8::Local<v8::FunctionTemplate> classTemplate) {
        classTemplate->SetClassName(ToV8(isolate, typeInfo->className));
        classTemplate->ReadOnlyPrototype();
        auto instanceTemplate = classTemplate->InstanceTemplate();
        auto prototypeTemplate = classTemplate->PrototypeTemplate();
        instanceTemplate->SetInternalFieldCount(InternalFields::DefaultInternalFieldCount);
        prototypeTemplate->Set(v8::Symbol::GetToStringTag(isolate), ToV8(isolate, typeInfo->className),
                               static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontEnum));
        if (typeInfo->parentClass) {
            auto parentClassTemplate = ClassTemplate(isolate, typeInfo->parentClass);
            classTemplate->Inherit(parentClassTemplate);
        }
        if (typeInfo->constructor) {
            classTemplate->SetCallHandler(typeInfo->constructor);
            classTemplate->SetLength(typeInfo->constructorLength);
        }
        if (typeInfo->accessorCount) {
            InstallAccessors(isolate, prototypeTemplate, typeInfo->accessors, typeInfo->accessorCount);
        }
        if (typeInfo->methodCount) {
            InstallMethods(isolate, prototypeTemplate, typeInfo->methods, typeInfo->methodCount);
        }
        if (typeInfo->constantCount) {
            InstallConstants(isolate, classTemplate, typeInfo->constants, typeInfo->constantCount);
        }
        if (typeInfo->lazyAttributeCount) {
            InstallLazyAttributes(isolate, instanceTemplate, typeInfo->lazyAttributes, typeInfo->lazyAttributeCount);
        }
    }

    void V8Binding::InstallAccessors(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                     const AccessorConfiguration* accessors, int accessorCount) {

    }

    void V8Binding::InstallMethods(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                   const MethodConfiguration* methods, int methodCount) {

    }

    void V8Binding::InstallConstants(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> classTemplate,
                                     const ConstantConfiguration* constants, int constantCount) {

    }

    void V8Binding::InstallLazyAttributes(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> instanceTemplate,
                                          const LazyAttributeConfiguration* lazyAttributes, int lazyAttributeCount) {

    }

}