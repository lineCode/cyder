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



#include "OSAnimationFrame.h"
#include "OSApplication.h"
#include "utils/GetTimer.h"

namespace cyder {
    unsigned long AnimationFrame::Request(FrameRequestCallback callback) {
        return OSAnimationFrame::Request(callback);
    }

    void AnimationFrame::Cancel(unsigned long handle) {
        OSAnimationFrame::Cancel(handle);
    }


    OSAnimationFrame* OSAnimationFrame::animationFrame = nullptr;

    static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* inNow,
                                          const CVTimeStamp* inOutputTime, CVOptionFlags flagsIn,
                                          CVOptionFlags* flagsOut, void* displayLinkContext) {
        OSAnimationFrame* animationFrame = static_cast<OSAnimationFrame*>(displayLinkContext);
        dispatch_sync(dispatch_get_main_queue(), ^{
            animationFrame->update();
        });
        animationFrame->stopDisplayLinkIfNeed();
        return 0;
    }

    OSAnimationFrame::OSAnimationFrame() : callbackList(new std::vector<FrameRequestCallback>()) {
        animationFrame = this;
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
        CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, this);
    }

    OSAnimationFrame::~OSAnimationFrame() {
        delete callbackList;
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
        animationFrame = nullptr;
    }

    void OSAnimationFrame::stopDisplayLinkIfNeed() {
        locker.lock();
        if (!hasNextFrame) {
            CVDisplayLinkStop(displayLink);
        }
        locker.unlock();
    }

    void OSAnimationFrame::update() {
        hasNextFrame = false;
        if (callbackList->size()) {
            std::vector<FrameRequestCallback> list;
            callbackList->swap(list);
            double timestamp = getTimer();
            for (const auto& callback : list) {
                callback(timestamp);
            }
        }
        if (needUpdateScreen) {
            needUpdateScreen = false;
            auto app = static_cast<OSApplication*>(Application::application);
            for (const auto& window : *(app->openedWindows())) {
                window->screenBuffer()->flush();
            }
        }
    }
}