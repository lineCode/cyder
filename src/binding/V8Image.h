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


#ifndef CYDER_V8IMAGE_H
#define CYDER_V8IMAGE_H

#include <v8.h>
#include "base/Environment.h"
#include "display/Image.h"

namespace cyder {

    inline Image* getInternalImage(const v8::Local<v8::Object>& self, Environment* env) {
        auto image = static_cast<Image*>(self->GetAlignedPointerFromInternalField(0));
        if (!image) {
            env->throwError(ErrorType::ERROR, "Invalid Image.");
            return nullptr;
        }
        return image;
    }

    class V8Image {
    public:
        static void install(const v8::Local<v8::Object>& parent, Environment* env);
    };

}

#endif //CYDER_V8IMAGE_H
