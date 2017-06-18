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

#include "PerIsolateData.h"

namespace cyder {
    v8::MaybeLocal<v8::FunctionTemplate> PerIsolateData::findClassTemplate(const WrapperTypeInfo* typeInfo) {
        auto result = classTemplateMap.find(typeInfo);
        if (result == classTemplateMap.end()) {
            return v8::MaybeLocal<v8::FunctionTemplate>();
        }

        auto classTemplate = result->second.Get(_isolate);
        return v8::MaybeLocal<v8::FunctionTemplate>(classTemplate);
    }

    void PerIsolateData::setInterfaceTemplate(const WrapperTypeInfo* typeInfo,
                                              const v8::Local<v8::FunctionTemplate>& classTemplate) {
        v8::Eternal<v8::FunctionTemplate> handle(_isolate, classTemplate);
        classTemplateMap.insert(std::make_pair(typeInfo, std::move(handle)));
    }

    v8::Local<v8::String> PerIsolateData::getStringFromCacheSlow(const std::string& text) {
        static auto type = v8::NewStringType::kNormal;
        auto value = v8::String::NewFromUtf8(_isolate, text.c_str(), type).ToLocalChecked();
        v8::UniquePersistent<v8::String> persistent(_isolate, value);
        stringCacheMap.insert(std::make_pair(text, std::move(persistent)));
        return value;
    }
}