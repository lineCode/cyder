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

#ifndef CYDER_PERCONTEXTDATA_H
#define CYDER_PERCONTEXTDATA_H

#include <unordered_map>
#include "WrapperTypeInfo.h"


namespace cyder {

    class PerContextData {
    public:
        static PerContextData* From(const v8::Local<v8::Context>& context) {
            return static_cast<PerContextData*>(
                    context->GetAlignedPointerFromEmbedderData(CONTEXT_EMBEDDER_DATA_INDEX));
        }

        static PerContextData* From(const v8::Local<v8::Object>& object) {
            return From(object->CreationContext());
        }

        explicit PerContextData(const v8::Local<v8::Context>& context);

        ~PerContextData();

        v8::Local<v8::Context> context() const {
            return v8::Local<v8::Context>::New(_isolate, _context);
        }

        v8::Local<v8::Object> createWrapper(const WrapperTypeInfo* type) {
            auto item = wrapperBoilerplateMap.find(type);
            if (item != wrapperBoilerplateMap.end()) {
                auto boilerplate = v8::Local<v8::Object>::New(_isolate, item->second);
                return boilerplate->Clone();
            }
            return createWrapperSlow(type);
        }

        v8::Local<v8::Function> constructorForType(const WrapperTypeInfo* type) {
            auto item = constructorMap.find(type);
            if (item != constructorMap.end()) {
                return v8::Local<v8::Function>::New(_isolate, item->second);
            }
            return constructorForTypeSlow(type);
        }

    private:
        static const int CONTEXT_EMBEDDER_DATA_INDEX = 1;

        v8::Isolate* _isolate;
        v8::Persistent<v8::Context> _context;
        v8::Persistent<v8::Value> errorPrototype;

        typedef std::unordered_map<const WrapperTypeInfo*, v8::UniquePersistent<v8::Object>> WrapperBoilerplateMap;
        WrapperBoilerplateMap wrapperBoilerplateMap;

        typedef std::unordered_map<const WrapperTypeInfo*, v8::UniquePersistent<v8::Function>> ConstructorMap;
        ConstructorMap constructorMap;


        v8::Local<v8::Object> createWrapperSlow(const WrapperTypeInfo* type);
        v8::Local<v8::Function> constructorForTypeSlow(const WrapperTypeInfo* type);
    };

}

#endif //CYDER_PERCONTEXTDATA_H
