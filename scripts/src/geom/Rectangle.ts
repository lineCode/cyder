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