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

#include "EventEmitter.h"
#include "utils/ObjectPool.h"

namespace cyder {
    EventEmitter::~EventEmitter() {
        delete eventsMap;
    }

    void EventEmitter::doAddListener(const std::string& type, const EventListenerPtr listener, bool emitOnce) {
        EventListPtr list;
        auto item = eventsMap->find(type);
        if (item == eventsMap->end()) {
            EventListPtr newList(new EventList());
            eventsMap->insert(EventMapPair(type, newList));
            list = newList;
        } else {
            list = item->second;
            if (notifyLevel != 0) {
                EventListPtr tempList(new EventList());
                (*tempList) = (*list); // clone the list in case that we are in the middle of dispatching events.
                eventsMap->erase(item);
                eventsMap->insert(EventMapPair(type, tempList));
                list = tempList;
            }
        }
        insertEventNode(list, type, listener, emitOnce);
    }

    bool EventEmitter::insertEventNode(EventListPtr list, const std::string& type,
                                       const EventListenerPtr listener, bool emitOnce) {
        for (const auto& item : *list) {
            if (item->listener == listener && item->target == this) {
                return false;
            }
        }

        list->push_back(new EventNode(type, listener, this, emitOnce));
        return true;
    }

    void EventEmitter::removeListener(const std::string& type, const EventListenerPtr listener) {
        auto item = eventsMap->find(type);
        if (item == eventsMap->end()) {
            return;
        }
        EventListPtr list = item->second;
        if (notifyLevel != 0) {
            EventListPtr tempList(new EventList());
            (*tempList) = (*list); // clone the list in case that we are in the middle of dispatching events.
            eventsMap->erase(item);
            eventsMap->insert(EventMapPair(type, tempList));
            list = tempList;
        }
        removeEventNode(list, listener);
        if (list->size() == 0) {
            eventsMap->erase(type);
        }
    }

    bool EventEmitter::removeEventNode(EventListPtr list, const EventListenerPtr listener) {
        int index = 0;
        for (const auto& item : *list) {
            if (item->listener == listener && item->target == this) {
                list->erase(list->begin() + index);
                delete item;
                return true;
            }
            index++;
        }
        return false;
    }

    bool EventEmitter::hasListener(const std::string& type) {
        return eventsMap->count(type) > 0;
    }

    bool EventEmitter::emit(Event* event) {
        event->_target = this;
        auto item = eventsMap->find(event->_type);
        if (item == eventsMap->end()) {
            return true;
        }
        EventListPtr list = item->second;
        if (list->size() == 0) {
            return true;
        }
        EventList onceList;
        notifyLevel++;
        for (const auto& node : *list) {
            node->listener->handleEvent(event);
            if (node->emitOnce) {
                onceList.push_back(node);
            }
        }
        notifyLevel--;
        for (const auto& eventNode : onceList) {
            eventNode->target->removeListener(eventNode->type, eventNode->listener);
        }
        return event->_isDefaultPrevented;
    }

    bool EventEmitter::emitWith(const std::string& type, bool cancelable) {
        if (hasListener(type)) {
            static ObjectPool<Event> eventPool;
            Event* event;
            if (eventPool.size() > 0) {
                event = eventPool.pop();
                event->reset(type, cancelable);

            } else {
                event = new Event(type, cancelable);
            }
            auto result = emit(event);
            event->_target = nullptr;
            return result;
        }
        return true;
    }
}