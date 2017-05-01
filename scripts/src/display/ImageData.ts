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
 * The ImageData class represents the underlying pixel data of an area of a Canvas or an Image. It is created using the ImageData()
 * constructor or creator methods on the CanvasRenderingContext2D object associated with a canvas: createImageData() and
 * getImageData(). It can also be used to set a part of the canvas by using putImageData().
 */
class ImageData {
    /**
     * The ImageData() constructor returns a newly instantiated ImageData object build from the typed array given and
     * having the specified width and height.
     * @param width An unsigned long representing the width of the represented image.
     * @param height An unsigned long representing the height of the represented image.
     */
    public constructor(width:number, height:number);
    /**
     * The ImageData() constructor returns a newly instantiated ImageData object build from the typed array given and
     * having the specified width and height.
     * @param array A Uint8ClampedArray containing the underlying pixel representation of the image. If no such array is
     * given, an image with a transparent black rectangle of the given dimension will be created.
     * @param width An unsigned long representing the width of the represented image.
     * @param height An unsigned long representing the height of the represented image. This value is optional if an
     * array is given: it will be inferred from its size and the given width.
     */
    public constructor(array:Uint8ClampedArray, width:number, height?:number);
    public constructor(array:any, width:number, height?:number) {
        if (arguments.length == 2) {
            this.data = new Uint8ClampedArray(4 * width * height);
            this.width = <number>array;
            this.height = width;
        } else {
            if (!(array instanceof Uint8ClampedArray)) {
                throw new TypeError("Failed to construct 'ImageData': parameter 1 is not of type 'Uint8ClampedArray'.");
            }
            this.data = array;
            this.width = width;
            if (height === undefined) {
                this.height = array.length * 0.25 / width;
            } else {
                if (height * width * 4 != array.length) {
                    throw new Error("Failed to construct 'ImageData': The input data length is not equal to (4 * width * height).");
                }
                this.height = height;
            }
        }
    }

    /**
     * A Uint8ClampedArray representing a one-dimensional array containing the data in the RGBA order, with integer
     * values between 0 and 255 (included).
     */
    public readonly data:Uint8ClampedArray;
    /**
     * An unsigned long representing the actual height, in pixels, of the ImageData.
     */
    public readonly height:number;
    /**
     * An unsigned long representing the actual width, in pixels, of the ImageData.
     */
    public readonly width:number;
}


