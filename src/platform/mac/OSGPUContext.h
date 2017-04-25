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

#ifndef CYDER_OSGPUCONTEXT_H
#define CYDER_OSGPUCONTEXT_H

#include "platform/GPUContext.h"
#include <Cocoa/Cocoa.h>
#include <OpenGL/gl.h>

namespace cyder {

    class OSGPUContext : public GPUContext {
    public:
        static OSGPUContext* gpuContext;

        OSGPUContext();

        ~OSGPUContext();

        void flush() override;

        GrContext* grContext() const override {
            return _grContext;
        }

        const GrGLInterface* glInterface() const override {
            return _glInterface;
        }

        NSOpenGLContext* openGLContext() const {
            return _openGLContext;
        }

    private:
        NSOpenGLContext* _openGLContext;
        GrContext* _grContext;
        const GrGLInterface* _glInterface;

        void initContext();
    };

}

#endif //CYDER_OSGPUCONTEXT_H
