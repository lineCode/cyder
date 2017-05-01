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
    /**
     * @internal
     * load image from url.
     * @param url The URL of the image to be loaded.
     * @param callback The callback function that receive the loaded image data.
     * @param thisArg The value of this provided for the call to the callback function.
     */
    export declare function loadImageFromURL(url:string, callback:(data:Image) => void, thisArg:any);

    /**
     * @internal
     * load image from byte array.
     * @param bytes The byte array of image to be loaded.
     * @param callback The callback function that receive the loaded image data.
     * @param thisArg The value of this provided for the call to the callback function.
     */
    export declare function loadImageFromBytes(bytes:ArrayBuffer, callback:(data:Image) => void, thisArg:any);

    /**
     * @internal
     */
    let ioErrorEvent = new IOErrorEvent(IOErrorEvent.IO_ERROR);

    /**
     * @internal
     * The Loader class is used to load image (JPG, PNG, or GIF) files. Use the load() method to initiate loading.
     * The loaded image data is in the data property of ImageLoader.
     * @event Event.COMPLETE Emitted when the net request is complete.
     * @event IOErrorEvent.IO_ERROR Emitted when the net request is failed.
     */
    export class ImageLoader extends cyder.EventEmitter {
        /**
         * Creates a ImageLoader instance.
         */
        public constructor() {
            super();
        }

        /**
         * The data received from the load operation.
         */
        public data:Image = null;

        /**
         * @private
         */
        private currentURL:string;

        /**
         * start a load operation。<br/>
         * Note: Calling this method for an already active request (one for which load() has already been called) will abort
         * the last load operation immediately.
         * @param url The URL of the image to be loaded.
         */
        public load(url:string):void {
            this.currentURL = url;
            cyder.loadImageFromURL(url, this.onLoadFinish, this);
        }

        /**
         * Loads image from binary data stored in a ArrayBuffer object.
         * @param bytes The binary data of the image to be loaded.
         */
        public loadBytes(bytes:ArrayBuffer):void {
            this.currentURL = "";
            if (bytes.byteLength == 0) {
                throw new Error("The ArrayBuffer parameter in ImageLoader.loadBytes() must have length greater than 0.");
            }
            cyder.loadImageFromBytes(bytes, this.onLoadFinish, this);
        }

        /**
         * @private
         */
        private onLoadFinish(data:Image) {
            this.data = data;
            if (data) {
                this.emitWith(Event.COMPLETE);
            }
            else {
                let errorText = this.currentURL ? "Stream Error. URL: " + this.currentURL : "Loaded file is an unknown type.";
                ioErrorEvent.text = errorText;
                if (this.hasListener(IOErrorEvent.IO_ERROR)) {
                    this.emit(ioErrorEvent);
                    ioErrorEvent.text = "";
                }
                else {
                    throw new URIError(errorText);
                }

            }

        }
    }
}


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

ImageLoader = cyder.ImageLoader;
