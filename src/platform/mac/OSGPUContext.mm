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

#include "OSGPUContext.h"
#include "platform/Log.h"

namespace cyder {

    GPUContext* GPUContext::gpuContext = nullptr;
    OSGPUContext* OSGPUContext::gpuContext = nullptr;

    OSGPUContext::OSGPUContext() {
        GPUContext::gpuContext = this;
        OSGPUContext::gpuContext = this;
        initContext();
    }

    void OSGPUContext::initContext() {
        const CGLPixelFormatAttribute attributes[] = {
                kCGLPFAStencilSize, (CGLPixelFormatAttribute) 8,
                kCGLPFAAccelerated,
                kCGLPFADoubleBuffer,
                kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute) kCGLOGLPVersion_3_2_Core,
                (CGLPixelFormatAttribute) 0
        };

        CGLPixelFormatObj format = nullptr;
        GLint npix = 0;
        CGLChoosePixelFormat(attributes, &format, &npix);
        CGLContextObj cglContext;
        CGLCreateContext(format, NULL, &cglContext);
        ASSERT(cglContext);
        CGLDestroyPixelFormat(format);
        GLint interval = 1;
        CGLSetParameter(cglContext, kCGLCPSwapInterval, &interval);
        CGLSetCurrentContext(cglContext);
        _openGLContext = [[NSOpenGLContext alloc] initWithCGLContextObj:
        cglContext];
        ASSERT(_openGLContext);
        CGLReleaseContext(cglContext);
        [_openGLContext makeCurrentContext];
        _glInterface = GrGLCreateNativeInterface();
        ASSERT(_glInterface);
        _grContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext) _glInterface);
        ASSERT(_grContext);
    }

    OSGPUContext::~OSGPUContext() {
        _grContext->abandonContext();
        SkSafeUnref(_grContext);
        SkSafeUnref(_glInterface);
        [_openGLContext release];
    }

    void OSGPUContext::flush() {
        [_openGLContext makeCurrentContext];
        _grContext->flush();
        [_openGLContext flushBuffer];
    }
}