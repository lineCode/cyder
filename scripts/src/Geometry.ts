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
 * The Point object represents a location in a two-dimensional coordinate system, where x represents the horizontal
 * axis and y represents the vertical axis.
 */
class Point {

    /**
     * Returns the distance between pt1 and pt2.
     * @param p1 The first point.
     * @param p2 The second point.
     * @returns The distance between the first and second points.
     */
    public static distance(p1:Point, p2:Point):number {
        return Math.sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }

    /**
     * Determines a point between two specified points.
     * The parameter f determines where the new interpolated point is located relative to the two end points specified
     * by parameters pt1 and pt2. The closer the value of the parameter f is to 1.0, the closer the interpolated point
     * is to the first point (parameter pt1). The closer the value of the parameter f is to 0, the closer the interpolated
     * point is to the second point (parameter pt2).
     * @param pt1 The first point.
     * @param pt2 The second point.
     * @param f The level of interpolation between the two points. Indicates where the new point will be, along the
     * line between pt1 and pt2. If f=1, pt1 is returned; if f=0, pt2 is returned.
     * @returns The new interpolated point.
     */
    public static interpolate(pt1:Point, pt2:Point, f:number):Point {
        let f1:number = 1 - f;
        return new Point(pt1.x * f + pt2.x * f1, pt1.y * f + pt2.y * f1);
    }

    /**
     * Converts a pair of polar coordinates to a Cartesian point coordinate.
     * @param len The length coordinate of the polar pair.
     * @param angle The angle, in radians, of the polar pair.
     */
    public static polar(len:number, angle:number):Point {
        return new Point(len * cyder.cos(angle / cyder.DEG_TO_RAD), len * cyder.sin(angle / cyder.DEG_TO_RAD));
    }


    /**
     * Creates a new point. If you pass no parameters to this method, a point is created at (0,0).
     * @param x The horizontal coordinate.
     * @param y The vertical coordinate.
     */
    public constructor(x:number = 0, y:number = 0) {
        this.x = x;
        this.y = y;
    }

    /**
     * The horizontal coordinate.
     * @default 0
     */
    public x:number;
    /**
     * The vertical coordinate.
     * @default 0
     */
    public y:number;

    /**
     * The length of the line segment from (0,0) to this point.
     */
    public get length():number {
        return Math.sqrt(this.x * this.x + this.y * this.y);
    }

    /**
     * Sets the members of Point to the specified values
     * @param x The horizontal coordinate.
     * @param y The vertical coordinate.
     */
    public setTo(x:number, y:number):Point {
        this.x = x;
        this.y = y;
        return this;
    }

    /**
     * Copies all of the point data from the source Point object into the calling Point object.
     * @param sourcePoint The Point object from which to copy the data.
     */
    public copyFrom(sourcePoint:Point):void {
        this.x = sourcePoint.x;
        this.y = sourcePoint.y;
    }

    /**
     * Scales the line segment between (0,0) and the current point to a set length.
     * @param thickness The scaling value. For example, if the current point is (0,5), and you normalize it to 1, the
     * point returned is at (0,1).
     */
    public normalize(thickness:number):void {
        if (this.x != 0 || this.y != 0) {
            let relativeThickness:number = thickness / this.length;
            this.x *= relativeThickness;
            this.y *= relativeThickness;
        }
    }

    /**
     * Offsets the Point object by the specified amount. The value of dx is added to the original value of x to create
     * the new x value. The value of dy is added to the original value of y to create the new y value.
     * @param dx The amount by which to offset the horizontal coordinate, x.
     * @param dy The amount by which to offset the vertical coordinate, y.
     */
    public offset(dx:number, dy:number):void {
        this.x += dx;
        this.y += dy;
    }

    /**
     * Adds the coordinates of another point to the coordinates of this point to create a new point.
     * @param v The point to be added.
     * @returns The new point.
     */
    public add(v:Point):Point {
        return new Point(this.x + v.x, this.y + v.y);
    }

    /**
     * Subtracts the coordinates of another point from the coordinates of this point to create a new point.
     * @param v The point to be subtracted.
     * @returns The new point.
     */
    public subtract(v:Point):Point {
        return new Point(this.x - v.x, this.y - v.y);
    }

