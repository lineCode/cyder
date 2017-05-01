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
 * The Matrix class represents a transformation matrix that determines how to map points from one coordinate space to
 * another. You can perform various graphical transformations on a display object by setting the properties of a Matrix
 * object, applying that Matrix object to the matrix property of a display object, These transformation functions include
 * translation (x and y repositioning), rotation, scaling, and skewing.
 */
class Matrix {

    /**
     * Creates a new Matrix object with the specified parameters.
     * @param a The value that affects the positioning of pixels along the x axis when scaling or rotating an image.
     * @param b The value that affects the positioning of pixels along the y axis when rotating or skewing an image.
     * @param c The value that affects the positioning of pixels along the x axis when rotating or skewing an image.
     * @param d The value that affects the positioning of pixels along the y axis when scaling or rotating an image..
     * @param tx The distance by which to translate each point along the x axis.
     * @param ty The distance by which to translate each point along the y axis.
     */
    constructor(a:number = 1, b:number = 0, c:number = 0, d:number = 1, tx:number = 0, ty:number = 0) {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        this.tx = tx;
        this.ty = ty;
    }

    /**
     * The value that affects the positioning of pixels along the x axis when scaling or rotating an image.
     * @default 1
     */
    public a:number;
    /**
     * The value that affects the positioning of pixels along the y axis when rotating or skewing an image.
     * @default 0
     */
    public b:number;
    /**
     * The value that affects the positioning of pixels along the x axis when rotating or skewing an image.
     * @default 0
     */
    public c:number;
    /**
     * The value that affects the positioning of pixels along the y axis when scaling or rotating an image.
     * @default 1
     */
    public d:number;
    /**
     * The distance by which to translate each point along the x axis.
     * @default 0
     */
    public tx:number;

    /**
     * The distance by which to translate each point along the y axis.
     * @default 0
     */
    public ty:number;

    /**
     * Returns a new Matrix object that is a clone of this matrix, with an exact copy of the contained object.
     */
    public clone():Matrix {
        return new Matrix(this.a, this.b, this.c, this.d, this.tx, this.ty);
    }

    /**
     * Concatenates a matrix with the current matrix, effectively combining the geometric effects of the two. In mathematical
     * terms, concatenating two matrices is the same as combining them using matrix multiplication.
     * @param other The matrix to be concatenated to the source matrix.
     */
    public concat(other:Matrix):void {
        let a = this.a * other.a;
        let b = 0.0;
        let c = 0.0;
        let d = this.d * other.d;
        let tx = this.tx * other.a + other.tx;
        let ty = this.ty * other.d + other.ty;

        if (this.b !== 0.0 || this.c !== 0.0 || other.b !== 0.0 || other.c !== 0.0) {
            a += this.b * other.c;
            d += this.c * other.b;
            b += this.a * other.b + this.b * other.d;
            c += this.c * other.a + this.d * other.c;
            tx += this.ty * other.c;
            ty += this.tx * other.b;
        }

        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        this.tx = tx;
        this.ty = ty;
    }

    /**
     * Copies all of the matrix data from the source Point object into the calling Matrix object.
     * @param other  The Matrix object from which to copy the data.
     */
    public copyFrom(other:Matrix):Matrix {
        this.a = other.a;
        this.b = other.b;
        this.c = other.c;
        this.d = other.d;
        this.tx = other.tx;
        this.ty = other.ty;
        return this;
    }

    /**
     * Sets each matrix property to a value that causes a null transformation. An object transformed by applying an
     * identity matrix will be identical to the original. After calling the identity() method, the resulting matrix
     * has the following properties: a=1, b=0, c=0, d=1, tx=0, ty=0.
     */
    public identity():void {
        this.a = this.d = 1;
        this.b = this.c = this.tx = this.ty = 0;
    }

