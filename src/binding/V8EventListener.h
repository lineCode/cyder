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

#ifndef CYDER_V8EVENTLISTENER_H
#define CYDER_V8EVENTLISTENER_H

#include <memory>
#include <v8.h>
#include "ScriptState.h"
#include "ExceptionState.h"
#include "modules/events/EventListener.h"

namespace cyder {

    class V8EventListener : public EventListener {
    public:
        static V8EventListener* Create(const ScriptState* scriptState,
                                       const v8::Local<v8::Function> callback,
                                       const v8::Local<v8::Value>& thisArg) {
            return new V8EventListener(scriptState, callback, thisArg);
        }

        ~V8EventListener() override;

        void handleEvent(Event* event) const override;
        bool equals(const EventListener* target) const override;

    private:
        v8::Persistent<v8::Function> callback;
        v8::Persistent<v8::Value> thisArg;
        const ScriptState* scriptState;

        V8EventListener(const ScriptState* scriptState,
                        const v8::Local<v8::Function> callback,
                        const v8::Local<v8::Value>& thisArg);
    };

}

#endif //CYDER_V8EVENTLISTENER_H
