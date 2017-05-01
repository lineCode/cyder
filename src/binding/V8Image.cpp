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
#include "utils/WeakWrapper.h"
#include "utils/Base64.h"

namespace cyder {

    void disposeMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto self = args.This();
        auto image = static_cast<SkImage*>(self->GetAlignedPointerFromInternalField(0));
        if (!image) {
            return;
        }
        env->setObjectProperty(self, "width", 0);
        env->setObjectProperty(self, "height", 0);
        self->SetAlignedPointerInInternalField(0, nullptr);
        auto weakHandle = static_cast<WeakWrapper*>(self->GetAlignedPointerFromInternalField(1));
        self->SetAlignedPointerInInternalField(1, nullptr);
        delete weakHandle;
        SkSafeUnref(image);
    }


    SkData* encodeBitmapData(SkImage* image, const std::string& type, double quality) {
        SkEncodedImageFormat encodeType;
        if (type == "image/jpeg") {
            encodeType = SkEncodedImageFormat::kJPEG;
            if (quality == -1) {
                quality = 0.92;
            }
        } else if (type == "image/webp") {
            encodeType = SkEncodedImageFormat::kWEBP;
            if (quality == -1) {
                quality = 0.8;
            }
        } else {
            encodeType = SkEncodedImageFormat::kPNG;
            quality = 1;
        }
        int compressionQuality = static_cast<int>(quality);
        if (quality >= 0.0 && quality <= 1.0) {
            compressionQuality = static_cast<int>(quality * 100 + 0.5);
        }
        return image->encode(encodeType, compressionQuality);
    }

    void encodeMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto image = getInternalImage(args.This(), env);
        if (!image) {
            return;
        }
        std::string type = env->toStdString(args[0]);
        auto quality = args[1]->IsUndefined() ? -1 : env->toDouble(args[1]);
        auto bytes = encodeBitmapData(image, type, quality);
        if (!bytes || bytes->size() == 0) {
            args.GetReturnValue().Set(env->makeNull());
            return;
        }
        auto arrayBuffer = v8::ArrayBuffer::New(env->isolate(), bytes->writable_data(), bytes->size(),
                                                v8::ArrayBufferCreationMode::kExternalized);
        WeakWrapper::BindReference(env->isolate(), arrayBuffer, bytes);
        args.GetReturnValue().Set(arrayBuffer);
    }

    void getImageDataMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
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
        auto arrayBuffer = v8::ArrayBuffer::New(env->isolate(), byteSize);
        auto buffer = arrayBuffer->GetContents().Data();
        SkImageInfo info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
        image->readPixels(info, buffer, static_cast<size_t>(4 * width), x, y);
        auto data = v8::Uint8ClampedArray::New(arrayBuffer, 0, byteSize);
        auto ImageData = env->readGlobalFunction("ImageData");
        auto result = env->newInstance(ImageData, data, env->makeValue(width), env->makeValue(height)).ToLocalChecked();
        args.GetReturnValue().Set(result);
    }

    void makeSubsetMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto image = getInternalImage(args.This(), env);
        if (!image) {
            return;
        }
        auto x = env->toInt(args[0]);
        auto y = env->toInt(args[1]);
        auto width = env->toInt(args[2]);
        auto height = env->toInt(args[3]);
        auto subsetImage = image->makeSubset(SkIRect::MakeXYWH(x, y, width, height)).release();
        if (!subsetImage) {
            args.GetReturnValue().Set(env->makeNull());
            return;
        }
        auto ImageClass = env->readGlobalFunction("Image");
        auto imageObject = env->newInstance(ImageClass, env->makeExternal(subsetImage)).ToLocalChecked();
        args.GetReturnValue().Set(imageObject);
    }


    void toDataURLMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        auto image = getInternalImage(args.This(), env);
        if (!image) {
            return;
        }
        std::string type = env->toStdString(args[0]);
        if (type != "image/jpeg" && type != "image/webp") {
            type = "image/png";
        }
        auto quality = args[1]->IsUndefined() ? -1 : env->toDouble(args[1]);
        auto bytes = encodeBitmapData(image, type, quality);
        if (!bytes || bytes->size() == 0) {
            args.GetReturnValue().Set(env->makeString("data:,").ToLocalChecked());
            return;
        }
        auto data = reinterpret_cast<char*>(bytes->writable_data());
        auto textLength = Base64::EncodeLength(bytes->size());
        auto base64Buffer = new char[textLength + 1];
        Base64::Encode(data, bytes->size(), base64Buffer);
        base64Buffer[textLength] = '\0';
        std::string url = "data:" + type + ";base64," + base64Buffer;
        auto maybeURLObject = env->makeString(url);
        SkSafeUnref(bytes);
        if (maybeURLObject.IsEmpty()) {
            args.GetReturnValue().Set(env->makeString("data:,").ToLocalChecked());
            return;
        }
        args.GetReturnValue().Set(maybeURLObject.ToLocalChecked());
    }

    void constructor(const v8::FunctionCallbackInfo<v8::Value>& args) {
        auto env = Environment::GetCurrent(args);
        v8::HandleScope scope(env->isolate());
        if (!args[0]->IsExternal()) {
            env->throwError(ErrorType::TYPE_ERROR, "Illegal constructor");
            return;
        }
        auto external = v8::Local<v8::External>::Cast(args[0]);
        auto image = reinterpret_cast<SkImage*>(external->Value());
        int width = image->width();
        int height = image->height();
        bool transparent = !image->isOpaque();

        auto self = args.This();
        self->SetAlignedPointerInInternalField(0, image);
        env->setObjectProperty(self, "width", env->makeValue(width));
        env->setObjectProperty(self, "height", env->makeValue(height));
        env->setObjectProperty(self, "transparent", env->makeValue(transparent), true);
        auto handle = WeakWrapper::BindReference(env->isolate(), self, image);
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