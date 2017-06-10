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

#ifndef CYDER_PERISOLATEDATA_H
#define CYDER_PERISOLATEDATA_H

#include <unordered_map>
#include <v8.h>
#include "WrapperTypeInfo.h"

namespace cyder {

    class PerIsolateData {
    public:
        static PerIsolateData* From(v8::Isolate* isolate) {
            return static_cast<PerIsolateData*>(isolate->GetData(ISOLATE_EMBEDDER_DATA_INDEX));
        }

        explicit PerIsolateData(v8::Isolate* isolate);

        ~PerIsolateData();

        v8::Isolate* isolate() const {
            return _isolate;
        }

        v8::MaybeLocal<v8::FunctionTemplate> findClassTemplate(const WrapperTypeInfo* typeInfo);
        void setInterfaceTemplate(const WrapperTypeInfo* typeInfo, const v8::Local<v8::FunctionTemplate>& classTemplate);
        bool hasInstance(const WrapperTypeInfo* typeInfo, const v8::Local<v8::Value>& value);

    private:
        static const int ISOLATE_EMBEDDER_DATA_INDEX = 1;

        v8::Isolate* _isolate;
        bool allowNewConstructor = false;

        typedef std::unordered_map<const WrapperTypeInfo*,
                v8::Eternal<v8::FunctionTemplate>> ClassTemplateMap;
        ClassTemplateMap classTemplateMap;

        friend class ConstructorScope;
    };

}

#endif //CYDER_PERISOLATEDATA_H
