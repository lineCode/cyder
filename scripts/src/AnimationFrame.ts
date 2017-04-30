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

/**
 * @internal
 */
namespace cyder {

    let callbackList:FrameRequestCallback[] = [];
    let requested = false;

    export declare function requestFrame():void;

    export function updateFrame(timestamp:number):void {
        requested = false;
        if (callbackList.length == 0) {
            return;
        }
        let list = callbackList;
        callbackList = [];
        for (let callback of list) {
            callback(timestamp);
        }
    }

    function requestAnimationFrame(callback:FrameRequestCallback):number {
        if (typeof callback != "function") {
            throw new Error("The callback provided as parameter 1 is not a function.");
        }
        callbackList.push(callback);
        if (!requested) {
            requested = true;
            cyder.requestFrame();
        }
        return callbackList.length - 1;
    }

    function cancelAnimationFrame(handle:number):void {
        if (handle >= 0 && handle < callbackList.length) {
            callbackList.splice(handle, 1);
        }
    }

    global.requestAnimationFrame = requestAnimationFrame;
    global.cancelAnimationFrame = cancelAnimationFrame;
}
