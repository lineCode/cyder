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


#include <platform/Application.h>
#include <libplatform.h>
#include "base/Globals.h"
#include "base/Environment.h"
#include "platform/Ticker.h"
#include "binding/JSMain.h"
#include "binding/DebugAgent.h"

namespace cyder {

    class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
    public:
        virtual void* Allocate(size_t length) {
            void* data = AllocateUninitialized(length);
            return data == nullptr ? data : memset(data, 0, length);
        }

        virtual void* AllocateUninitialized(size_t length) { return malloc(length); }

        virtual void Free(void* data, size_t) { free(data); }
    };


    int Start(int argc, const char* argv[]) {
        Globals::initialize(argv[0]);

        // Initialize V8.
        v8::V8::InitializeExternalStartupData(Globals::applicationDirectory.c_str());
        auto platform = v8::platform::CreateDefaultPlatform();
        v8::V8::InitializePlatform(platform);
        v8::V8::Initialize();

        // Create a new Isolate and make it the current one.
        auto allocator = new ArrayBufferAllocator();
        v8::Isolate::CreateParams create_params;
        create_params.array_buffer_allocator = allocator;
        auto isolate = v8::Isolate::New(create_params);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope scope(isolate);
        // Create a new context.
        auto context = v8::Context::New(isolate);
        v8::Context::Scope contextScope(context);
        // Enable javascript debug agent.
        std::string arg = argc > 1 ? argv[1] : "";
        bool waitForConnection = (arg == "--debug-brk");
        if (arg == "--debug" || waitForConnection) {
            //DebugAgent::Initialize(Globals::applicationDirectory);
            DebugAgent::Enable("Cyder", 5959, waitForConnection);
        }

        Environment environment(context);
        JSMain jsMain(Globals::resolvePath("cyder.js"), &environment);
        auto result = environment.executeScript(Globals::resolvePath("test.js"));
        ASSERT(!result.IsEmpty());
        jsMain.start("", argc, argv);
        Ticker::Start(std::bind(&JSMain::update, &jsMain));
        Application::application->run();

        Ticker::Stop();
        DebugAgent::Disable();
        isolate->Dispose();
        v8::V8::Dispose();
        v8::V8::ShutdownPlatform();
        delete platform;
        delete allocator;
        return 0;
    }

}
