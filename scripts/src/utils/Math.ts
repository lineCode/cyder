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
     */
    export let DEG_TO_RAD = Math.PI / 180;

    let sinTable = {};
    let cosTable = {};

    /**
     * @private
     */
    function createTables():void {
        for (let i = 0; i < 360; i++) {
            sinTable[i] = Math.sin(i * DEG_TO_RAD);
            cosTable[i] = Math.cos(i * DEG_TO_RAD);
        }
        sinTable[90] = 1;
        cosTable[90] = 0;
        sinTable[180] = 0;
        cosTable[180] = -1;
        sinTable[270] = -1;
        cosTable[270] = 0;
    }

    createTables();


    /**
     * @private
     */
    function sinInt(value:number):number {
        value = value % 360;
        if (value < 0) {
            value += 360;
        }
        return sinTable[value];
    }

    /**
     * @private
     */
    function cosInt(value:number):number {
        value = value % 360;
        if (value < 0) {
            value += 360;
        }
        return cosTable[value];
    }

    /**
     * @internal
     * Calculates the approximate sin value of the corresponding angle value.
     * @param value The rotation angle in radians.
     */
    export function sin(value:number):number {
        let valueFloor = Math.floor(value);
        let valueCeil = valueFloor + 1;
        let resultFloor = sinInt(valueFloor);
        if (valueFloor == value) {
            return resultFloor;
        }
        let resultCeil = sinInt(valueCeil);

        return (value - valueFloor) * resultCeil + (valueCeil - value) * resultFloor;
    }

    /**
     * @internal
     * Calculates the approximate cos value of the corresponding angle value.
     * @param value The rotation angle in radians.
     */
    export function cos(value:number):number {
        let valueFloor = Math.floor(value);
        let valueCeil = valueFloor + 1;
        let resultFloor = cosInt(valueFloor);
        if (valueFloor == value) {
            return resultFloor;
        }
        let resultCeil = cosInt(valueCeil);

        return (value - valueFloor) * resultCeil + (valueCeil - value) * resultFloor;
    }
}