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
 * An IOErrorEvent object is emitted when an error causes input or output operations to fail.
 */
class IOErrorEvent extends Event {

    /**
     * Emitted when an error causes input or output operations to fail.
     */
    public static readonly IO_ERROR:string = "ioError";

    /**
     * Creates an Event object that contains specific information about ioError events. Event objects are passed as
     * parameters to Event listeners.
     * @param type The type of the event.
     * @param cancelable Determine whether the Event object can be canceled. The default value is false.
     * @param text Text to be displayed as an error message.
     */
    public constructor(type:string, cancelable?:boolean, text:string = "") {
        super(type, cancelable);
        this.text = text;
    }

    /**
     * Text to be displayed as an error message.
     */
    public text:string;
}