    /**
     * Creates a copy of this Point object.
     */
    public clone():Point {
        return new Point(this.x, this.y);
    }


    /**
     * Determines whether two points are equal. Two points are equal if they have the same x and y values.
     * @param toCompare The point to be compared.
     * @returns A value of true if the object is equal to this Point object; false if it is not equal.
     */
    public equals(toCompare:Point):boolean {
        return this.x == toCompare.x && this.y == toCompare.y;
    }

    /**
     * Returns a string that contains the values of the x and y coordinates. The string has the form "(x=x, y=y)", so
     * calling the toString() method for a point at 23,17 would return "(x=23, y=17)".
     * @returns The string representation of the coordinates.
     */
    public toString():string {
        return "(x=" + this.x + ", y=" + this.y + ")";
    }
}


/**
 * A Rectangle object is an area defined by its position, as indicated by its top-left corner point (x, y) and by its
 * width and its height.<br/>
 * The x, y, width, and height properties of the Rectangle class are independent of each other; changing the value of
 * one property has no effect on the others. However, the right and bottom properties are integrally related to those
 * four properties. For example, if you change the value of the right property, the value of the width property changes;
 * if you change the bottom property, the value of the height property changes.
 */
class Rectangle {

    /**
     * Creates a new Rectangle object with the top-left corner specified by the x and y parameters and with the specified
     * width and height parameters.
     * @param x The x coordinate of the top-left corner of the rectangle.
     * @param y The y coordinate of the top-left corner of the rectangle.
     * @param width The width of the rectangle, in pixels.
     * @param height The height of the rectangle, in pixels.
     */
    public constructor(x:number = 0, y:number = 0, width:number = 0, height:number = 0) {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
    }

    /**
     * The x coordinate of the top-left corner of the rectangle.
     * @default 0
     */
    public x:number;
    /**
     * The y coordinate of the top-left corner of the rectangle.
     * @default 0
     */
    public y:number;
    /**
     * The width of the rectangle, in pixels.
     * @default 0
     */
    public width:number;
    /**
     * 矩形的高度（以像素为单位）。
     * @default 0
     */
    public height:number;

    /**
     * The sum of the x and width properties.
     */
    public get right():number {
        return this.x + this.width;
    }

    public set right(value:number) {
        this.width = value - this.x;
    }

    /**
     * The sum of the y and height properties.
     */
    public get bottom():number {
        return this.y + this.height;
    }

    public set bottom(value:number) {
        this.height = value - this.y;
    }

    /**
     * The x coordinate of the top-left corner of the rectangle. Changing the left property of a Rectangle object has
     * no effect on the y and height properties. However it does affect the width property, whereas changing the x value
     * does not affect the width property.
     * The value of the left property is equal to the value of the x property.
     */
    public get left():number {
        return this.x;
    }

    public set left(value:number) {
        this.width += this.x - value;
        this.x = value;
    }

    /**
     * The y coordinate of the top-left corner of the rectangle. Changing the top property of a Rectangle object has
     * no effect on the x and width properties. However it does affect the height property, whereas changing the y
     * value does not affect the height property.<br/>
     * The value of the top property is equal to the value of the y property.
     */
    public get top():number {
        return this.y;
    }

    public set top(value:number) {
        this.height += this.y - value;
        this.y = value;
    }

    /**
     * The location of the Rectangle object's top-left corner, determined by the x and y coordinates of the point.
     */
    public get topLeft():Point {
        return new Point(this.left, this.top);
    }

    public set topLeft(value:Point) {
        this.top = value.y;
        this.left = value.x;
    }

    /**
     * The location of the Rectangle object's bottom-right corner, determined by the values of the right and bottom properties.
     */
    public get bottomRight():Point {
        return new Point(this.right, this.bottom);
    }

    public set bottomRight(value:Point) {
        this.bottom = value.y;
        this.right = value.x;
    }

    /**
     * Sets the members of Rectangle to the specified values
     * @param x The x coordinate of the top-left corner of the rectangle.
     * @param y The y coordinate of the top-left corner of the rectangle.
     * @param width The width of the rectangle, in pixels.
     * @param height The height of the rectangle, in pixels.
     */
    public setTo(x:number, y:number, width:number, height:number):Rectangle {
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        return this;
    }


