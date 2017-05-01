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
 * The Canvas object is a handle onto a raw buffer that is being managed by the screen compositor. It can be used to draw
 * graphics. For example, draw graphs, make photo compositions, create animations, or even do real-time video processing
 * or rendering. And it also can be drawn to another render object.
 */
interface Canvas {
    /**
     * Gets or sets the height of a render object.
     */
    height:number;
    /**
     * Gets or sets the width of a render object.
     */
    width:number;

    /**
     * Returns a drawing context on the render, or null if the context identifier is not supported.
     * @param contextType A string containing the context identifier defining the drawing context associated to the render.
     * @param contextAttributes You can use several context attributes when creating your rendering context.
     */
    getContext(contextType:string, contextAttributes?:{}):any;
    getContext(contextType:"2d", contextAttributes?:Canvas2DContextAttributes):CanvasRenderingContext2D;

    /**
     * Returns an image of the current state of the canvas pixels up to this point. Subsequent changes to the canvas
     * will not be reflected in this image.
     */
    makeImageSnapshot():Image;
}

declare let Canvas:{
    prototype:Canvas;
    new(width:number, height:number):Canvas;
}