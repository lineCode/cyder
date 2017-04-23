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

#include "V8Canvas.h"
#include "canvas/Canvas.h"
#include "utils/WeakWrapper.h"
#include "canvas2d/CanvasRenderingContext2D.h"

namespace cyder {

    class CanvasWrapper {
    public:
        Canvas* canvas;
        std::string contextType = "";
        RenderingContext* context = nullptr;
        v8::Persistent<v8::Object> contextObject;

        CanvasWrapper(Canvas* c) : canvas(c) {
        }

        ~CanvasWrapper() {
            contextObject.Reset();
            delete context;
            delete canvas;
        }
    };

    static void getContextMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        auto self = args.This();
        auto wrapper = static_cast<CanvasWrapper*>(self->GetAlignedPointerFromInternalField(0));
        auto contextType = env->toStdString(args[0]);
        if (wrapper->context) {
            if (wrapper->contextType == contextType) {
                args.GetReturnValue().Set(env->toLocal(wrapper->contextObject));
            } else {
                args.GetReturnValue().Set(env->makeNull());
            }
            return;
        }

        //auto contextAttributes = v8::Local<v8::Object>::Cast(args[1]);
        wrapper->contextType = contextType;
        if (contextType == "2d") {
            auto CanvasRenderingContext2DClass = env->readGlobalFunction("CanvasRenderingContext2D");
            wrapper->context = new CanvasRenderingContext2D(wrapper->canvas);
            auto contextObject = env->newInstance(CanvasRenderingContext2DClass,
                                                  env->makeExternal(wrapper->context)).ToLocalChecked();
            wrapper->contextObject.Reset(env->isolate(), contextObject);
            args.GetReturnValue().Set(contextObject);
        } else {
            args.GetReturnValue().Set(env->makeNull());
        }
    }

    static void constructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        int width = env->toInt(args[0]);
        int height = env->toInt(args[1]);
        auto canvas = new Canvas(width, height);
        auto wrapper = new CanvasWrapper(canvas);
        auto self = args.This();
        self->SetAlignedPointerInInternalField(0, wrapper);
        WeakWrapper::BindObject(env->isolate(), self, wrapper);
    }

    void V8Canvas::install(v8::Local<v8::Object> parent, Environment* env) {
        auto classTemplate = env->makeFunctionTemplate(constructor);
        auto prototypeTemplate = classTemplate->PrototypeTemplate();
        env->setTemplateProperty(prototypeTemplate, "getContext", getContextMethod);
        env->attachClass(parent, "Canvas", classTemplate);
    }
}