    /**
     * Copies all of rectangle data from the source Rectangle object into the calling Rectangle object.
     * @param sourceRect The Rectangle object from which to copy the data.
     */
    public copyFrom(sourceRect:Rectangle):Rectangle {
        this.x = sourceRect.x;
        this.y = sourceRect.y;
        this.width = sourceRect.width;
        this.height = sourceRect.height;
        return this;
    }

    /**
     * Determines whether the specified point is contained within the rectangular region defined by this Rectangle object.
     * @param x The x coordinate (horizontal position) of the point.
     * @param y The y coordinate (vertical position) of the point.
     * @returns A value of true if the Rectangle object contains the specified point; otherwise false.
     */
    public contains(x:number, y:number):boolean {
        return this.x <= x &&
            this.x + this.width >= x &&
            this.y <= y &&
            this.y + this.height >= y;
    }

    /**
     * If the Rectangle object specified in the toIntersect parameter intersects with this Rectangle object, returns
     * the area of intersection as a Rectangle object. If the rectangles do not intersect, this method returns an empty
     * Rectangle object with its properties set to 0.
     * @param toIntersect The Rectangle object to compare against to see if it intersects with this Rectangle object.
     * @returns A Rectangle object that equals the area of intersection. If the rectangles do not intersect, this method
     * returns an empty Rectangle object; that is, a rectangle with its x, y, width, and height properties set to 0.
     */
    public intersection(toIntersect:Rectangle):Rectangle {
        let x0 = this.x;
        let y0 = this.y;
        let x1 = toIntersect.x;
        let y1 = toIntersect.y;
        let l = Math.max(x0, x1);
        let r = Math.min(x0 + this.width, x1 + toIntersect.width);
        if (l <= r) {
            let t = Math.max(y0, y1);
            let b = Math.min(y0 + this.height, y1 + toIntersect.height);
            if (t <= b) {
                return new Rectangle(l, t, r - l, b - t);
            }
        }
        return new Rectangle();
    }

    /**
     * Determines whether the object specified in the toIntersect parameter intersects with this Rectangle object.
     * This method checks the x, y, width, and height properties of the specified Rectangle object to see if it
     * intersects with this Rectangle object.
     * @param toIntersect The Rectangle object to compare against this Rectangle object.
     * @returns A value of true if the specified object intersects with this Rectangle object; otherwise false.
     */
    public intersects(toIntersect:Rectangle):boolean {
        return Math.max(this.x, toIntersect.x) <= Math.min(this.right, toIntersect.right)
            && Math.max(this.y, toIntersect.y) <= Math.min(this.bottom, toIntersect.bottom);
    }

    /**
     * Determines whether or not this Rectangle object is empty.
     * @returns A value of true if the Rectangle object's width or height is less than or equal to 0; otherwise false.
     */
    public isEmpty():boolean {
        return this.width <= 0 || this.height <= 0;
    }

    /**
     * Sets all of the Rectangle object's properties to 0. A Rectangle object is empty if its width or height is less
     * than or equal to 0.
     */
    public setEmpty():void {
        this.x = 0;
        this.y = 0;
        this.width = 0;
        this.height = 0;
    }

    /**
     * Returns a new Rectangle object with the same values for the x, y, width, and height properties as the original
     * Rectangle object.
     * @returns A new Rectangle object with the same values for the x, y, width, and height properties as the original
     * Rectangle object.
     */
    public clone():Rectangle {
        return new Rectangle(this.x, this.y, this.width, this.height);
    }

    /**
     * Determines whether the specified point is contained within the rectangular region defined by this Rectangle object.
     * This method is similar to the Rectangle.contains() method, except that it takes a Point object as a parameter.
     * @param point The point, as represented by its x and y coordinates.
     * @returns A value of true if the Rectangle object contains the specified point; otherwise false.
     */
    public containsPoint(point:Point):boolean {
        return this.x < point.x
            && this.x + this.width > point.x
            && this.y < point.y
            && this.y + this.height > point.y;
    }

