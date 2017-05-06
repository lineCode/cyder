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
 * Image is an abstraction for drawing a rectangle of pixels, though the particular type of image could be actually
 * storing its data on the GPU, or as drawing commands, ready to be played back into another canvas.<br/>
 * Calls to any method or property of a Image object throw an error if the Image object has been disposed of via
 * dispose().<br/>
 * It is created by the Canvas.makeImageSnapshot() method, or loading a bitmap file with the ImageLoader class.
 */
interface Image {
    /**
     * The width of the image in pixels.
     */
    readonly width:number;

    /**
     * The height of the image in pixels.
     */
    readonly height:number;

    /**
     * Indicates whether the image supports per-pixel transparency.
     */
    readonly transparent:boolean;

    /**
     * Immediately frees memory that is used to store the Image object. <br/>
     * When the dispose() method is called on an image, the width and height of the image are set to 0, and the
     * Image object is no longer usable. All subsequent calls to methods or properties of this Image instance fail,
     * and an exception is thrown. However, Image.dispose() does not garbage collect the Image object itself. The
     * memory occupied by the actual Image object is released at the time the Image object is collected by the garbage
     * collector. <br/>
     * Use the dispose() method only when you really need to free memory immediately, otherwise it is unnecessary.
     * Because the memory used to store the Image object will be automatically freed at the time the Image object is
     * garbage collected.
     */
    dispose():void;

    /**
     * Compresses this Image object using the format specified by the type parameter and returns an ArrayBuffer object.
     * @param type A string indicating the image format. The default type is "image/png".
     * @param quality A number between 0 and 1 indicating image quality if the requested type is "image/jpeg"
     * or "image/webp". If this argument is anything else, the default value for image quality is used. The default
     * value is 0.92 for "image/jpeg", and 0.8 for "image/webp". Other arguments are ignored.
     * @returns A ArrayBuffer containing the encoded image.
     */
    encode(type?:string, quality?:number):ArrayBuffer;

    /**
     * Generates an ImageData object from a rectangular region of pixel data.
     * @param x The x coordinate of the upper left corner of the rectangle from which the pixel data will be extracted.
     * @param y The y coordinate of the upper left corner of the rectangle from which the pixel data will be extracted.
     * @param width The width of the rectangle from which the pixel data will be extracted.
     * @param height The height of the rectangle from which the pixel data will be extracted.
     * @returns An ImageData object containing the pixel data for the given rectangle of the Image.
     */
    getImageData(x:number, y:number, width:number, height:number):ImageData;

    /**
     * Returns a new image that is a subset of this image. The underlying implementation may share the pixels, or it
     * may make a copy. It depends on the value passed in the sharePixels parameter.
     * @param x The x coordinate of the upper left corner of the subset rectangle.
     * @param y The y coordinate of the upper left corner of the subset rectangle.
     * @param width The width of the subset rectangle.
     * @param height The height of the subset rectangle.
     * @param sharePixels Indicates whether the underlying implementation should share the pixels, otherwise make a copy.
     * The default value is true.
     * @returns If subset does not intersect the bounds of this image, or the copy/share cannot be made, null will
     * be returned.
     */
    makeSubset(x:number, y:number, width:number, height:number, sharePixels?:boolean):Image;

    /**
     * Returns a data URI containing a representation of the image in the format specified by the type parameter.
     * (defaults to PNG)
     * @param type A string indicating the image format. The default type is "image/png".
     * @param quality A number between 0 and 1 indicating image quality if the requested type is "image/jpeg"
     * or "image/webp". If this argument is anything else, the default value for image quality is used. The default
     * value is 0.92 for "image/jpeg", and 0.8 for "image/webp". Other arguments are ignored.
     * @returns A string containing the data URI of encoded image.
     */
    toDataURL(type?:string, quality?:number):string;
}