    /**
     * Performs the opposite transformation of the original matrix. You can apply an inverted matrix to an object to
     * undo the transformation performed when applying the original matrix.
     */
    public invert():void {
        if (this.b == 0 && this.c == 0) {
            if (this.a == 0 || this.d == 0) {
                this.tx = this.ty = 0;
            }
            else {
                this.a = 1 / this.a;
                this.d = 1 / this.d;
                this.tx = -this.a * this.tx;
                this.ty = -this.d * this.ty;
            }
            return;
        }
        let determinant = this.a * this.d - this.b * this.c;
        if (determinant == 0) {
            this.identity();
            return;
        }
        determinant = 1 / determinant;
        this.a = this.d * determinant;
        this.b = -this.b * determinant;
        this.c = -this.c * determinant;
        this.d = this.a * determinant;
        this.tx = -(this.a * this.tx + this.c * this.ty);
        this.ty = -(this.b * this.tx + this.d * this.ty);
    }


    /**
     * Applies a rotation transformation to the Matrix object.
     * The rotate() method alters the a, b, c, and d properties of the Matrix object.
     * @param angle The rotation angle in radians.
     */
    public rotate(angle:number):void {
        angle = +angle;
        if (angle !== 0) {
            angle = angle / cyder.DEG_TO_RAD;
            let u = cyder.cos(angle);
            let v = cyder.sin(angle);
            let ta = this.a;
            let tb = this.b;
            let tc = this.c;
            let td = this.d;
            let ttx = this.tx;
            let tty = this.ty;
            this.a = ta * u - tb * v;
            this.b = ta * v + tb * u;
            this.c = tc * u - td * v;
            this.d = tc * v + td * u;
            this.tx = ttx * u - tty * v;
            this.ty = ttx * v + tty * u;
        }
    }

    /**
     * Applies a scaling transformation to the matrix. The x axis is multiplied by sx, and the y axis it is multiplied by sy.
     * The scale() method alters the a and d properties of the Matrix object.
     * @param sx A multiplier used to scale the object along the x axis.
     * @param sy A multiplier used to scale the object along the y axis.
     */
    public scale(sx:number, sy:number):void {
        if (sx !== 1) {
            this.a *= sx;
            this.c *= sx;
            this.tx *= sx;
        }
        if (sy !== 1) {
            this.b *= sy;
            this.d *= sy;
            this.ty *= sy;
        }
    }

    /**
     * Sets the members of Matrix to the specified values
     * @param a The value that affects the positioning of pixels along the x axis when scaling or rotating an image.
     * @param b The value that affects the positioning of pixels along the y axis when rotating or skewing an image.
     * @param c The value that affects the positioning of pixels along the x axis when rotating or skewing an image.
     * @param d The value that affects the positioning of pixels along the y axis when scaling or rotating an image..
     * @param tx The distance by which to translate each point along the x axis.
     * @param ty The distance by which to translate each point along the y axis.
     */
    public setTo(a:number, b:number, c:number, d:number, tx:number, ty:number):Matrix {
        this.a = a;
        this.b = b;
        this.c = c;
        this.d = d;
        this.tx = tx;
        this.ty = ty;
        return this;
    }

    /**
     * Returns the result of applying the geometric transformation represented by the Matrix object to the specified point.
     * @param pointX The x coordinate for which you want to get the result of the Matrix transformation.
     * @param pointY The y coordinate for which you want to get the result of the Matrix transformation.
     * @returns The point resulting from applying the Matrix transformation.
     */
    public transformPoint(pointX:number, pointY:number):Point {
        let x = this.a * pointX + this.c * pointY + this.tx;
        let y = this.b * pointX + this.d * pointY + this.ty;
        return new Point(x, y);
    }

    /**
     * Translates the matrix along the x and y axes, as specified by the dx and dy parameters.
     * @param dx The amount of movement along the x axis to the right, in pixels.
     * @param dy The amount of movement down along the y axis, in pixels.
     */
    public translate(dx:number, dy:number):void {
        this.tx += dx;
        this.ty += dy;
    }

