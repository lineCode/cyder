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

let window:NativeWindow;

requestAnimationFrame(onTick);

function onTick(timeStamp:number):void {
    nativeApplication.on("callback", onCallback, null);

    function onCallback() {
        console.log("it works! " + performance.now() + "ms");
        window = new NativeWindow();
        window.activate();
        window = nativeApplication.activeWindow;
        let canvas = window.canvas;
        let context = canvas.getContext("2d");
        console.log(context, canvas.width, canvas.height);
    }

    nativeApplication.emitWith("callback");

    // requestAnimationFrame(onTick);
}

let loader = new ImageLoader();
loader.on(Event.COMPLETE, function (event:Event):void {
    let image = loader.data;
    console.log("Image:", image.width, image.height);
    let subset = image.makeSubset(50, 50, 10, 10);
    let data = subset.getImageData(0, 0, 10, 10);
    console.log(data.data.length, data.width, data.height);
    console.log(image.toDataURL());
    console.log(subset.toDataURL("image/png"));
    console.log("subset:", subset.width, subset.height);
    image.toDataURL();
}, null);
loader.load("test.png");