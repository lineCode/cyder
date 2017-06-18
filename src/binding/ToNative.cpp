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

#include "ToNative.h"
#include <cmath>

namespace cyder {
    bool ToBooleanSlow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        v8::TryCatch block(isolate);
        bool result = false;
        if (!value->BooleanValue(isolate->GetCurrentContext()).To(&result)) {
            exceptionState.rethrowException(block.Exception());
        }
        return result;
    }


    int32_t ToInt32Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        // Can the value be converted to a number?
        v8::TryCatch block(isolate);
        v8::Local<v8::Number> numberObject;
        if (!value->ToNumber(isolate->GetCurrentContext()).ToLocal(&numberObject)) {
            exceptionState.rethrowException(block.Exception());
            return 0;
        }

        double numberValue = numberObject->Value();
        if (std::isnan(numberValue) || std::isinf(numberValue))
            return 0;

        int32_t result;
        if (!numberObject->Int32Value(isolate->GetCurrentContext()).To(&result)) {
            exceptionState.rethrowException(block.Exception());
            return 0;
        }
        return result;
    }

    uint32_t ToUInt32Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        if (value->IsInt32()) {
            int32_t result = value.As<v8::Int32>()->Value();
            if (result >= 0) {
                return static_cast<uint32_t>(result);
            }
        }

        // Can the value be converted to a number?
        v8::TryCatch block(isolate);
        v8::Local<v8::Number> numberObject;
        if (!value->ToNumber(isolate->GetCurrentContext()).ToLocal(&numberObject)) {
            exceptionState.rethrowException(block.Exception());
            return 0;
        }

        double number_value = numberObject->Value();

        if (std::isnan(number_value) || std::isinf(number_value))
            return 0;

        uint32_t result;
        if (!numberObject->Uint32Value(isolate->GetCurrentContext()).To(&result)) {
            exceptionState.rethrowException(block.Exception());
            return 0;
        }
        return result;
    }

    // Calculate d % 2^{64}.
    inline void doubleToInteger(double d, unsigned long long& value) {
        if (std::isnan(d) || std::isinf(d)) {
            value = 0;
        } else {
            // -2^{64} < fmodValue < 2^{64}.
            double fmodValue =
                    fmod(trunc(d), std::numeric_limits<unsigned long long>::max() + 1.0);
            if (fmodValue >= 0) {
                // 0 <= fmodValue < 2^{64}.
                // 0 <= value < 2^{64}. This cast causes no loss.
                value = static_cast<unsigned long long>(fmodValue);
            } else {
                // -2^{64} < fmodValue < 0.
                // 0 < fmodValueInUnsignedLongLong < 2^{64}. This cast causes no loss.
                unsigned long long fmodValueInUnsignedLongLong =
                        static_cast<unsigned long long>(-fmodValue);
                // -1 < (std::numeric_limits<unsigned long long>::max() -
                //       fmodValueInUnsignedLongLong)
                //    < 2^{64} - 1.
                // 0 < value < 2^{64}.
                value = std::numeric_limits<unsigned long long>::max() -
                        fmodValueInUnsignedLongLong + 1;
            }
        }
    }


    int64_t ToInt64Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        v8::Local<v8::Number> numberObject;
        // Can the value be converted to a number?
        v8::TryCatch block(isolate);
        if (!value->ToNumber(isolate->GetCurrentContext()).ToLocal(&numberObject)) {
            exceptionState.rethrowException(block.Exception());
            return 0;
        }

        double number_value = numberObject->Value();

        if (std::isnan(number_value) || std::isinf(number_value))
            return 0;

        // NaNs and +/-Infinity should be 0, otherwise modulo 2^64.
        unsigned long long integer;
        doubleToInteger(number_value, integer);
        return integer;
    }

    uint64_t ToUInt64Slow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        if (value->IsInt32()) {
            int32_t result = value.As<v8::Int32>()->Value();
            if (result >= 0) {
                return static_cast<uint32_t>(result);
            }
        }

        v8::Local<v8::Number> numberObject;
        // Can the value be converted to a number?
        v8::TryCatch block(isolate);
        if (!value->ToNumber(isolate->GetCurrentContext()).ToLocal(&numberObject)) {
            exceptionState.rethrowException(block.Exception());
            return 0;
        }

        double number_value = numberObject->Value();

        if (std::isnan(number_value) || std::isinf(number_value))
            return 0;

        // NaNs and +/-Infinity should be 0, otherwise modulo 2^64.
        unsigned long long integer;
        doubleToInteger(number_value, integer);
        return integer;
    }

    float ToRestrictedFloat(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        float number_value = ToFloat(isolate, value, exceptionState);
        if (exceptionState.hadException())
            return 0;
        if (!std::isfinite(number_value)) {
            exceptionState.throwTypeError("The provided float value is non-finite.");
            return 0;
        }
        return number_value;
    }

    double ToDoubleSlow(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        v8::TryCatch block(isolate);
        v8::Local<v8::Number> number_value;
        if (!value->ToNumber(isolate->GetCurrentContext()).ToLocal(&number_value)) {
            exceptionState.rethrowException(block.Exception());
            return 0;
        }
        return number_value->Value();
    }

    double ToRestrictedDouble(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        double numberValue = ToDouble(isolate, value, exceptionState);
        if (exceptionState.hadException())
            return 0;
        if (!std::isfinite(numberValue)) {
            exceptionState.throwTypeError("The provided double value is non-finite.");
            return 0;
        }
        return numberValue;
    }


    std::string ToStdString(v8::Isolate* isolate, v8::Local<v8::Value> value, ExceptionState& exceptionState) {
        if (value.IsEmpty()) {
            return "";
        }

        v8::Local<v8::String> stringObject;
        if (value->IsString()) {
            stringObject = value.As<v8::String>();
        } else {
            v8::TryCatch block(isolate);
            if (!value->ToString(isolate->GetCurrentContext()).ToLocal(&stringObject)) {
                exceptionState.rethrowException(block.Exception());
                return "";
            }
        }
        v8::String::Utf8Value v(stringObject);
        return std::string(*v);
    }

    EventListenerPtr ToEventListener(v8::Isolate* isolate, const v8::Local<v8::Value> callback,
                                     const v8::Local<v8::Value>& thisArg, ExceptionState& exceptionState) {
        auto scriptState = ScriptState::From(isolate->GetCurrentContext());
        if (!callback->IsFunction()) {
            exceptionState.throwTypeError("The provided callback parameter for event listener is not a function.");
            return EventListenerPtr();
        }
        auto function = v8::Local<v8::Function>::Cast(callback);
        auto receiver = thisArg;
        if (thisArg->IsNullOrUndefined()) {
            // in case that null !== undefined
            receiver = v8::Null(scriptState->isolate());
        }

        return EventListenerPtr(V8EventListener::Create(scriptState, function, receiver));
    }

}