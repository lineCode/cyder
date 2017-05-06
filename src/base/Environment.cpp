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


#include "Environment.h"
#include <fstream>

namespace cyder {

    WeakHandle::WeakHandle(v8::Isolate* isolate, const v8::Local<v8::Object>& handle, std::function<void()> callback) :
            persistent(isolate, handle),
            callback(callback) {
        persistent.SetWeak(this, Callback, v8::WeakCallbackType::kParameter);
        persistent.MarkIndependent();
    }

    WeakHandle::~WeakHandle() {
        if (persistent.IsEmpty()) {
            return;
        }
        persistent.ClearWeak();
        persistent.Reset();
    }

    void WeakHandle::Callback(const v8::WeakCallbackInfo<WeakHandle>& data) {
        v8::Isolate* isolate = data.GetIsolate();
        v8::HandleScope handle_scope(isolate);
        WeakHandle* handle = data.GetParameter();
        handle->persistent.Reset();
        if (handle->callback) {
            handle->callback();
        }
        delete handle;
    }

    Environment::Environment(const v8::Local<v8::Context>& context) {
        _isolate = context->GetIsolate();
        _context.Reset(_isolate, context);
        context->SetAlignedPointerInEmbedderData(CONTEXT_EMBEDDER_DATA_INDEX, this);
        auto external = v8::External::New(_isolate, this);
        _external.Reset(_isolate, external);
        _global.Reset(_isolate, context->Global());
    }

    Environment::~Environment() {
        context()->SetAlignedPointerInEmbedderData(CONTEXT_EMBEDDER_DATA_INDEX, nullptr);
        _context.Reset();
        _external.Reset();
    }

    WeakHandle* Environment::bind(const v8::Local<v8::Object>& handle, std::function<void()> callback) {
        return new WeakHandle(_isolate, handle, callback);
    }

    v8::MaybeLocal<v8::Value> Environment::executeScript(const std::string& path) {
        std::ifstream in(path);
        std::istreambuf_iterator<char> beg(in), end;
        std::string jsText(beg, end);
        // Create a string containing the JavaScript source code.
        auto maybeSource = makeString(jsText);
        ASSERT(!maybeSource.IsEmpty());
        if (maybeSource.IsEmpty()) {
            return v8::MaybeLocal<v8::Value>();
        }
        auto source = maybeSource.ToLocalChecked();
        auto maybeJSPath = makeString(path);
        ASSERT(!maybeSource.IsEmpty());
        if (maybeSource.IsEmpty()) {
            return v8::MaybeLocal<v8::Value>();
        }
        auto jsPath = maybeJSPath.ToLocalChecked();
        v8::ScriptOrigin origin(jsPath);
        auto context = this->context();
        v8::TryCatch tryCatch(_isolate);
        // Compile the source code.
        auto script = v8::Script::Compile(context, source, &origin);
        if (script.IsEmpty()) {
            printStackTrace(tryCatch);
            return v8::MaybeLocal<v8::Value>();
        }
        ASSERT(!script.IsEmpty());
        // Run the script to get the result.
        auto result = script.ToLocalChecked()->Run(context);
        if (result.IsEmpty()) {
            printStackTrace(tryCatch);
        }
        ASSERT(!result.IsEmpty());
        return result;
    }


    v8::MaybeLocal<v8::Function> Environment::attachClass(v8::Local<v8::Object> parent, const std::string& className,
                                                          const v8::Local<v8::FunctionTemplate> classTemplate,
                                                          int internalFieldCount) const {
        auto maybeName = makeString(className);
        ASSERT(!maybeName.IsEmpty());
        if (maybeName.IsEmpty()) {
            return v8::MaybeLocal<v8::Function>();
        }
        auto nameValue = maybeName.ToLocalChecked();
        classTemplate->SetClassName(nameValue);
        classTemplate->InstanceTemplate()->SetInternalFieldCount(internalFieldCount);
        auto maybeFunction = classTemplate->GetFunction(context());
        ASSERT(!maybeFunction.IsEmpty());
        if (maybeFunction.IsEmpty()) {
            return v8::MaybeLocal<v8::Function>();

        }
        auto classFunction = maybeFunction.ToLocalChecked();
        auto result = parent->Set(context(), nameValue, classFunction);
        USE(result);
        return v8::MaybeLocal<v8::Function>(classFunction);
    }

