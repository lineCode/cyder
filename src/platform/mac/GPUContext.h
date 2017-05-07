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

#ifndef CYDER_GPUCONTEXT_H
#define CYDER_GPUCONTEXT_H

#include <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>
#include <skia.h>

namespace cyder {

    class GPUContext {
    public:
        GPUContext();

        ~GPUContext();

        static GrContext* GRContext() {
            return context->_grContext;
        }

        static const GrGLInterface* GLInterface() {
            return context->_glInterface;
        }

        static NSOpenGLContext* OpenGLContext() {
            return context->_openGLContext;
        }

        static void Flush() {
            auto openGLContext = context->_openGLContext;
            [openGLContext makeCurrentContext];
            context->_grContext->flush();
            [openGLContext flushBuffer];
        }

    private:
        static GPUContext* context;

        NSOpenGLContext* _openGLContext;
        GrContext* _grContext;
        const GrGLInterface* _glInterface;

        void initContext();
    };

}

#endif //CYDER_GPUCONTEXT_H
