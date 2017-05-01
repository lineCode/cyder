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
 * The Loader class is used to load image (JPG, PNG, or GIF) files. Use the load() method to initiate loading.
 * The loaded image data is in the data property of ImageLoader.
 * @event Event.COMPLETE Emitted when the net request is complete.
 * @event IOErrorEvent.IO_ERROR Emitted when the net request is failed.
 */
interface ImageLoader extends EventEmitter {
    /**
     * The data received from the load operation.
     */
    data:Image;

    /**
     * start a load operation。<br/>
     * Note: Calling this method for an already active request (one for which load() has already been called) will abort
     * the last load operation immediately.
     * @param url The URL of the image to be loaded.
     */
    load(url:string):void;

    /**
     * Loads image from binary data stored in a ArrayBuffer object.
     * @param bytes The binary data of the image to be loaded.
     */
    loadBytes(bytes:ArrayBuffer):void;
}

let ImageLoader:{
    /**
     * Creates a ImageLoader instance.
     */
    new():ImageLoader;
};