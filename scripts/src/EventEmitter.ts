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
     * @private
     */
    interface EventBin {
        type:string;
        listener:Function;
        thisObject:any;
        target:EventEmitter;
        dispatchOnce:boolean;
    }

    /**
     * @private
     */
    let ONCE_EVENT_LIST:EventBin[] = [];
    /**
     * @private
     */
    let eventPool:Event[] = [];

    /**
     * @internal
     * The EventEmitter class defines methods for adding or removing event listeners, checks whether specific types
     * of event listeners are registered, and dispatches events.
     */
    export class EventEmitter {

        /**
         * create an instance of the EventDispatcher class.
         */
        public constructor() {
            this.eventsMap = createMap<EventBin[]>();
        }

        /**
         * @private
         */
        private eventsMap:Map<EventBin[]>;

        /**
         * @private
         */
        private notifyLevel:number = 0;

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
        public addEventListener(type:string, listener:Function, thisObject?:any):void {
            this.doAddListener(type, listener, thisObject);
        }

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
        public once(type:string, listener:Function, thisObject?:any):void {
            this.doAddListener(type, listener, thisObject, true);
        }

        /**
         * @private
         */
        private doAddListener(type:string, listener:Function, thisObject:any, dispatchOnce?:boolean):void {
            let eventMap = this.eventsMap;
            let list:EventBin[] = eventMap[type];
            if (!list) {
                list = eventMap[type] = [];
            }
            else if (this.notifyLevel !== 0) {
                // If the notifyLevel is not 0, that indicates we are traversing the event list, so we need to concat it first.
                eventMap[type] = list = list.concat();
            }

            this.insertEventBin(list, type, listener, thisObject, dispatchOnce);
        }

        /**
         * @private
         */
        private insertEventBin(list:EventBin[], type:string, listener:Function, thisObject:any, dispatchOnce?:boolean):boolean {
            for (let bin of list) {
                if (bin.listener == listener && bin.thisObject == thisObject && bin.target == this) {
                    return false;
                }
            }
            list.push({
                type: type, listener: listener, thisObject: thisObject,
                target: this, dispatchOnce: dispatchOnce
            });
            return true;
        }

        /**
         * Removes a listener from the EventEmitter object. If there is no matching listener registered with the EventEmitter
         * object, a call to this method has no effect.
         * @param type The type of event.
         * @param listener The listener function to be removed.
         * @param thisObject The object that the "this" keyword of the listener function points to.
         */
        public removeEventListener(type:string, listener:Function, thisObject?:any):void {

            let eventMap = this.eventsMap;
            let list:EventBin[] = eventMap[type];
            if (!list) {
                return;
            }
            if (this.notifyLevel !== 0) {
                // If the notifyLevel is not 0, that indicates we are traversing the event list, so we need to concat it first.
                eventMap[type] = list = list.concat();
            }

            this.removeEventBin(list, listener, thisObject);

            if (list.length == 0) {
                eventMap[type] = null;
            }
        }

        /**
         * @private
         */
        private removeEventBin(list:EventBin[], listener:Function, thisObject:any):boolean {
            let length = list.length;
            for (let i = 0; i < length; i++) {
                let bin = list[i];
                if (bin.listener == listener && bin.thisObject == thisObject && bin.target == this) {
                    list.splice(i, 1);
                    return true;
                }
            }
            return false;
        }

        /**
         * Checks whether the EventEmitter object has any listeners registered for a specific type of event. This allows
         * you to determine where an EventEmitter object has altered handling of an event type in the event flow hierarchy.
         * @param type The type of event.
         * @returns A value of true if a listener of the specified type is registered; false otherwise.
         */
        public hasEventListener(type:string):boolean {
            return !!(this.eventsMap[type]);
        }

        /**
         * Dispatches an event to all objects that have registered listeners for the type of this event. The event target is
         * the EventEmitter object upon which dispatchEvent() is called.
         * @param event The event object dispatched into the event flow.
         * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
         */
        public dispatchEvent(event:Event):boolean {
            event.target = this;
            let list = this.eventsMap[event.type];
            if (!list) {
                return true;
            }
            let length = list.length;
            if (length == 0) {
                return true;
            }
            let onceList = ONCE_EVENT_LIST;
            this.notifyLevel++;
            for (let eventBin of list) {
                eventBin.listener.call(eventBin.thisObject, event);
                if (eventBin.dispatchOnce) {
                    onceList.push(eventBin);
                }
            }
            this.notifyLevel--;
            while (onceList.length) {
                let eventBin = onceList.pop();
                eventBin.target.removeEventListener(eventBin.type, eventBin.listener, eventBin.thisObject);
            }
            return !event.isDefaultPrevented();
        }


        /**
         * Dispatches an event with the given parameters to all objects that have registered listeners for the given type.
         * The method uses an internal pool of event objects to avoid allocations.
         * @param type The type of the event.
         * @param cancelable Determines whether the Event object can be canceled. The default values is false.
         * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
         */
        public dispatchEventWith(type:string, cancelable?:boolean):boolean {
            if (this.eventsMap[type]) {
                let event:Event;
                if (eventPool.length) {
                    event = eventPool.pop();
                    event.$initEvent(type, cancelable);
                } else {
                    event = new Event(type, cancelable);
                }

                let result = this.dispatchEvent(event);
                event.target = null;
                eventPool.push(event);
                return result;
            }
            return true;
        }
    }

    /**
     * @internal
     */
    export function implementEventEmitter(Class:Function):void {
        let targetProto = Class.prototype;
        let emitterProto = EventEmitter.prototype;
        let keys = Object.keys(emitterProto);
        for (let key of keys) {
            targetProto[key] = emitterProto[key];
        }
    }
}