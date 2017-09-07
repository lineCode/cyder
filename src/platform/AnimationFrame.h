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

#ifndef CYDER_TICKER_H
#define CYDER_TICKER_H

#include <functional>

namespace cyder {

    typedef std::function<void(double timestamp)> FrameRequestCallback;

    class AnimationFrame {
    public:
        /**
         * The AnimationFrame::Request() method tells the runtime that you wish to perform an animation and requests that
         * the runtime call a specified function to update an animation before the next repaint. The method takes as an
         * argument a callback to be invoked before the repaint.
         * @param callback A parameter specifying a function to call when it's time to update your animation for the next
         * repaint. The callback has one single argument, a high-resolution timestamp, which indicates the current time
         * for when requestAnimationFrame starts to fire callbacks.
         * @returns A integer value, the request id, that uniquely identifies the entry in the callback list. You can pass
         * this value to AnimationFrame::Cancel() to cancel the refresh callback request.
         */
        static size_t Request(FrameRequestCallback callback);

        /**
         * Cancels an animation frame request previously scheduled through a call to AnimationFrame::Request().
         * @param handle The ID value returned by the call to AnimationFrame::Request() that requested the callback.
         */
        static void Cancel(unsigned long handle);
    };


} // namespace cyder

#endif //CYDER_TICKER_H