    /**
     * Given a point in the pretransform coordinate space, returns the coordinates of that point after the transformation occurs.
     * Unlike the standard transformation applied using the transformPoint() method, the deltaTransformPoint() method's
     * transformation does not consider the translation parameters tx and ty.
     * @param point The point for which you want to get the result of the matrix transformation.
     * @returns The point resulting from applying the matrix transformation.
     */
    public deltaTransformPoint(point:Point):Point {
        let x = this.a * point.x + this.c * point.y;
        let y = this.b * point.x + this.d * point.y;
        return new Point(x, y);
    }

    /**
     * Determines whether two matrixes are equal.
     * @param other The matrix to be compared.
     * @returns A value of true if the object is equal to this Matrix object; false if it is not equal.
     */
    public equals(other:Matrix):boolean {
        return this.a == other.a && this.b == other.b &&
            this.c == other.c && this.d == other.d &&
            this.tx == other.tx && this.ty == other.ty;
    }

    /**
     * Includes parameters for scaling, rotation, and translation. When applied to a matrix it sets the matrix's values
     * based on those parameters.
     * @param scaleX The factor by which to scale horizontally.
     * @param scaleY The factor by which scale vertically.
     * @param rotation The amount to rotate, in radians.
     * @param tx The number of pixels to translate (move) to the right along the x axis.
     * @param ty The number of pixels to translate (move) down along the y axis.
     */
    public createBox(scaleX:number, scaleY:number, rotation:number = 0, tx:number = 0, ty:number = 0):void {
        if (rotation !== 0) {
            rotation = rotation / cyder.DEG_TO_RAD;
            let u = cyder.cos(rotation);
            let v = cyder.sin(rotation);
            this.a = u * scaleX;
            this.b = v * scaleY;
            this.c = -v * scaleX;
            this.d = u * scaleY;
        } else {
            this.a = scaleX;
            this.b = 0;
            this.c = 0;
            this.d = scaleY;
        }
        this.tx = tx;
        this.ty = ty;
    }

    /**
     * Applies the geometric transformation represented by the Matrix object to the specified rectangle.
     * @param bounds The rectangle for which you want to get the result of the matrix transformation.
     */
    public transformBounds(bounds:Rectangle):void {
        let a = this.a;
        let b = this.b;
        let c = this.c;
        let d = this.d;
        let tx = this.tx;
        let ty = this.ty;

        let x = bounds.x;
        let y = bounds.y;
        let xMax = x + bounds.width;
        let yMax = y + bounds.height;

        let x0 = a * x + c * y + tx;
        let y0 = b * x + d * y + ty;
        let x1 = a * xMax + c * y + tx;
        let y1 = b * xMax + d * y + ty;
        let x2 = a * xMax + c * yMax + tx;
        let y2 = b * xMax + d * yMax + ty;
        let x3 = a * x + c * yMax + tx;
        let y3 = b * x + d * yMax + ty;

        let tmp = 0;

        if (x0 > x1) {
            tmp = x0;
            x0 = x1;
            x1 = tmp;
        }
        if (x2 > x3) {
            tmp = x2;
            x2 = x3;
            x3 = tmp;
        }

        bounds.x = Math.floor(x0 < x2 ? x0 : x2);
        bounds.width = Math.ceil((x1 > x3 ? x1 : x3) - bounds.x);

        if (y0 > y1) {
            tmp = y0;
            y0 = y1;
            y1 = tmp;
        }
        if (y2 > y3) {
            tmp = y2;
            y2 = y3;
            y3 = tmp;
        }

        bounds.y = Math.floor(y0 < y2 ? y0 : y2);
        bounds.height = Math.ceil((y1 > y3 ? y1 : y3) - bounds.y);
    }


    /**
     * Returns a text value listing the properties of the Matrix object.
     * @returns A string containing the values of the properties of the Matrix object: a, b, c, d, tx, and ty.
     */
    public toString():string {
        return "(a=" + this.a + ", b=" + this.b + ", c=" + this.c + ", d=" + this.d + ", tx=" + this.tx + ", ty=" + this.ty + ")";
    }
}