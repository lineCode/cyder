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


namespace cyder {

    /**
     * The Application interface provides application information, application-wide functions, and dispatches application-level
     * events. The Application object is a singleton object, created automatically at startup. Get the Application instance
     * with the global 'application' property.
     */
    export interface Application {
        /**
         * The standard output stream is the default destination of output for applications. In most systems, it is usually
         * directed by default to the text console (generally, on the screen).
         */
        standardOutput:WritableStream;

        /**
         * The standard error stream is the default destination for error messages and other diagnostic warnings. Like stdout,
         * it is usually also directed by default to the text console (generally, on the screen).
         */
        standardError:WritableStream;
    }

    /**
     * The Performance interface represents timing-related performance information for the application.
     */
    export interface Performance {

        /**
         * Returns the high resolution time since the application was initialized, measured in milliseconds, accurate to five
         * thousandths of a millisecond (5 microseconds).
         */
        now():number;
    }

    /**
     * Writable streams are an abstraction for a destination to which data is written.
     */
    export interface WritableStream {
        /**
         * Write a string message to WritableStream.
         */
        write(message:string):void;
    }

    /**
     * Global variables.
     */
    export interface Global {
        application:cyder.Application;
        performance:Performance;
        console:Console;
    }
}


declare let global:cyder.Global;
declare let application:cyder.Application;
declare let performance:cyder.Performance;
declare let console:Console;

global = this;