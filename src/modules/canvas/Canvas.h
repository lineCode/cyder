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

#ifndef CYDER_CANVAS_H
#define CYDER_CANVAS_H

namespace cyder {

    class Canvas {
    public:
        std::string contextType = "";
        DrawingBuffer* buffer = nullptr;
        RenderingContext* context = nullptr;
        v8::Persistent<v8::Object> contextObject;

        Canvas(int width = 200, int height = 200) : _width(width), _height(height) {
        }

        Canvas(DrawingBuffer* buffer) : buffer(buffer), externalBuffer(true) {
        }

        ~Canvas() {
            contextObject.Reset();
            delete context;
            if (!externalBuffer) {
                delete buffer;
            }
        }

        int width() const {
            return buffer ? buffer->width() : _width;
        }

        virtual void setWidth(int value) {
            if (buffer) {
                buffer->setWidth(value);
            } else {
                _width = value;
            }
        }

        int height() const {
            return buffer ? buffer->height() : _height;
        }

        virtual void setHeight(int value) {
            if (buffer) {
                buffer->setHeight(value);
            } else {
                _height = value;
            }
        }

    private:
        int _width;
        int _height;
        bool externalBuffer = false;
    };

}

#endif //CYDER_CANVAS_H
