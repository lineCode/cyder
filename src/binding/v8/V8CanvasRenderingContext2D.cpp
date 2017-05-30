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

#include "V8CanvasRenderingContext2D.h"
#include "modules/canvas2d/CanvasRenderingContext2D.h"
#include <skia.h>

namespace cyder {
    static void drawImageMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        auto self = args.This();
        auto context = static_cast<CanvasRenderingContext2D*>(self->GetAlignedPointerFromInternalField(0));
        auto imageObject = v8::Local<v8::Object>::Cast(args[0]);
        auto image = static_cast<CanvasImageSource*>(imageObject->GetAlignedPointerFromInternalField(0));
        if (!image) {
            return;
        }
        if (args.Length() == 3) {
            context->drawImage(image, env->toFloat(args[1]), env->toFloat(args[2]));
            return;
        }
        if (args.Length() == 5) {
            context->drawImage(image, env->toFloat(args[1]), env->toFloat(args[2]), env->toFloat(args[3]),
                               env->toFloat(args[4]));
            return;
        }
        context->drawImage(image, env->toFloat(args[1]), env->toFloat(args[2]), env->toFloat(args[3]),
                           env->toFloat(args[4]), env->toFloat(args[5]), env->toFloat(args[6]), env->toFloat(args[7]),
                           env->toFloat(args[8]));
    }

    static void constructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        if (!args[0]->IsExternal()) {
            env->throwError(ErrorType::TYPE_ERROR, "Illegal constructor");
            return;
        }
        auto external = v8::Local<v8::External>::Cast(args[0]);
        auto context = reinterpret_cast<CanvasRenderingContext2D*>(external->Value());
        auto self = args.This();
        self->SetAlignedPointerInInternalField(0, context);
    }

    void V8CanvasRenderingContext2D::install(v8::Local<v8::Object> parent, Environment* env) {
        auto classTemplate = env->makeFunctionTemplate(constructor);
        auto prototypeTemplate = classTemplate->PrototypeTemplate();
        env->setTemplateProperty(prototypeTemplate, "drawImage", drawImageMethod);
        env->attachClass(parent, "CanvasRenderingContext2D", classTemplate);
    }
}