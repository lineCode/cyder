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



#ifndef CYDER_OSANIMATIONFRAME_H
#define CYDER_OSANIMATIONFRAME_H

#import <Cocoa/Cocoa.h>
#import <vector>
#import "platform/AnimationFrame.h"

namespace cyder {

    class OSAnimationFrame {
    public:
        static void RequestScreenUpdate() {
            if (animationFrame->needUpdateScreen) {
                return;
            }
            animationFrame->needUpdateScreen = true;
            if (!animationFrame->displayLinkIsRunning) {
                animationFrame->startDisplayLink();
            }
        }

        static unsigned long Request(FrameRequestCallback callback) {
            auto& callbackList = animationFrame->callbackList;
            callbackList->push_back(callback);
            if (!animationFrame->displayLinkIsRunning) {
                animationFrame->startDisplayLink();
            }
            return callbackList->size() - 1;
        }

        static void Cancel(unsigned long handle) {
            auto& callbackList = animationFrame->callbackList;
            if (handle < callbackList->size()) {
                callbackList->erase(callbackList->begin() + handle);
            }
        }

        static void Update() {
            animationFrame->update();
        }

        OSAnimationFrame();
        ~OSAnimationFrame();
        bool displayLinkIsRunning = false;

    private:
        static OSAnimationFrame* animationFrame;

        CVDisplayLinkRef displayLink;
        std::vector<FrameRequestCallback>* callbackList;
        bool needUpdateScreen = false;


        void update();

        void startDisplayLink() {
            if (!displayLinkIsRunning) {
                displayLinkIsRunning = true;
                CVDisplayLinkStart(displayLink);
            }
        }
    };
}

#endif  //CYDER_OSANIMATIONFRAME_H