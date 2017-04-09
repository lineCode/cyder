//////////////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017-present, Cyder.org
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



#include "OSApplication.h"

namespace cyder {
    
    Application* Application::application = nullptr;

    OSApplication::OSApplication() {
        Application::application = this;
        nsApp = [NSApplication sharedApplication];
        appDelegate = [[AppDelegate alloc] init];
        nsApp.delegate = appDelegate;
        [nsApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    }

    OSApplication::~OSApplication() {
        [appDelegate release];
    }

    void OSApplication::exit(int errorCode) {
        if(errorCode==0){
            [nsApp terminate:nil];
        }
        else{
            ::exit(errorCode);
        }
    }

    void OSApplication::run() {
        [nsApp run];
    }
    
    void OSApplication::windowActivated(OSWindow* window) {
        auto windows = _openedWindows;
        auto result = std::find(windows.begin(),windows.end(),window);
        if(result==windows.end()-1){
            return;
        }
        if(result != windows.end()){
            windows.erase(result);
        }
        windows.push_back(window);
    }
    
    void OSApplication::windowClosed(OSWindow* window) {
        auto windows = _openedWindows;
        auto result = std::find(windows.begin(),windows.end(),window);
        if(result != windows.end()){
            if(result == windows.end()-1){
                auto size = windows.size();
                auto activeWindow = size > 1 ? windows[size-2] : nullptr;
                windowActivated(activeWindow);
            }
            windows.erase(result);
        }
    }


} // namespace cyder