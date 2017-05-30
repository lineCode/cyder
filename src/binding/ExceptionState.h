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

#ifndef CYDER_EXCEPTIONSTATE_H
#define CYDER_EXCEPTIONSTATE_H

#include "ThrowException.h"

namespace cyder {

    class ExceptionState {
    public:
        enum ErrorType {
            NoError,
            Error,
            TypeError,
            RangeError,
            ReferenceError,
            SyntaxError,
            RethrownError
        };

        enum ContextType {
            ConstructionContext,
            ExecutionContext,
            DeletionContext,
            GetterContext,
            SetterContext,
            EnumerationContext,
            QueryContext,
            IndexedGetterContext,
            IndexedSetterContext,
            IndexedDeletionContext,
            UnknownContext
        };

        ExceptionState(v8::Isolate* isolate, ContextType contextType,
                       const std::string& interfaceName, const std::string& propertyName)
                : isolate(isolate),
                  _contextType(contextType),
                  _interfaceName(interfaceName),
                  _propertyName(propertyName),
                  errorType(NoError) {
        }

        ExceptionState(v8::Isolate* isolate, ContextType contextType, const std::string& interfaceName)
                : ExceptionState(isolate, contextType, interfaceName, nullptr) {
        }

        ~ExceptionState() {
            if (!_exception.IsEmpty()) {
                ThrowException::Throw(isolate, exception());
            }
        }

        bool hadException() const {
            return errorType != NoError;
        }

        v8::Local<v8::Value> exception() const {
            return v8::Local<v8::Value>::New(isolate, _exception);
        }

        const ContextType contextType() const {
            return _contextType;
        }

        const std::string& message() const {
            return _message;
        }

        const std::string& propertyName() const {
            return _propertyName;
        }

        const std::string& interfaceName() const {
            return _interfaceName;
        }

        void throwError(const std::string& message);
        void throwRangeError(const std::string& message);
        void throwReferenceError(const std::string& message);
        void throwSyntaxError(const std::string& message);
        void throwTypeError(const std::string& message);
        void rethrowException(v8::Local<v8::Value> exception);

        void clearException();

    private:
        std::string _message;
        std::string _propertyName;
        std::string _interfaceName;
        v8::Persistent<v8::Value> _exception;
        ContextType _contextType;
        ErrorType errorType;
        v8::Isolate* isolate;

        void SetException(ErrorType type, const std::string& message, v8::Local<v8::Value> exception);
        std::string addExceptionContext(const std::string& message) const;
    };

}

#endif //CYDER_EXCEPTIONSTATE_H
