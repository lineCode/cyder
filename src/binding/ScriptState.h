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

#ifndef CYDER_SCRIPTSTATE_H
#define CYDER_SCRIPTSTATE_H

#include <v8.h>
#include "PerContextData.h"

namespace cyder {

    class ScriptState {
    public:
        static ScriptState* From(const v8::Local<v8::Context>& context) {
            return static_cast<ScriptState*>(
                    context->GetAlignedPointerFromEmbedderData(CONTEXT_EMBEDDER_DATA_INDEX));
        }

        static ScriptState* From(const v8::FunctionCallbackInfo<v8::Value>& info) {
            return From(info.Holder()->CreationContext());
        }

        static ScriptState* From(const v8::PropertyCallbackInfo<v8::Value>& info) {
            return From(info.Holder()->CreationContext());
        }

        static ScriptState* From(const v8::PropertyCallbackInfo<void>& info) {
            return From(info.Holder()->CreationContext());
        }

        explicit ScriptState(const v8::Local<v8::Context>& context) : _perContextData(context) {
            _isolate = context->GetIsolate();
            _context.Reset(_isolate, context);
            context->SetAlignedPointerInEmbedderData(CONTEXT_EMBEDDER_DATA_INDEX, this);
        }

        ~ScriptState() {
            _context.Reset();
        }

        v8::Isolate* isolate() const {
            return _isolate;
        }

        v8::Local<v8::Context> context() const {
            return v8::Local<v8::Context>::New(_isolate, _context);
        }

        PerContextData* perContextData() {
            return &_perContextData;
        }

    private:
        static const int CONTEXT_EMBEDDER_DATA_INDEX = 0;

        v8::Isolate* _isolate;
        v8::Persistent<v8::Context> _context;
        PerContextData _perContextData;
    };

}

#endif //CYDER_SCRIPTSTATE_H
