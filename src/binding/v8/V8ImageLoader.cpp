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


#include "V8ImageLoader.h"
#include <fstream>
#include "base/Globals.h"
#include "utils/Base64.h"
#include "modules/image/Image.h"

namespace cyder {

    static void loadImageFromURLMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        auto url = env->toStdString(args[0]);
        auto callback = v8::Local<v8::Function>::Cast(args[1]);
        auto thisArg = v8::Local<v8::Object>::Cast(args[2]);

        char* buffer = nullptr;
        size_t length = 0;
        if (url.substr(0, 5) == "data:") {
            auto pos = url.find(",");
            if (pos != std::string::npos && pos != 5) {
                const char* data = url.c_str() + pos + 1;
                size_t textLength = static_cast<size_t>(url.size() - pos - 1);
                length = Base64::DecodeLength(textLength);
                if (length > 0) {
                    buffer = new char[length];
                    Base64::Decode(data, textLength, buffer);
                }
            }
        }
        if (!buffer) {
            url = Globals::resolvePath(url);
            std::ifstream t;
            t.open(url);
            t.seekg(0, std::ios::end);
            auto fileSize = t.tellg();
            if (fileSize > 0) {
                length = static_cast<size_t>(fileSize);
                t.seekg(0, std::ios::beg);
                buffer = new char[length];
                t.read(buffer, length);
            }
            t.close();
        }

        if (!buffer) {
            env->call(callback, thisArg, env->makeNull());
            return;
        }

        auto image = Image::Decode(buffer, length);
        delete[] buffer;
        if (!image) {
            env->call(callback, thisArg, env->makeNull());
            return;
        }
        auto ImageClass = env->readGlobalFunction("Image");
        auto imageObject = env->newInstance(ImageClass, env->makeExternal(image)).ToLocalChecked();
        env->call(callback, thisArg, imageObject);
    }

    static void loadImageFromBytesMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        auto arrayBuffer = v8::Local<v8::ArrayBuffer>::Cast(args[0]);
        auto buffer = arrayBuffer->GetContents().Data();
        auto length = arrayBuffer->ByteLength();
        auto callback = v8::Local<v8::Function>::Cast(args[1]);
        auto thisArg = v8::Local<v8::Object>::Cast(args[2]);
        auto image = Image::Decode(buffer, length);
        if (!image) {
            env->call(callback, thisArg, env->makeNull());
            return;
        }
        auto ImageClass = env->readGlobalFunction("Image");
        auto imageObject = env->newInstance(ImageClass, env->makeExternal(image)).ToLocalChecked();
        env->call(callback, thisArg, imageObject);
    }


    void V8ImageLoader::install(const v8::Local<v8::Object>& parent, Environment* env) {
        auto cyderScope = env->readGlobalObject("cyder");
        env->setObjectProperty(cyderScope, "loadImageFromURL", loadImageFromURLMethod);
        env->setObjectProperty(cyderScope, "loadImageFromBytes", loadImageFromBytesMethod);
    }

}