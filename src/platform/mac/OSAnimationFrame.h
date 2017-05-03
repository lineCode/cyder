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

#include <Cocoa/Cocoa.h>
#include <vector>
#include <mutex>
#include "platform/AnimationFrame.h"

namespace cyder {

    class OSAnimationFrame {
    public:
        static void RequestScreenUpdate() {
            if (animationFrame->needUpdateScreen) {
                return;
            }
            animationFrame->needUpdateScreen = true;
            animationFrame->requestNextFrame();
        }

        static unsigned long Request(FrameRequestCallback callback) {
            auto callbackList = animationFrame->callbackList;
            auto handle = callbackList->size();
            callbackList->push_back(callback);
            animationFrame->requestNextFrame();
            return handle;
        }

        static void Cancel(unsigned long handle) {
            auto callbackList = animationFrame->callbackList;
            if (handle < callbackList->size()) {
                callbackList->erase(callbackList->begin() + handle);
            }
            animationFrame->requestNextFrame();
        }

        static void ForceScreenUpdateNow() {
            animationFrame->needUpdateScreen = true;
            animationFrame->update();
        }

        OSAnimationFrame();
        ~OSAnimationFrame();

        void update();
        void stopDisplayLinkIfNeed();
    private:
        static OSAnimationFrame* animationFrame;

        CVDisplayLinkRef displayLink;
        std::vector<FrameRequestCallback>* callbackList;
        bool needUpdateScreen = false;
        bool hasNextFrame = false;
        std::mutex locker;



        void requestNextFrame() {
            if (hasNextFrame) {
                return;
            }
            locker.lock();
            hasNextFrame = true;
            locker.unlock();
            if (!CVDisplayLinkIsRunning(displayLink)) {
                CVDisplayLinkStart(displayLink);
            }
        }
    };
}

#endif  //CYDER_OSANIMATIONFRAME_H