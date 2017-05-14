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

#ifndef CYDER_SCRIPTCONTEXT_H
#define CYDER_SCRIPTCONTEXT_H

#include <v8.h>

namespace cyder {

    class ScriptContext {
    public:
        static ScriptContext* From(const v8::Local<v8::Context>& context) {
            auto scriptState = static_cast<ScriptContext*>(context->GetAlignedPointerFromEmbedderData(
                    CONTEXT_EMBEDDER_DATA_INDEX));
            return scriptState;
        }

        explicit ScriptContext(const v8::Local<v8::Context>& context);

        v8::Local<v8::Object> createWrapper(const v8::Local<v8::FunctionTemplate>& classTemplate);

    private:
        static const int CONTEXT_EMBEDDER_DATA_INDEX = 2;

        v8::Isolate* _isolate;
        v8::Persistent<v8::Context> _context;
        v8::Persistent<v8::Object> _global;
    };

}

#endif //CYDER_SCRIPTCONTEXT_H
