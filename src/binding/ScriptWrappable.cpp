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

#include "ScriptWrappable.h"
#include "PerContextData.h"

namespace cyder {
    ScriptWrappable::~ScriptWrappable() {
        if (persistent.IsEmpty()) {
            return;
        }
        persistent.ClearWeak();
        persistent.Reset();
    }

    v8::Local<v8::Object> ScriptWrappable::wrap(v8::Isolate* isolate, v8::Local<v8::Object> creationContext) {
        auto wrapperTypeInfo = getWrapperTypeInfo();
        auto contextData = PerContextData::From(creationContext);
        auto wrapper = contextData->createWrapper(wrapperTypeInfo);
        setWrapper(isolate, wrapper);
        return wrapper;
    }

    bool ScriptWrappable::setWrapper(v8::Isolate* isolate, v8::Local<v8::Object> wrapper) {
        if (!persistent.IsEmpty()) {
            return false;
        }
        wrapper->SetAlignedPointerInInternalField(InternalFields::WrapperObjectIndex, this);
        persistent.Reset(isolate, wrapper);
        persistent.SetWeak(this, WeakCallback, v8::WeakCallbackType::kParameter);
        return true;
    }
}