    void Environment::throwError(ErrorType errorType, const std::string& errorText) {
        v8::HandleScope scope(_isolate);
        auto maybeString = makeString(errorText);
        ASSERT(!maybeString.IsEmpty());
        if (maybeString.IsEmpty()) {
            return;
        }
        auto stringValue = maybeString.ToLocalChecked();
        v8::Local<v8::Value> error;
        switch (errorType) {
            case ErrorType::TYPE_ERROR :
                error = v8::Exception::TypeError(stringValue);
                break;
            case ErrorType::RANGE_ERROR :
                error = v8::Exception::RangeError(stringValue);
                break;
            case ErrorType::REFERENCE_ERROR :
                error = v8::Exception::ReferenceError(stringValue);
                break;
            case ErrorType::SYNTAX_ERROR :
                error = v8::Exception::SyntaxError(stringValue);
                break;
            default:
                error = v8::Exception::Error(stringValue);
                break;
        }
        _isolate->ThrowException(error);
    }

    void Environment::printStackTrace(const v8::TryCatch& tryCatch) {
        v8::HandleScope scope(_isolate);
        auto context = this->context();

        v8::Local<v8::Value> exception = tryCatch.Exception();
        v8::String::Utf8Value exception_str(exception);
        LOG("========================JS Exception========================\n%s\n", *exception_str);

        static char STACK_TRACE_BUFFER[1024 * 16];
        std::string result = "";
        auto message = tryCatch.Message();
        if (message.IsEmpty()) {
            return;
        }
        v8::String::Utf8Value filename(message->GetScriptResourceName());
        result.append(*filename);
        result.append(":");
        int lineNum = message->GetLineNumber(context).FromMaybe(0);
        sprintf(STACK_TRACE_BUFFER, "%d", lineNum);
        result.append(STACK_TRACE_BUFFER);
        result.append(":");
        auto maybeSourceLine = message->GetSourceLine(context);
        if (!maybeSourceLine.IsEmpty()) {
            v8::String::Utf8Value sourceline(maybeSourceLine.ToLocalChecked());
            result.append(*sourceline);
        }
        result.append("\n");

        LOG("%s", result.c_str());

        auto stackTrace = message->GetStackTrace();
        if (stackTrace.IsEmpty()) {
            return;
        }

        for (unsigned int i = 0; i < stackTrace->GetFrameCount(); ++i) {
            v8::Local<v8::StackFrame> stackFrame = stackTrace->GetFrame(i);
            v8::Local<v8::String> name = stackFrame->GetScriptName();
            if (name.IsEmpty()) {
                result.append(" ");
            } else {
                name->WriteUtf8(STACK_TRACE_BUFFER);
                result.append(STACK_TRACE_BUFFER);
            }
            result.append(":");
            int lineNumber = stackFrame->GetLineNumber();
            sprintf(STACK_TRACE_BUFFER, "%d", lineNumber);
            result.append(STACK_TRACE_BUFFER);
            result.append(":");
            v8::Local<v8::String> functionName = stackFrame->GetFunctionName();
            if (functionName.IsEmpty()) {
                result.append(" ");
            } else {
                functionName->WriteUtf8(STACK_TRACE_BUFFER);
                result.append(STACK_TRACE_BUFFER);
            }
            result.append("\n");
        }
        LOG("%s", result.c_str());
    }

    v8::Local<v8::Object> Environment::findObjectInGlobal(const std::string& name, bool saveCache) {
        auto list = stringSplit(name, ".");
        auto current = global();
        for (const auto& key : list) {
            auto maybeObject = getObject(current, key);
            ASSERT(!maybeObject.IsEmpty());
            current = maybeObject.ToLocalChecked();
        }
        if (saveCache) {
            v8::UniquePersistent<v8::Object> persistent(_isolate, current);
            persistentMap.insert(std::make_pair(name, std::move(persistent)));
        }
        return current;
    }

}  // namespace cyder