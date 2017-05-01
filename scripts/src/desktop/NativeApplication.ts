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
 * The NativeApplication interface provides application information, application-wide functions, and emits application-level events. The NativeApplication object is a singleton object, created automatically at startup.
 * Get the NativeApplication instance with the global 'nativeApplication' property.
 */
interface NativeApplication extends EventEmitter {
    /**
     * The standard output stream is the default destination of output for applications. In most systems, it is usually
     * directed by default to the text console (generally, on the screen).
     */
    standardOutput:WritableStream;

    /**
     * The standard error stream is the default destination for error messages and other diagnostic warnings. Like stdout,
     * it is usually also directed by default to the text console (generally, on the screen).
     */
    standardError:WritableStream;

    /**
     * The active application window.<br/>
     * If the active desktop window does not belong to this application, or there is no active window, activeWindow is null.
     */
    activeWindow:NativeWindow;

    /**
     * An array containing all the open native windows of this application.
     */
    openedWindows:NativeWindow[];
}