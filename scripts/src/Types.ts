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

interface Canvas {
    width:number;
    height:number;
}

declare let Canvas:{
    prototype:Canvas;
    new():Canvas;
}

/**
 * The EventEmitter interface defines methods for adding or removing event listeners, checks whether specific types
 * of event listeners are registered, and dispatches events.
 */
interface EventEmitter {
    /**
     * Registers an event listener object with an EventEmitter object so that the listener receives notification of an
     * event. After the listener is registered, subsequent calls to addEventListener() with a different value for either
     * type or thisObject result in the creation of a separate listener registration. <br/>
     * When you no longer need an event listener, remove it by calling removeEventListener(); otherwise, memory problems
     * might result. Objects with registered event listeners are not automatically removed from memory because the garbage
     * collector does not remove objects that still have references. If the event listener is being registered on a node
     * while an event is also being processed on this node, the event listener is not triggered during the current phase
     * but may be triggered during next phase. If an event listener is removed from a node while an event is being
     * processed on the node, it is still triggered by the current actions. After it is removed, the event listener is
     * never invoked again (unless it is registered again for future processing).
     * @param type The type of event.
     * @param listener The listener function that processes the event. This function must accept an event object as
     * its only parameter and must return nothing, as this example shows: function(event:Event):void  The function can
     * have any name.
     * @param thisObject The object that the "this" keyword of the listener function points to.
     * @see #once()
     * @see #removeEventListener()
     */
    addEventListener(type:string, listener:Function, thisObject?:any):void;

    /**
     * Registers an event listener object with an EventEmitter object so that the listener receives notification of an
     * event. Different from the addEventListener() method, the listener receives notification only once, and then will
     * be removed automatically by the removeEventListener method.
     * @param type The type of event.
     * @param listener The listener function that processes the event. This function must accept an event object as
     * its only parameter and must return nothing, as this example shows: function(event:Event):void  The function can
     * have any name.
     * @param thisObject The object that the "this" keyword of the listener function points to.
     * @see #addEventListener()
     * @see #removeEventListener()
     */
    once(type:string, listener:Function, thisObject?:any):void;

    /**
     * Removes a listener from the EventEmitter object. If there is no matching listener registered with the EventEmitter
     * object, a call to this method has no effect.
     * @param type The type of event.
     * @param listener The listener function to be removed.
     * @param thisObject The object that the "this" keyword of the listener function points to.
     */
    removeEventListener(type:string, listener:Function, thisObject?:any):void;

    /**
     * Checks whether the EventEmitter object has any listeners registered for a specific type of event. This allows
     * you to determine where an EventEmitter object has altered handling of an event type in the event flow hierarchy.
     * @param type The type of event.
     * @returns A value of true if a listener of the specified type is registered; false otherwise.
     */
    hasEventListener(type:string):boolean;

    /**
     * Dispatches an event to all objects that have registered listeners for the type of this event. The event target is
     * the EventEmitter object upon which dispatchEvent() is called.
     * @param event The event object dispatched into the event flow.
     * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
     */
    dispatchEvent(event:Event):boolean;

    /**
     * Dispatches an event with the given parameters to all objects that have registered listeners for the given type.
     * The method uses an internal pool of event objects to avoid allocations.
     * @param type The type of the event.
     * @param cancelable Determines whether the Event object can be canceled. The default values is false.
     * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
     */
    dispatchEventWith(type:string, cancelable?:boolean):boolean
}

/**
 * The callback function for requestAnimation method.
 */
interface FrameRequestCallback {
    (time:number):void;
}

/**
 * The NativeApplication interface provides application information, application-wide functions, and dispatches
 * application-level events. The NativeApplication object is a singleton object, created automatically at startup.
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

interface NativeWindow extends EventEmitter {
    canvas:Canvas;
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