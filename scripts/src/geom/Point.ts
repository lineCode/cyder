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