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

#include "Image.h"

namespace cyder {
    Image* Image::Decode(const void* bytes, size_t length) {
        if (!length) {
            return nullptr;
        }
        auto codec = SkCodec::NewFromData(SkData::MakeWithoutCopy(bytes, length));
        if (!codec) {
            return nullptr;
        }

        SkImageInfo codecInfo = codec->getInfo();
        SkBitmap bitmap;
        bitmap.allocN32Pixels(codecInfo.width(), codecInfo.height(), codecInfo.isOpaque());
        auto result = codec->getPixels(bitmap.info(), bitmap.getPixels(), bitmap.rowBytes());
        delete codec;
        if (result != SkCodec::kSuccess) {
            return nullptr;
        }
        bitmap.setImmutable();
        bitmap.lockPixels();
        auto image = SkImage::MakeFromBitmap(bitmap).release();
        return new Image(image);
    }

    Image* Image::MakeFromPixels(const void* pixels, int width, int height, bool transparent) {
        const size_t bytesPerRow = static_cast<size_t>(4 * width);
        SkBitmap bitmap;
        bitmap.allocN32Pixels(width, height, !transparent);
        SkCanvas canvas(bitmap);
        SkImageInfo info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
        if (!canvas.writePixels(info, pixels, bytesPerRow, 0, 0)) {
            return nullptr;
        }
        auto image = SkImage::MakeFromBitmap(bitmap).release();
        return new Image(image);
    }

    Image::Image(SkImage* pixels) :
            pixels(pixels), subset(nullptr) {
    }

    Image::Image(SkImage* pixels, const SkIRect& subset) :
            pixels(pixels), subset(new SkIRect()) {
        *(this->subset) = subset;
    }

    Image::~Image() {
        SkSafeUnref(pixels);
    }

    Image* Image::makeSubset(int x, int y, int width, int height, bool sharePixels) {
        const SkIRect bounds = subset ? *subset : SkIRect::MakeWH(pixels->width(), pixels->height());
        auto rect = SkIRect::MakeXYWH(x + bounds.x(), y + bounds.y(), width, height);
        if (rect.isEmpty()) {
            return nullptr;
        }
        if (!bounds.contains(rect)) {
            return nullptr;
        }
        if (sharePixels) {
            pixels->ref();
            if (!subset && bounds == rect) {
                return new Image(pixels);
            }
            return new Image(pixels, rect);
        } else {
            auto subImage = pixels->makeSubset(rect).release();
            if (!subImage) {
                return nullptr;
            }
            return new Image(subImage);
        }
    }

    SkData* Image::encode(ImageFormat type, double quality = -1) {
        SkEncodedImageFormat encodeType;
        if (type == ImageFormat::JPEG) {
            encodeType = SkEncodedImageFormat::kJPEG;
            if (quality < 0) {
                quality = 0.92;
            }
        } else if (type == ImageFormat::WEBP) {
            encodeType = SkEncodedImageFormat::kWEBP;
            if (quality < 0) {
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
        if (subset) {
            return pixels->makeSubset(*subset)->encode(encodeType, compressionQuality);
        }
        return pixels->encode(encodeType, compressionQuality);
    }

    bool Image::readPixels(void* buffer, int x, int y, int width, int height) {
        const SkIRect bounds = subset ? *subset : SkIRect::MakeWH(pixels->width(), pixels->height());
        auto rect = SkIRect::MakeXYWH(x + bounds.x(), y + bounds.y(), width, height);
        if (rect.isEmpty()) {
            return false;
        }
        if (!bounds.contains(rect)) {
            return false;
        }
        SkImageInfo info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kUnpremul_SkAlphaType);
        return pixels->readPixels(info, buffer, static_cast<size_t>(4 * width), rect.x(), rect.y());
    }

    void Image::draw(SkCanvas* canvas, const SkRect& dstRect, const SkRect& srcRect) {
        SkRect adjustedSrcRect = srcRect;
        if(subset){
            adjustedSrcRect.offset(subset->fLeft,subset->fTop);
        }
        const SkIRect bounds = subset ? *subset : SkIRect::MakeWH(pixels->width(), pixels->height());
        adjustedSrcRect.intersect(SkRect::Make(bounds));

        if (adjustedSrcRect.isEmpty() || dstRect.isEmpty()){
            return;  // Nothing to draw.
        }
        canvas->drawImageRect(pixels, adjustedSrcRect, dstRect, nullptr);
    }
}