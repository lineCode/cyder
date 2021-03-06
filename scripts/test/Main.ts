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
        // let image = canvas.makeImageSnapshot();
        // let subImage = image.makeSubset(190,190,40,40);
        // console.log(subImage.toDataURL());
        console.log(context, canvas.width, canvas.height);
    }

    nativeApplication.emitWith("callback");

    // requestAnimationFrame(onTick);
}

let loader = new ImageLoader();
loader.on(Event.COMPLETE, function (event:Event):void {
    let image = loader.data;
    let canvas = new Canvas(250, 250);
    let context = canvas.getContext("2d");
    context.drawImage(image, 5, 5);
    let canvasImage = canvas.makeImageSnapshot();
    console.log(canvasImage.toDataURL());
}, null);
loader.load("test.png");

HTMLCanvasElement