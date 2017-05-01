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
 * The Event class is used as the base class for the creation of Event objects, which are passed as parameters to event
 * listeners when an event occurs. The properties of the Event class carry basic information about an event, such as
 * the event's type or whether the event's default behavior can be canceled. For many events, such as the events represented
 * by the Event class constants, this basic information is sufficient. Other events, however, may require more detailed
 * information. The methods of the Event class can be used in event listener functions to affect the behavior of the event
 * object. Some events have an associated default behavior. Your event listener can cancel this behavior by calling the
 * preventDefault() method.
 */
class Event {

    /**
     * Emitted when the application become active.
     */
    public static readonly ACTIVATE:string = "activate";
    /**
     * Emitted when the application become inactive.
     */
    public static readonly DEACTIVATE:string = "deactivate";

    /**
     * Emitted when the size of NativeWindow is changed.
     */
    public static readonly RESIZE:string = "resize";

    /**
     * Emitted when the size of NativeWindow is going to change. you can cancel this by calling the preventDefault()
     * method.
     */
    public static readonly RESIZING:string = "resizing";

    /**
     * Emitted when the value or selection of a property is changed.
     */
    public static readonly CHANGE:string = "change";

    /**
     * Emitted when the value or selection of a property is going to change. you can cancel this by calling the
     * preventDefault() method.
     */
    public static readonly CHANGING:string = "changing";

    /**
     * Emitted when the net request is complete.
     */
    public static readonly COMPLETE:string = "complete";


    /**
     * Creates an Event object to pass as a parameter to event listeners.
     * @param type  The type of the event, accessible as Event.type.
     * @param cancelable Determines whether the Event object can be canceled. The default values is false.
     */
    public constructor(type:string, cancelable?:boolean) {
        this.$initEvent(type, !!cancelable);
    }

    private _type:string;

    /**
     * The type of event. The type is case-sensitive.
     */
    public get type():string {
        return this._type;
    }

    private _cancelable:boolean;

    /**
     * Indicates whether the behavior associated with the event can be prevented. If the behavior can be
     * canceled, this value is true; otherwise it is false.
     * @see #preventDefault()
     */
    public get cancelable():boolean {
        return this._cancelable;
    }

    /**
     * The event target. It is the EventEmitter object which calls the emit() method.
     */
    public target:any;

    private _isDefaultPrevented:boolean;

    /**
     * Checks whether the preventDefault() method has been called on the event. If the preventDefault() method has been
     * called, returns true; otherwise, returns false.
     * @returns If preventDefault() has been called, returns true; otherwise, returns false.
     * @see #preventDefault()
     */
    public isDefaultPrevented():boolean {
        return this._isDefaultPrevented;
    }

    /**
     * Cancels an event's default behavior if that behavior can be canceled.Many events have associated behaviors that
     * are carried out by default. For example, if a user types a character into a text input, the default behavior
     * is that the character is displayed in the text input. Because the TextEvent.TEXT_INPUT event's default behavior
     * can be canceled, you can use the preventDefault() method to prevent the character from appearing.
     * You can use the Event.cancelable property to check whether you can prevent the default behavior associated with
     * a particular event. If the value of Event.cancelable is true, then preventDefault() can be used to cancel the event;
     * otherwise, preventDefault() has no effect.
     * @see #cancelable
     * @see #isDefaultPrevented
     */
    public preventDefault():void {
        if (this.cancelable)
            this._isDefaultPrevented = true;
    }

    protected initEvent(type:string, cancelable?:boolean) {
        this.$initEvent(type, !!cancelable);
    }

    /**
     * @internal
     */
    $initEvent(type:string, cancelable?:boolean) {
        this._type = type;
        this._cancelable = cancelable;
        this._isDefaultPrevented = false;
        this.target = null;
    }
}
