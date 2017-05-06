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


#include "V8Image.h"
#include "utils/Base64.h"
#include "utils/SkUnref.h"

namespace cyder {

    static std::string formatImageMimeType(const std::string& mimeType) {
        auto type = toLowerString(mimeType);
        if (type != "image/jpeg" && type != "image/webp") {
            type = "image/png";
        }
        return type;
    }

    static ImageFormat toImageFormat(const std::string& type) {
        ImageFormat format;
        if (type == "image/jpeg") {
            format = ImageFormat::JPEG;
        } else if (type == "image/webp") {
            format = ImageFormat::WEBP;
        } else {
            format = ImageFormat::PNG;
        }
        return format;
    }

    static void disposeMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto self = args.This();
        auto image = static_cast<SkImage*>(self->GetAlignedPointerFromInternalField(0));
        if (!image) {
            return;
        }
        env->setObjectProperty(self, "width", 0);
        env->setObjectProperty(self, "height", 0);
        self->SetAlignedPointerInInternalField(0, nullptr);
        auto weakHandle = static_cast<WeakHandle*>(self->GetAlignedPointerFromInternalField(1));
        self->SetAlignedPointerInInternalField(1, nullptr);
        delete weakHandle;
        SkSafeUnref(image);
    }

    static void encodeMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto image = getInternalImage(args.This(), env);
        if (!image) {
            return;
        }
        std::string type = formatImageMimeType(env->toStdString(args[0]));
        auto quality = args[1]->IsUndefined() ? -1 : env->toDouble(args[1]);
        auto bytes = image->encode(toImageFormat(type), quality);
        if (!bytes || bytes->size() == 0) {
            args.GetReturnValue().Set(env->makeNull());
            return;
        }
        auto arrayBuffer = env->makeArrayBuffer(bytes->writable_data(), bytes->size());
        env->bind(arrayBuffer, SkUnref::Wrap(bytes));
        args.GetReturnValue().Set(arrayBuffer);
    }

    static void getImageDataMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto image = getInternalImage(args.This(), env);
        if (!image) {
            return;
        }
        auto x = env->toInt(args[0]);
        auto y = env->toInt(args[1]);
        auto width = env->toInt(args[2]);
        auto height = env->toInt(args[3]);
        size_t byteSize = static_cast<size_t>(width * height * 4);
        if (byteSize == 0) {
            args.GetReturnValue().Set(env->makeNull());
            return;
        }
        auto arrayBuffer = env->makeArrayBuffer(byteSize);
        auto buffer = arrayBuffer->GetContents().Data();
        if (!image->readPixels(buffer, x, y, width, height)) {
            args.GetReturnValue().Set(env->makeNull());
            return;
        }
        auto data = v8::Uint8ClampedArray::New(arrayBuffer, 0, byteSize);
        auto ImageData = env->readGlobalFunction("ImageData");
        auto result = env->newInstance(ImageData, data, env->makeValue(width), env->makeValue(height)).ToLocalChecked();
        args.GetReturnValue().Set(result);
    }

    static void makeSubsetMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto image = getInternalImage(args.This(), env);
        if (!image) {
            return;
        }
        auto x = env->toInt(args[0]);
        auto y = env->toInt(args[1]);
        auto width = env->toInt(args[2]);
        auto height = env->toInt(args[3]);
        auto sharePixels = args[4]->IsUndefined() ? true : env->toBoolean(args[4]);
        auto subsetImage = image->makeSubset(x, y, width, height, sharePixels);
        if (!subsetImage) {
            args.GetReturnValue().Set(env->makeNull());
            return;
        }
        auto ImageClass = env->readGlobalFunction("Image");
        auto imageObject = env->newInstance(ImageClass, env->makeExternal(subsetImage)).ToLocalChecked();
        args.GetReturnValue().Set(imageObject);
    }


    static void toDataURLMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto image = getInternalImage(args.This(), env);
        if (!image) {
            return;
        }
        std::string mimeType = formatImageMimeType(env->toStdString(args[0]));
        auto quality = args[1]->IsUndefined() ? -1 : env->toDouble(args[1]);
        auto bytes = image->encode(toImageFormat(mimeType), quality);
        if (!bytes || bytes->size() == 0) {
            args.GetReturnValue().Set(env->makeString("data:,").ToLocalChecked());
            return;
        }
        auto data = reinterpret_cast<char*>(bytes->writable_data());
        auto textLength = Base64::EncodeLength(bytes->size());
        auto base64Buffer = new char[textLength + 1];
        Base64::Encode(data, bytes->size(), base64Buffer);
        base64Buffer[textLength] = '\0';
        std::string url = "data:" + mimeType + ";base64," + base64Buffer;
        auto maybeURLObject = env->makeString(url);
        SkSafeUnref(bytes);
        if (maybeURLObject.IsEmpty()) {
            args.GetReturnValue().Set(env->makeString("data:,").ToLocalChecked());
            return;
        }
        args.GetReturnValue().Set(maybeURLObject.ToLocalChecked());
    }

    static Image* createFromImageData(const v8::Local<v8::Object>& imageData, bool transparent, Environment* env) {
        auto width = env->getInt(imageData, "width");
        auto height = env->getInt(imageData, "height");
        if (width == 0 || height == 0) {
            return nullptr;
        }
        auto maybeData = env->getObject(imageData, "data");
        if (maybeData.IsEmpty()) {
            return nullptr;
        }
        auto maybeBuffer = env->getObject(maybeData.ToLocalChecked(), "buffer");
        if (maybeBuffer.IsEmpty()) {
            return nullptr;
        }
        auto buffer = maybeBuffer.ToLocalChecked();
        if (!buffer->IsArrayBuffer()) {
            return nullptr;
        }
        auto arrayBuffer = v8::Local<v8::ArrayBuffer>::Cast(buffer);
        auto bytes = arrayBuffer->GetContents().Data();
        auto length = arrayBuffer->ByteLength();
        if (length < width * height * 4) {
            return nullptr;
        }
        return Image::FromPixels(bytes, width, height, transparent);
    }

    static void constructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        Image* image;
        if (args[0]->IsExternal()) {
            auto external = v8::Local<v8::External>::Cast(args[0]);
            image = reinterpret_cast<Image*>(external->Value());
        } else {
            auto imageData = v8::Local<v8::Object>::Cast(args[0]);
            auto transparent = args[1]->IsUndefined() ? true : env->toBoolean(args[1]);
            image = createFromImageData(imageData, transparent, env);
            if(!image){
                env->throwError(ErrorType::TYPE_ERROR, "Failed to execute 'new Image()': parameter 1 is invalid ImageData.");
                return;
            }
        }
        auto self = args.This();
        self->SetAlignedPointerInInternalField(0, image);
        env->setObjectProperty(self, "width", env->makeValue(image->width()));
        env->setObjectProperty(self, "height", env->makeValue(image->height()));
        env->setObjectProperty(self, "transparent", env->makeValue(image->transparent()), true);
        auto handle = env->bind(self, image);
        self->SetAlignedPointerInInternalField(1, handle);
    }

    void V8Image::install(const v8::Local<v8::Object>& parent, Environment* env) {
        auto classTemplate = env->makeFunctionTemplate(constructor);
        auto prototypeTemplate = classTemplate->PrototypeTemplate();
        env->setTemplateProperty(prototypeTemplate, "dispose", disposeMethod);
        env->setTemplateProperty(prototypeTemplate, "encode", encodeMethod);
        env->setTemplateProperty(prototypeTemplate, "getImageData", getImageDataMethod);
        env->setTemplateProperty(prototypeTemplate, "makeSubset", makeSubsetMethod);
        env->setTemplateProperty(prototypeTemplate, "toDataURL", toDataURLMethod);
        env->attachClass(parent, "Image", classTemplate, 2);
    }

}