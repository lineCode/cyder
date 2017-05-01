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
 * The NativeWindow class provides an interface for creating and controlling native desktop windows.
 * NativeWindow objects will not be garbage collected after the window constructor has been called and before close()
 * has been called. It is the responsibility of the application to close its own windows.
 */
interface NativeWindow extends EventEmitter {
    /**
     * The Canvas object for this window. Anything drawn to it will show on the NativeWindow.
     */
    canvas:Canvas;
    /**
     * Activates this window.<br/>
     * Activating a window will:<br/>
     * 1. Make the window visible<br/>
     * 2. Bring the window to the front<br/>
     * 3. Give the window keyboard and mouse focus<br/>
     */
    activate():void;
}

declare let NativeWindow:{
    prototype:NativeWindow;
    new():NativeWindow;
}