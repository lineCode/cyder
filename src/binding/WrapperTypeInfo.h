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

#ifndef CYDER_WRAPPERTYPEINFO_H
#define CYDER_WRAPPERTYPEINFO_H

#include <v8.h>

namespace cyder {

    enum InternalFields {
        WrapperInfoIndex,
        EncodedValueIndex,
        NumberOfInternalFields,
    };

    static const int V8DOMWrapperTypeIndex = static_cast<int>(WrapperInfoIndex);
    static const int V8DOMWrapperObjectIndex = static_cast<int>(EncodedValueIndex);
    static const int V8DefaultWrapperInternalFieldCount = static_cast<int>(NumberOfInternalFields);
    static const int V8PrototypeTypeIndex = 0;
    static const int V8PrototypeInternalFieldCount = 1;

    typedef v8::Local<v8::FunctionTemplate> (* ClassTemplateFunction)(v8::Isolate* isolate);

    struct WrapperTypeInfo {

        enum WrapperTypePrototype {
            WrapperTypeObjectPrototype,
            WrapperTypeExceptionPrototype,
        };

        enum WrapperClassId {
            NodeClassId = 1,  // NodeClassId must be smaller than ObjectClassId.
            ObjectClassId,
        };

        enum ActiveScriptWrappableInheritance {
            NotInheritFromActiveScriptWrappable,
            InheritFromActiveScriptWrappable,
        };

        enum Lifetime {
            Dependent,
            Independent,
        };

        v8::Local<v8::FunctionTemplate> classTemplate(v8::Isolate* isolate) const {
            return classTemplateFunction(isolate);
        }

        const ClassTemplateFunction classTemplateFunction;
        const char* const className;
        const WrapperTypeInfo* parentClass;
        const unsigned wrapperTypePrototype : 2;  // WrapperTypePrototype
        const unsigned wrapperClassID : 2;        // WrapperClassId
        const unsigned activeScriptWrappableInheritance : 1; // ActiveScriptWrappableInheritance
        const unsigned lifeTime : 1;  // Lifetime
    };

}

#endif //CYDER_WRAPPERTYPEINFO_H
