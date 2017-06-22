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
#include "ObjectConstructor.h"

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
        auto signature = v8::Signature::New(isolate, classTemplate);
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
        auto data = v8::External::New(isolate, const_cast<WrapperTypeInfo*>(typeInfo));
        if (typeInfo->accessorCount) {
            for (int i = 0; i < typeInfo->accessorCount; i++) {
                InstallAccessor(isolate, instanceTemplate, prototypeTemplate,
                                classTemplate, data, signature, typeInfo->accessors[i]);
            }
        }
        if (typeInfo->methodCount) {
            for (int i = 0; i < typeInfo->methodCount; i++) {
                InstallMethod(isolate, instanceTemplate, prototypeTemplate,
                              classTemplate, signature, typeInfo->methods[i]);
            }
        }
        if (typeInfo->constantCount) {
            for (int i = 0; i < typeInfo->constantCount; i++) {
                InstallConstant(isolate, classTemplate, prototypeTemplate, typeInfo->constants[i]);
            }
        }
        if (typeInfo->lazyAttributeCount) {
            for (int i = 0; i < typeInfo->lazyAttributeCount; i++) {
                InstallLazyAttribute(isolate, instanceTemplate, data, typeInfo->lazyAttributes[i]);
            }
        }
    }

    v8::Local<v8::FunctionTemplate> V8Binding::CreateAccessorTemplate(v8::Isolate* isolate,
                                                                      v8::FunctionCallback callback,
                                                                      v8::Local<v8::Value> data,
                                                                      v8::Local<v8::Signature> signature,
                                                                      int length) {
        v8::Local<v8::FunctionTemplate> functionTemplate;
        if (callback) {
            functionTemplate = v8::FunctionTemplate::New(isolate, callback, data, signature, length);
            functionTemplate->RemovePrototype();
        }
        return functionTemplate;
    }

    void V8Binding::InstallAccessor(v8::Isolate* isolate,
                                    v8::Local<v8::ObjectTemplate> instanceTemplate,
                                    v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                    v8::Local<v8::FunctionTemplate> classTemplate,
                                    v8::Local<v8::Value> data,
                                    v8::Local<v8::Signature> signature,
                                    const AccessorConfiguration& accessor) {
        auto name = ToV8(isolate, accessor.name);
        if (accessor.location & (InstallOnInstance | InstallOnPrototype)) {
            auto getter = CreateAccessorTemplate(isolate, accessor.getter, data, signature, 0);
            auto setter = CreateAccessorTemplate(isolate, accessor.setter, data, signature, 1);
            if (accessor.location & InstallOnInstance) {
                instanceTemplate->SetAccessorProperty(name, getter, setter,
                                                      static_cast<v8::PropertyAttribute>(accessor.attribute));
            }
            if (accessor.location & InstallOnPrototype) {
                prototypeTemplate->SetAccessorProperty(name, getter, setter,
                                                       static_cast<v8::PropertyAttribute>(accessor.attribute));
            }
        }
        if (accessor.location & InstallOnClass) {
            // Attributes installed on the interface object must be static attributes, so no need to specify a signature,
            // i.e. no need to do type check against a holder.
            auto getter = CreateAccessorTemplate(isolate, accessor.getter, data, v8::Local<v8::Signature>(), 0);
            auto setter = CreateAccessorTemplate(isolate, accessor.setter, data, v8::Local<v8::Signature>(), 1);
            classTemplate->SetAccessorProperty(name, getter, setter,
                                               static_cast<v8::PropertyAttribute>(accessor.attribute));
        }

    }

    void V8Binding::InstallMethod(v8::Isolate* isolate,
                                  v8::Local<v8::ObjectTemplate> instanceTemplate,
                                  v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                  v8::Local<v8::FunctionTemplate> classTemplate,
                                  v8::Local<v8::Signature> signature,
                                  const MethodConfiguration& method) {
        auto name = ToV8(isolate, method.name);
        if (method.location & (InstallOnInstance | InstallOnPrototype)) {
            auto functionTemplate = v8::FunctionTemplate::New(isolate, method.callback, v8::Local<v8::Value>(),
                                                              signature, method.length);
            functionTemplate->RemovePrototype();
            if (method.location & InstallOnInstance) {
                instanceTemplate->Set(name, functionTemplate, static_cast<v8::PropertyAttribute>(method.attribute));
            }
            if (method.location & InstallOnPrototype) {
                prototypeTemplate->Set(name, functionTemplate, static_cast<v8::PropertyAttribute>(method.attribute));
            }
        }
        if (method.location & InstallOnClass) {
            // Operations installed on the interface object must be static methods, so no need to specify a signature,
            // i.e. no need to do type check against a holder.
            auto functionTemplate = v8::FunctionTemplate::New(isolate, method.callback, v8::Local<v8::Value>(),
                                                              v8::Local<v8::Signature>(), method.length);
            functionTemplate->RemovePrototype();
            classTemplate->Set(name, functionTemplate, static_cast<v8::PropertyAttribute>(method.attribute));
        }
    }

    void V8Binding::InstallConstant(v8::Isolate* isolate,
                                    v8::Local<v8::FunctionTemplate> classTemplate,
                                    v8::Local<v8::ObjectTemplate> prototypeTemplate,
                                    const ConstantConfiguration& constant) {
        auto name = ToV8(isolate, constant.name);
        auto attributes = static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete);
        auto value = constant.value(isolate);
        classTemplate->Set(name, value, attributes);
        prototypeTemplate->Set(name, value, attributes);
    }

    void V8Binding::InstallLazyAttribute(v8::Isolate* isolate,
                                         v8::Local<v8::ObjectTemplate> instanceTemplate,
                                         v8::Local<v8::Value> data,
                                         const LazyAttributeConfiguration& lazyAttribute) {
        auto name = ToV8(isolate, lazyAttribute.name);
        instanceTemplate->SetLazyDataProperty(name, lazyAttribute.getter, data,
                                              static_cast<v8::PropertyAttribute>(lazyAttribute.attribute));
    }

    bool AddHiddenValueToTarget(v8::Isolate* isolate, v8::Local<v8::Object> target, v8::Local<v8::Value> value) {
        auto arrayValue = target->GetInternalField(InternalFields::HiddenValuesIndex);
        if (!arrayValue->IsArray()) {
            arrayValue = v8::Array::New(isolate);
            target->SetInternalField(InternalFields::HiddenValuesIndex, arrayValue);
        }
        auto array = v8::Local<v8::Array>::Cast(arrayValue);
        auto maybe = array->CreateDataProperty(isolate->GetCurrentContext(), array->Length(), value);
        bool result;
        return maybe.To(&result) && result;
    }

    void RemoveHiddenValueFromTarget(v8::Isolate* isolate, v8::Local<v8::Object> target, v8::Local<v8::Value> value) {
        auto arrayValue = target->GetInternalField(InternalFields::HiddenValuesIndex);
        if (!arrayValue->IsArray()) {
            return;
        }
        auto array = v8::Local<v8::Array>::Cast(arrayValue);
        auto context = isolate->GetCurrentContext();
        for (int i = array->Length() - 1; i >= 0; i--) {
            v8::Local<v8::Value> item;
            if (!array->Get(context, static_cast<uint32_t>(i)).ToLocal(&item)) {
                return;
            }
            if (item->StrictEquals(value)) {
                array->Delete(context, static_cast<uint32_t>(i)).ToChecked();
                return;
            }
        }
    }

    void V8ConstructorAttributeGetter(v8::Local<v8::Name> propertyName,
                                      const v8::PropertyCallbackInfo<v8::Value>& info) {
        auto data = info.Data();
        auto perContextData = PerContextData::From(info.Holder());
        if (!perContextData || !data->IsExternal()) {
            return;
        }
        auto typeInfo = reinterpret_cast<const WrapperTypeInfo*>(data.As<v8::External>()->Value());
        info.GetReturnValue().Set(perContextData->constructorForType(typeInfo));
    }
}