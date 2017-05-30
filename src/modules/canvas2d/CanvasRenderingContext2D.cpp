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

#include "CanvasRenderingContext2D.h"
#include <cmath>

namespace cyder {
    CanvasRenderingContext2D::CanvasRenderingContext2D(DrawingBuffer* buffer) : buffer(buffer) {
    }

    CanvasRenderingContext2D::~CanvasRenderingContext2D() {
    }

    static inline SkRect normalizeRect(const SkRect& rect) {
        return SkRect::MakeXYWH(std::min(rect.fLeft, rect.fRight),
                                std::min(rect.fTop, rect.fBottom),
                                std::max(rect.width(), -rect.width()),
                                std::max(rect.height(), -rect.height()));
    }

    void CanvasRenderingContext2D::drawImage(CanvasImageSource* image, float targetX, float targetY) {
        drawImage(image, 0, 0, image->width(), image->height(), targetX, targetY, image->width(), image->height());
    }

    void CanvasRenderingContext2D::drawImage(CanvasImageSource* image, float targetX, float targetY, float targetWidth,
                                             float targetHeight) {
        drawImage(image, 0, 0, image->width(), image->height(), targetX, targetY, targetWidth, targetHeight);
    }

    void CanvasRenderingContext2D::drawImage(CanvasImageSource* image, float sourceX, float sourceY, float sourceWidth,
                                             float sourceHeight, float targetX, float targetY, float targetWidth,
                                             float targetHeight) {
        if (!image || !image->width() || !image->height())
            return;

        if (!std::isfinite(targetX) || !std::isfinite(targetY) || !std::isfinite(targetWidth) ||
            !std::isfinite(targetHeight) || !std::isfinite(sourceX) || !std::isfinite(sourceY) ||
            !std::isfinite(sourceWidth) || !std::isfinite(sourceHeight) ||
            !targetWidth || !targetHeight || !sourceWidth || !sourceHeight) {
            return;
        }

        SkRect srcRect = normalizeRect(SkRect::MakeXYWH(sourceX, sourceY, sourceWidth, sourceHeight));
        SkRect dstRect = normalizeRect(SkRect::MakeXYWH(targetX, targetY, targetWidth, targetHeight));
        SkRect imageRect = SkRect::MakeWH(image->width(), image->height());

        if (!imageRect.contains(srcRect)) {
            // Compute the src to dst transform
            float scaleX = dstRect.width() / srcRect.width();
            float scaleY = dstRect.height() / srcRect.height();
            float offsetX = dstRect.fLeft * (scaleX - 1);
            float offsetY = dstRect.fTop * (scaleY - 1);
            srcRect.intersect(imageRect);
            // To clip the destination rectangle in the same proportion, transform the clipped src rect
            dstRect = srcRect;
            dstRect = SkRect::MakeXYWH(srcRect.x() * scaleX, srcRect.y() * scaleY,
                                       srcRect.width() * scaleX, srcRect.height() * scaleY);
            dstRect.offset(offsetX, offsetY);
        }

        if (srcRect.isEmpty()) {
            return;
        }
        image->draw(buffer->getCanvas(), dstRect, srcRect);
    }
}