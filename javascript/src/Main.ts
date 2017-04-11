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
interface OutputStream {
    /**
     * Write a string message to OutputStream.
     */
    write(message:string):void;
}

/**
 * Returns the number of milliseconds since the runtime was initialized.
 */
declare function getTimer():number;

/**
 * @internal
 * The standard output stream is the default destination of output for applications. In most systems, it is usually
 * directed by default to the text console (generally, on the screen).
 */
declare let stdout:OutputStream;

/**
 * @internal
 * The standard error stream is the default destination for error messages and other diagnostic warnings. Like stdout,
 * it is usually also directed by default to the text console (generally, on the screen).
 */
declare let stderr:OutputStream;

namespace cyder {

    function formatString(format):string {
        let objects = new Array(arguments.length);
        for (let index = 0; index < arguments.length; index++) {
            objects[index] = arguments[index];
        }
        return objects.join(' ');
    }

    /**
     * @internal
     */
    export class Console {

        public constructor(stdout:OutputStream, stderr:OutputStream) {
            this.stdout = stdout;
            this.stderr = stderr;
        }

        /**
         * The standard output stream.
         */
        private stdout:OutputStream;

        /**
         * The standard error stream.
         */
        private stderr:OutputStream;

        /**
         * Writes an message to the console.
         * @param message the message written to the console
         * @param optionalParams the extra messages written to the console
         */
        public log(message?:any, ...optionalParams:any[]):void {
            let text = formatString.apply(this, arguments);
            this.stdout.write(text + "\n");
        }

        /**
         * Writes an error message to the console if the assertion is false. If the assertion is true, nothing will happen.
         * @param assertion Any boolean expression. If the assertion is false, the message will get written to the console.
         * @param message the message written to the console
         * @param optionalParams the extra messages written to the console
         */
        public assert(assertion?:boolean, message?:string, ...optionalParams:any[]):void {
            if (!assertion) {
                this.stderr.write(message + "\n");
            }
        }

        /**
         * Writes a warning message to the console.
         * @param message the message written to the console
         * @param optionalParams the extra messages written to the console
         */
        public warn(message?:any, ...optionalParams:any[]):void {
            let text = formatString.apply(this, arguments);
            this.stderr.write(text + "\n");
        }

        /**
         * Writes an error message to the console.
         * @param message the message written to the console
         * @param optionalParams the extra messages written to the console
         */
        public error(message?:any, ...optionalParams:any[]):void {
            let text = formatString.apply(this, arguments);
            this.stderr.write(text + "\n");
        }

    }

}

this.console = new cyder.Console(stdout, stderr);