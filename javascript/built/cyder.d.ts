declare namespace cyder {
    /**
     * The Application interface provides application information, application-wide functions, and dispatches application-level
     * events. The Application object is a singleton object, created automatically at startup. Get the Application instance
     * with the global 'application' property.
     */
    interface Application {
        /**
         * The standard output stream is the default destination of output for applications. In most systems, it is usually
         * directed by default to the text console (generally, on the screen).
         */
        standardOutput: WritableStream;
        /**
         * The standard error stream is the default destination for error messages and other diagnostic warnings. Like stdout,
         * it is usually also directed by default to the text console (generally, on the screen).
         */
        standardError: WritableStream;
    }
    /**
     * The Performance interface represents timing-related performance information for the application.
     */
    interface Performance {
        /**
         * Returns the high resolution time since the application was initialized, measured in milliseconds, accurate to five
         * thousandths of a millisecond (5 microseconds).
         */
        now(): number;
    }
    /**
     * Writable streams are an abstraction for a destination to which data is written.
     */
    interface WritableStream {
        /**
         * Write a string message to WritableStream.
         */
        write(message: string): void;
    }
    /**
     * Global variables.
     */
    interface Global {
        application: cyder.Application;
        performance: Performance;
        console: Console;
    }
}
declare let global: cyder.Global;
declare let application: cyder.Application;
declare let performance: cyder.Performance;
declare let console: Console;
/**
 * The Console class can be used to create a simple logger with configurable output streams.
 */
declare class Console {
    constructor(stdout: cyder.WritableStream, stderr: cyder.WritableStream);
    private static formatString(format);
    /**
     * Writes an message to the console.
     * @param message the message written to the console
     * @param optionalParams the extra messages written to the console
     */
    log(message?: any, ...optionalParams: any[]): void;
    /**
     * Writes an error message to the console if the assertion is false. If the assertion is true, nothing will happen.
     * @param assertion Any boolean expression. If the assertion is false, the message will get written to the console.
     * @param message the message written to the console
     * @param optionalParams the extra messages written to the console
     */
    assert(assertion?: boolean, message?: string, ...optionalParams: any[]): void;
    /**
     * Writes a warning message to the console.
     * @param message the message written to the console
     * @param optionalParams the extra messages written to the console
     */
    warn(message?: any, ...optionalParams: any[]): void;
    /**
     * Writes an error message to the console.
     * @param message the message written to the console
     * @param optionalParams the extra messages written to the console
     */
    error(message?: any, ...optionalParams: any[]): void;
}
