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

#include "PerContextData.h"
#include "platform/Log.h"
#include "ObjectConstructor.h"
#include "ToV8.h"
#include "utils/USE.h"

namespace cyder {
    PerContextData::PerContextData(const v8::Local<v8::Context>& context) {
        _isolate = context->GetIsolate();
        _context.Reset(_isolate, context);
        context->Enter();
        context->SetAlignedPointerInEmbedderData(CONTEXT_EMBEDDER_DATA_INDEX, this);
        auto objectValue = context->Global()->Get(context, ToV8("Error", _isolate)).ToLocalChecked();
        auto prototypeValue = objectValue.As<v8::Object>()->Get(context, ToV8("prototype", _isolate)).ToLocalChecked();
        errorPrototype.Reset(_isolate, prototypeValue);
    }

    PerContextData::~PerContextData() {
        context()->Exit();
    }

    v8::Local<v8::Object> PerContextData::createWrapperSlow(const WrapperTypeInfo* type) {
        auto classFunction = constructorForType(type);
        auto boilerplate = ObjectConstructor::NewInstance(_isolate, classFunction).ToLocalChecked();
        v8::UniquePersistent<v8::Object> persistent(_isolate, boilerplate);
        wrapperBoilerplateMap.insert(std::make_pair(type, std::move(persistent)));
        return boilerplate->Clone();
    }

    v8::Local<v8::Function> PerContextData::constructorForTypeSlow(const WrapperTypeInfo* type) {
        auto currentContext = context();
        v8::Context::Scope scope(currentContext);
        ASSERT(type->classTemplateFunction);
        auto classTemplate = type->classTemplate(_isolate);
        auto classFunction = classTemplate->GetFunction(currentContext).ToLocalChecked();

        if (type->parentClass) {
            auto prototypeTemplate = constructorForType(type->parentClass);
            auto result = classFunction->SetPrototype(currentContext, prototypeTemplate);
            USE(result);
            ASSERT(result.FromMaybe(false))
        }

        auto prototypeValue = classFunction->Get(currentContext, ToV8("prototype", _isolate)).ToLocalChecked();
        ASSERT(prototypeValue->IsObject());
        auto prototypeObject = prototypeValue.As<v8::Object>();
        if (prototypeObject->InternalFieldCount() == V8PrototypeInternalFieldCount &&
            type->wrapperTypePrototype == WrapperTypeInfo::WrapperTypeObjectPrototype) {
            prototypeObject->SetAlignedPointerInInternalField(V8PrototypeTypeIndex, const_cast<WrapperTypeInfo*>(type));
        }
        if (type->wrapperTypePrototype == WrapperTypeInfo::WrapperTypeExceptionPrototype) {
            auto errorProto = v8::Local<v8::Value>::New(_isolate, errorPrototype);
            auto result = prototypeObject->SetPrototype(currentContext, errorProto);
            USE(result);
            ASSERT(result.FromMaybe(false))
        }

        v8::UniquePersistent<v8::Function> persistent(_isolate, classFunction);
        constructorMap.insert(std::make_pair(type, std::move(persistent)));
        return classFunction;
    }
}