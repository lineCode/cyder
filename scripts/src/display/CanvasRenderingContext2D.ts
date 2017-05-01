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
 * The CanvasRenderingContext2D interface is used for drawing rectangles, text, images and other objects onto the render
 * object. It provides the 2D rendering context for the drawing surface of a render object.<br/>
 * To get an object of this interface, call getContext() on a render object, supplying "2d" as the contextType argument.
 */
interface CanvasRenderingContext2D {
    /**
     * A read-only back-reference to the associated render object.
     */
    readonly canvas:Canvas;
    /**
     * Alpha value that is applied to shapes and images before they are composited onto the canvas.
     * @default 1.0
     */
    globalAlpha:number;
    /**
     * Sets the type of compositing operation to apply when drawing new shapes, where type is a string identifying which
     * of the compositing or blending mode operations to use.
     * @default "source-over"
     */
    globalCompositeOperation:string;
    /**
     * Image smoothing mode; if disabled, images will not be smoothed if scaled.
     * @default false
     */
    imageSmoothingEnabled:boolean;
    /**
     * Draws an image onto the canvas.
     * @param image An image to draw into the context.
     * @param targetX The X coordinate in the destination canvas at which to place the top-left corner of the source image.
     * @param targetY The Y coordinate in the destination canvas at which to place the top-left corner of the source image.
     */
    drawImage(image:CanvasImageSource, targetX:number, targetY:number);
    /**
     * Draws an image onto the canvas.
     * @param image An image to draw into the context.
     * @param targetX The X coordinate in the destination canvas at which to place the top-left corner of the source image.
     * @param targetY The Y coordinate in the destination canvas at which to place the top-left corner of the source image.
     * @param targetWidth The width to draw the image in the destination canvas. This allows scaling of the drawn image.
     * If not specified, the image is not scaled in width when drawn.
     * @param targetHeight The height to draw the image in the destination canvas. This allows scaling of the drawn image.
     * If not specified, the image is not scaled in height when drawn.
     */
    drawImage(image:CanvasImageSource, targetX:number, targetY:number, targetWidth:number, targetHeight:number);
    /**
     * Draws an image onto the canvas.
     * @param image An image to draw into the context.
     * @param sourceX The X coordinate of the top left corner of the sub-rectangle of the source image to draw into the
     * destination context.
     * @param sourceY The Y coordinate of the top left corner of the sub-rectangle of the source image to draw into the
     * destination context.
     * @param sourceWidth The width of the sub-rectangle of the source image to draw into the destination context.
     * @param sourceHeight The height of the sub-rectangle of the source image to draw into the destination context.
     * @param targetX The X coordinate in the destination canvas at which to place the top-left corner of the source image.
     * @param targetY The Y coordinate in the destination canvas at which to place the top-left corner of the source image.
     * @param targetWidth The width to draw the image in the destination canvas. This allows scaling of the drawn image.
     * If not specified, the image is not scaled in width when drawn.
     * @param targetHeight The height to draw the image in the destination canvas. This allows scaling of the drawn image.
     * If not specified, the image is not scaled in height when drawn.
     */
    drawImage(image:CanvasImageSource, sourceX:number, sourceY:number, sourceWidth:number, sourceHeight:number,
              targetX:number, targetY:number, targetWidth:number, targetHeight:number):void;
}