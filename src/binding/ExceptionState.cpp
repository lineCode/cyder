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

#include "ExceptionState.h"
#include "ExceptionMessages.h"

namespace cyder {
    void ExceptionState::throwError(const std::string& message) {
        SetException(Error, message, ThrowException::CreateError(isolate, addExceptionContext(message)));
    }

    void ExceptionState::throwRangeError(const std::string& message) {
        SetException(RangeError, message, ThrowException::CreateRangeError(isolate, addExceptionContext(message)));
    }

    void ExceptionState::throwReferenceError(const std::string& message) {
        SetException(ReferenceError, message, ThrowException::CreateReferenceError(isolate, addExceptionContext(message)));
    }

    void ExceptionState::throwSyntaxError(const std::string& message) {
        SetException(SyntaxError, message, ThrowException::CreateSyntaxError(isolate, addExceptionContext(message)));
    }

    void ExceptionState::throwTypeError(const std::string& message) {
        SetException(TypeError, message, ThrowException::CreateTypeError(isolate, addExceptionContext(message)));
    }

    void ExceptionState::rethrowException(v8::Local<v8::Value> exception) {
        SetException(RethrownError, "", exception);
    }

    void ExceptionState::SetException(ErrorType type, const std::string& message, v8::Local<v8::Value> exception) {
        errorType = type;
        _message = message;
        if (exception.IsEmpty()) {
            _exception.Reset();
        } else {
            _exception.Reset(isolate, exception);
        }
    }

    void ExceptionState::clearException() {
        errorType = NoError;
        _message = "";
        _exception.Reset();
    }

    std::string ExceptionState::addExceptionContext(const std::string& message) const {
        if (message.length() == 0) {
            return message;
        }

        std::string processedMessage = message;
        if (_propertyName.length() && _interfaceName.length() && _contextType != UnknownContext) {
            if (_contextType == DeletionContext) {
                processedMessage = ExceptionMessages::FailedToDelete(_propertyName, _interfaceName, message);
            } else if (_contextType == ExecutionContext) {
                processedMessage = ExceptionMessages::FailedToExecute(_propertyName, _interfaceName, message);
            } else if (_contextType == GetterContext) {
                processedMessage = ExceptionMessages::FailedToGet(_propertyName, _interfaceName, message);
            } else if (_contextType == SetterContext) {
                processedMessage = ExceptionMessages::FailedToSet(_propertyName, _interfaceName, message);
            }
        } else if (!_propertyName.length() && _interfaceName.length()) {
            if (_contextType == ConstructionContext) {
                processedMessage = ExceptionMessages::FailedToConstruct(_interfaceName, message);
            } else if (_contextType == EnumerationContext) {
                processedMessage = ExceptionMessages::FailedToEnumerate(_interfaceName, message);
            } else if (_contextType == IndexedDeletionContext) {
                processedMessage = ExceptionMessages::FailedToDeleteIndexed(_interfaceName, message);
            } else if (_contextType == IndexedGetterContext) {
                processedMessage = ExceptionMessages::FailedToGetIndexed(_interfaceName, message);
            } else if (_contextType == IndexedSetterContext) {
                processedMessage = ExceptionMessages::FailedToSetIndexed(_interfaceName, message);
            }
        }
        return processedMessage;
    }
}