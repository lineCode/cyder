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
 * or rendering. And it also can be drew to another canvas object.
 */
interface Canvas {
    /**
     * Gets or sets the height of a canvas object.
     */
    height:number;
    /**
     * Gets or sets the width of a canvas object.
     */
    width:number;

    /**
     * Returns a drawing context on the canvas, or null if the context identifier is not supported.
     * @param contextType A string containing the context identifier defining the drawing context associated to the canvas.
     * @param contextAttributes You can use several context attributes when creating your rendering context.
     */
    getContext(contextType:string, contextAttributes?:{}):any;
    getContext(contextType:"2d", contextAttributes?:Canvas2DContextAttributes):CanvasRenderingContext2D;
}

declare let Canvas:{
    prototype:Canvas;
    new(width:number, height:number):Canvas;
}

/**
 * 2d context attributes.
 */
interface Canvas2DContextAttributes {
    /**
     * A boolean value that indicates whether the canvas contains an alpha channel. If set to false, the runtime now knows
     * that the backdrop is always opaque, which can speed up drawing of transparent content and images then.
     */
    alpha?:boolean;
    /**
     * A boolean value that indicates whether a lot of read-back operations are planned. This will force the use of a
     * software (instead of hardware accelerated) 2D canvas and can save memory when calling getImageData() frequently.
     */
    willReadFrequently?:boolean;
}

/**
 * The CanvasRenderingContext2D interface is used for drawing rectangles, text, images and other objects onto the canvas
 * object. It provides the 2D rendering context for the drawing surface of a canvas object.<br/>
 * To get an object of this interface, call getContext() on a canvas object, supplying "2d" as the contextType argument.
 */
interface CanvasRenderingContext2D {
    /**
     * A read-only back-reference to the associated canvas object.
     */
    readonly canvas:HTMLCanvasElement;
}


/**
 * The EventEmitter interface defines methods for adding or removing event listeners, checks whether specific types
 * of event listeners are registered, and emits events.
 */
interface EventEmitter {
    /**
     * Registers an event listener object with an EventEmitter object so that the listener receives notification of an
     * event. After the listener is registered, subsequent calls to on() with a different value for either type or
     * thisObject result in the creation of a separate listener registration. <br/>
     * When you no longer need an event listener, remove it by calling removeListener(); otherwise, memory problems
     * might result. Objects with registered event listeners are not automatically removed from memory because the garbage
     * collector does not remove objects that still have references. If the event listener is being registered on a target
     * while an event is also being processed on this target, the event listener is not triggered during the current phase
     * but may be triggered during the next phase. If an event listener is removed from a target while an event is being
     * processed on the target, it is still triggered by the current actions. After it is removed, the event listener is
     * never invoked again (unless it is registered again for future processing).
     * @param type The type of event.
     * @param listener The listener function that processes the event.
     * @param thisArg The value of this provided for the call to a listener function.
     */
    on(type:string, listener:EventListener, thisArg:any):void;

    /**
     * Registers an event listener object with an EventEmitter object so that the listener receives notification of an
     * event. Different from the on() method, the listener receives notification only once, and then will be removed
     * automatically by the removeListener method.
     * @param type The type of event.
     * @param listener The listener function that processes the event.
     * @param thisArg The value of this provided for the call to a listener function.
     */
    once(type:string, listener:EventListener, thisArg:any):void;

    /**
     * Removes a listener from the EventEmitter object. If there is no matching listener registered with the EventEmitter
     * object, a call to this method has no effect.
     * @param type The type of event.
     * @param listener The listener function to be removed.
     * @param thisArg The value of this provided for the call to a listener function.
     */
    removeListener(type:string, listener:EventListener, thisArg:any):void;

    /**
     * Checks whether the EventEmitter object has any listeners registered for a specific type of event. This allows
     * you to determine where an EventEmitter object has altered handling of an event type in the event flow hierarchy.
     * @param type The type of event.
     * @returns A value of true if a listener of the specified type is registered; false otherwise.
     */
    hasListener(type:string):boolean;

    /**
     * Emits an event to all objects that have registered listeners for the type of this event. The event target is the
     * EventEmitter object upon which emit() is called.
     * @param event The event object emitted into the event flow.
     * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
     */
    emit(event:Event):boolean;

    /**
     * Emits an event with the given parameters to all objects that have registered listeners for the given type.
     * The method uses an internal pool of event objects to avoid allocations.
     * @param type The type of the event.
     * @param cancelable Determines whether the Event object can be canceled. The default values is false.
     * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
     */
    emitWith(type:string, cancelable?:boolean):boolean
}

/**
 * The callback function for event listener.
 */
interface EventListener {
    (event:Event):void;
}

/**
 * The callback function for requestAnimation method.
 */
interface FrameRequestCallback {
    (time:number):void;
}

/**
 * The NativeApplication interface provides application information, application-wide functions, and emits application-level events. The NativeApplication object is a singleton object, created automatically at startup.
 * Get the NativeApplication instance with the global 'nativeApplication' property.
 */
interface NativeApplication extends EventEmitter {
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

    /**
     * The active application window.<br/>
     * If the active desktop window does not belong to this application, or there is no active window, activeWindow is null.
     */
    activeWindow:NativeWindow;

    /**
     * An array containing all the open native windows of this application.
     */
    openedWindows:NativeWindow[];
}

/**
 * The NativeWindow class provides an interface for creating and controlling native desktop windows.
 * NativeWindow objects will not be garbage collected after the window constructor has been called and before close()
 * has been called. It is the responsibility of the application to close its own windows.
 */
interface NativeWindow extends EventEmitter {
    /**
     * The Canvas object for this window. Anything drew to it will show on the NativeWindow.
     */
    canvas:Canvas;
    /**
     * Activates this window.<br/>
     * Activating a window will:<br/>
     * 1. Make the window visible<br/>
     * 2. Bring the window to the front<br/>
     * 3. Give the window keyboard and mouse focus<br/>
     */
    activate():void;
}

declare let NativeWindow:{
    prototype:NativeWindow;
    new():NativeWindow;
}


/**
 * The Performance interface represents timing-related performance information for the application.
 */
interface Performance {

    /**
     * Returns the high resolution time since the application was initialized, measured in milliseconds, accurate to five
     * thousandths of a millisecond (5 microseconds).
     */
    now():number;
}

/**
 * Writable streams are an abstraction for a destination to which data is written.
 */
interface WritableStream {
    /**
     * Write a string message to WritableStream.
     */
    write(message:string):void;
}