    /**
     * Determines whether the Rectangle object specified by the rect parameter is contained within this Rectangle object.
     * A Rectangle object is said to contain another if the second Rectangle object falls entirely within the boundaries
     * of the first.
     * @param rect The Rectangle object being checked.
     * @returns A value of true if the Rectangle object that you specify is contained by this Rectangle object;
     * otherwise false.
     */
    public containsRect(rect:Rectangle):boolean {
        let r1 = rect.x + rect.width;
        let b1 = rect.y + rect.height;
        let r2 = this.x + this.width;
        let b2 = this.y + this.height;
        return (rect.x >= this.x) && (rect.x < r2) && (rect.y >= this.y) &&
            (rect.y < b2) && (r1 > this.x) && (r1 <= r2) && (b1 > this.y) && (b1 <= b2);
    }

    /**
     * Determines whether the object specified in the toCompare parameter is equal to this Rectangle object.<br/>
     * This method compares the x, y, width, and height properties of an object against the same properties of this
     * Rectangle object.
     * @param toCompare The rectangle to compare to this Rectangle object.
     * @returns A value of true if the object has exactly the same values for the x, y, width, and height properties
     * as this Rectangle object; otherwise false.
     */
    public equals(toCompare:Rectangle):boolean {
        if (this === toCompare) {
            return true;
        }
        return this.x === toCompare.x && this.y === toCompare.y
            && this.width === toCompare.width && this.height === toCompare.height;
    }

    /**
     * Increases the size of the Rectangle object by the specified amounts, in pixels.
     * The center point of the Rectangle object stays the same, and its size increases to the left and right by the
     * dx value, and to the top and the bottom by the dy value.
     * @param dx The value to be added to the left and the right of the Rectangle object.
     * @param dy The value to be added to the top and the bottom of the Rectangle.
     */
    public inflate(dx:number, dy:number):void {
        this.x -= dx;
        this.width += 2 * dx;
        this.y -= dy;
        this.height += 2 * dy;
    }

    /**
     * Increases the size of the Rectangle object. This method is similar to the Rectangle.inflate() method except it takes
     * a Point object as a parameter.
     * @param point The x property of this Point object is used to increase the horizontal dimension of the Rectangle object.
     * The y property is used to increase the vertical dimension of the Rectangle object.
     */
    public inflatePoint(point:Point):void {
        this.inflate(point.x, point.y);
    }

    /**
     * Adjusts the location of the Rectangle object, as determined by its top-left corner, by the specified amounts.
     * @param dx Moves the x value of the Rectangle object by this amount.
     * @param dy Moves the y value of the Rectangle object by this amount.
     */
    public offset(dx:number, dy:number):void {
        this.x += dx;
        this.y += dy;
    }

    /**
     * Adjusts the location of the Rectangle object using a Point object as a parameter. This method is similar to
     * the Rectangle.offset() method, except that it takes a Point object as a parameter.
     * @param point A Point object to use to offset this Rectangle object.
     */
    public offsetPoint(point:Point):void {
        this.offset(point.x, point.y);
    }

    /**
     * Adds two rectangles together to create a new Rectangle object, by filling in the horizontal and vertical space
     * between the two rectangles.
     * @param toUnion A Rectangle object to add to this Rectangle object.
     * @returns A new Rectangle object that is the union of the two rectangles.
     */
    public union(toUnion:Rectangle):Rectangle {
        let result = this.clone();
        if (toUnion.isEmpty()) {
            return result;
        }
        if (result.isEmpty()) {
            result.copyFrom(toUnion);
            return result;
        }
        let l:number = Math.min(result.x, toUnion.x);
        let t:number = Math.min(result.y, toUnion.y);
        result.setTo(l, t,
            Math.max(result.right, toUnion.right) - l,
            Math.max(result.bottom, toUnion.bottom) - t);
        return result;
    }

    /**
     * Builds and returns a string that lists the horizontal and vertical positions and the width and height of the Rectangle object.
     * @returns A string listing the value of each of the following properties of the Rectangle object: x, y, width, and height.
     */
    public toString():string {
        return "(x=" + this.x + ", y=" + this.y + ", width=" + this.width + ", height=" + this.height + ")";
    }
}


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