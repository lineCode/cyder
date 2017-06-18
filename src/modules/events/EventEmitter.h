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

#ifndef CYDER_EVENTEMITTER_H
#define CYDER_EVENTEMITTER_H

#include <string>
#include <unordered_map>
#include "binding/ScriptWrappable.h"
#include "EventListener.h"

namespace cyder {

    /**
     * The EventEmitter class defines methods for adding or removing event listeners, checks whether specific types of
     * event listeners are registered, and emits events.
     */
    class EventEmitter : public ScriptWrappable {
    DEFINE_WRAPPERTYPEINFO();

        /**
         * Data with event information.
         */
        struct EventNode {
        public:
            EventNode(const std::string& type, const EventListenerPtr listener,
                      EventEmitter* target, bool emitOnce) :
                    type(type), listener(listener), target(target), emitOnce(emitOnce) {
            }

            const std::string type;
            const EventListenerPtr listener;
            EventEmitter* target;
            const bool emitOnce;
        };

        typedef std::vector<EventNode*> EventList;
        typedef std::shared_ptr<EventList> EventListPtr;
        typedef std::unordered_map<std::string, EventListPtr> EventMap;

    public:
        /**
         * create an instance of the EventEmitter class.
         */
        EventEmitter() {}

        virtual ~EventEmitter();

        /**
         * Registers an event listener object with an EventEmitter object so that the listener receives notification of an
         * event. After the listener is registered, subsequent calls to on() with a different value for either type or
         * thisArg result in the creation of a separate listener registration. <br/>
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
        void on(const std::string& type, const EventListenerPtr listener) {
            doAddListener(type, listener, false);
        }

        /**
         * Registers an event listener object with an EventEmitter object so that the listener receives notification of an
         * event. Different from the on() method, the listener receives notification only once, and then will be removed
         * automatically by the removeListener method.
         * @param type The type of event.
         * @param listener The listener function that processes the event.
         * @param thisArg The value of this provided for the call to a listener function.
         */
        void once(const std::string& type, const EventListenerPtr listener) {
            doAddListener(type, listener, true);
        }

        /**
         * Removes a listener from the EventEmitter object. If there is no matching listener registered with the EventEmitter
         * object, a call to this method has no effect.
         * @param type The type of event.
         * @param listener The listener function to be removed.
         * @param thisArg The value of this provided for the call to a listener function.
         */
        void removeListener(const std::string& type, const EventListenerPtr listener);

        /**
         * Checks whether the EventEmitter object has any listeners registered for a specific type of event. This allows
         * you to determine where an EventEmitter object has altered handling of an event type in the event flow hierarchy.
         * @param type The type of event.
         * @returns A value of true if a listener of the specified type is registered; false otherwise.
         */
        bool hasListener(const std::string& type);

        /**
         * Emits an event to all objects that have registered listeners for the type of this event. The event target is the
         * EventEmitter object upon which emit() is called.
         * @param event The event object emitted into the event flow.
         * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
         */
        bool emit(Event* event);

        /**
         * Emits an event with the given parameters to all objects that have registered listeners for the given type.
         * The method uses an internal pool of event objects to avoid allocations.
         * @param type The type of the event.
         * @param cancelable Determines whether the Event object can be canceled. The default values is false.
         * @returns A value of true unless preventDefault() is called on the event, in which case it returns false.
         */
        bool emitWith(const std::string& type, bool cancelable = false);

    private:

        EventMap* eventsMap = new EventMap();
        int notifyLevel = 0;

        void doAddListener(const std::string& type, const EventListenerPtr listener, bool emitOnce);
        bool insertEventNode(EventListPtr list, const std::string& type, const EventListenerPtr listener,
                             bool emitOnce = false);
        bool removeEventNode(EventListPtr list, const EventListenerPtr listener);

    };

}

#endif //CYDER_EVENTEMITTER_H
