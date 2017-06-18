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

#ifndef CYDER_TONATIVE_H
#define CYDER_TONATIVE_H

#include "ScriptWrappable.h"
#include "ExceptionState.h"
#include "V8EventListener.h"

namespace cyder {

    inline ScriptWrappable* ToScriptWrappable(const v8::Local<v8::Object>& wrapper) {
        return reinterpret_cast<ScriptWrappable*>(wrapper->GetAlignedPointerFromInternalField(
                InternalFields::WrapperObjectIndex));
    }

    /**
     * Convert a value to a boolean.
     */
    bool ToBooleanSlow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    inline bool ToBoolean(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        if (value->IsBoolean()) {
            return value.As<v8::Boolean>()->Value();
        }
        return ToBooleanSlow(isolate, value, exceptionState);
    }

    /**
     * Convert a value to a 32-bit signed integer. The conversion fails if the value cannot be converted to a number.
     */
    int32_t ToInt32Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    inline int32_t ToInt32(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        // Fast case. The value is already a 32-bit integer.
        if (value->IsInt32()) {
            return value.As<v8::Int32>()->Value();
        }
        return ToInt32Slow(isolate, value, exceptionState);
    }

    /**
     * Convert a value to a 32-bit unsigned integer. The conversion fails if the value cannot be converted to a number.
     */
    uint32_t ToUInt32Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    inline uint32_t ToUInt32(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        // Fast case. The value is already a 32-bit unsigned integer.
        if (value->IsUint32()) {
            return value.As<v8::Uint32>()->Value();
        }
        // Fast case. The value is a 32-bit signed integer with NormalConversion
        // configuration.
        if (value->IsInt32()) {
            return static_cast<uint32_t>(value.As<v8::Int32>()->Value());
        }
        return ToUInt32Slow(isolate, value, exceptionState);
    }

    /*
     * Convert a value to a 64-bit signed integer. The conversion fails if the value cannot be converted to a number.
     */
    int64_t ToInt64Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    inline int64_t ToInt64(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        // Fast case. The value is a 32-bit integer.
        if (value->IsInt32()) {
            return value.As<v8::Int32>()->Value();
        }
        return ToInt64Slow(isolate, value, exceptionState);
    }

    /*
     * Convert a value to a 64-bit unsigned integer. The conversion fails if the value cannot be converted to a number
     * or the range violated per WebIDL: http://www.w3.org/TR/WebIDL/#es-unsigned-long-long
     */
    uint64_t ToUInt64Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    inline uint64_t ToUInt64(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        // Fast case. The value is a 32-bit unsigned integer.
        if (value->IsUint32()) {
            return value.As<v8::Uint32>()->Value();
        }
        if (value->IsInt32()) {
            return static_cast<uint32_t>(value.As<v8::Int32>()->Value());
        }
        return ToUInt64Slow(isolate, value, exceptionState);
    }

    inline int ToInt(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        return ToInt32(isolate, value, exceptionState);
    }

    inline unsigned int ToUInt(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        return ToUInt32(isolate, value, exceptionState);
    }

    /**
     * Convert a value to a double precision float, which might fail.
     */
    double ToDoubleSlow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    inline double ToDouble(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        if (value->IsNumber()) {
            return value.As<v8::Number>()->Value();
        }
        return ToDoubleSlow(isolate, value, exceptionState);
    }

    /**
     * Convert a value to a double precision float, throwing on non-finite values.
     */
    double ToRestrictedDouble(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    /**
     * Convert a value to a single precision float, which might fail.
     */
    inline float ToFloat(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        return static_cast<float>(ToDouble(isolate, value, exceptionState));
    }

    /**
     * Convert a value to a single precision float, throwing on non-finite values.
     */
    float ToRestrictedFloat(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);


    /**
     * Converts a value to a std::string
     */
    std::string ToStdString(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState);

    EventListenerPtr ToEventListener(v8::Isolate* isolate, const v8::Local<v8::Value> callback,
                                     const v8::Local<v8::Value>& thisArg, ExceptionState& exceptionState);
}

#endif //CYDER_TONATIVE_H
