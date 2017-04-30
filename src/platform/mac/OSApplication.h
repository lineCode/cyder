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



#ifndef CYDER_OSAPPLICATION_H
#define CYDER_OSAPPLICATION_H

#import <Cocoa/Cocoa.h>
#include <vector>
#include "platform/Application.h"
#include "AppDelegate.h"
#include "OSWindow.h"

namespace cyder {

    class OSApplication : public Application {
    public:

        OSApplication();
        ~OSApplication() override;

        void exit(int errorCode = 0) override;

        void run() override;
        
        OSWindow* activeWindow() const {
            return _activeWindow;
        }
        
        const std::vector<OSWindow*>* openedWindows() const {
            return _openedWindows;
        }


    private:
        NSApplication* nsApp;
        AppDelegate* appDelegate;
        OSWindow* _activeWindow;
        std::vector<OSWindow*>* _openedWindows;
        
        void windowActivated(OSWindow* window);
        void windowClosed(OSWindow* window);
        
        friend class OSWindow;

    };

} // namespace cyder

#endif //CYDER_OSAPPLICATION_H
