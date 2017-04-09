//////////////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017-present, Cyder.org
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


#ifndef CYDER_WEAKHELPER_H
#define CYDER_WEAKHELPER_H

#include <functional>
#include <v8.h>

namespace cyder {
    /**
     * This helper class wraps a v8::Object and a callback function. It will trigger the callback function when v8 garbage
     * collects the v8::Object, and then delete itself too. <br/>
     * Notice: The callback function will never trigger if you delete the instance of WeakHandle mamually.
     */
    class WeakHandle {
    public:
        static WeakHandle*
        New(v8::Isolate* isolate, v8::Local<v8::Object> handle, std::function<void()> callback = nullptr) {
            return new WeakHandle(isolate, handle, callback);
        }

        ~WeakHandle() {
            if (persistent.IsEmpty()) {
                return;
            }
            persistent.ClearWeak();
            persistent.Reset();
        }

        void setCallback(std::function<void()> callback) {
            this->callback = callback;
        }

        v8::Local<v8::Object> getLocal() {
            return *reinterpret_cast<v8::Local<v8::Object>*>(const_cast<v8::Persistent<v8::Object>*>(&persistent));
        }

    private:

        WeakHandle(v8::Isolate* isolate, v8::Local<v8::Object> handle, std::function<void()> callback) :
                persistent(isolate, handle),
                callback(callback) {
            persistent.SetWeak(this, Callback, v8::WeakCallbackType::kParameter);
            persistent.MarkIndependent();
        }

        v8::Persistent<v8::Object> persistent;
        std::function<void()> callback;

        static void Callback(const v8::WeakCallbackInfo<WeakHandle>& data) {
            v8::Isolate* isolate = data.GetIsolate();
            v8::HandleScope handle_scope(isolate);
            WeakHandle* handle = data.GetParameter();
            handle->persistent.Reset();
            if (handle->callback) {
                handle->callback();
            }
            delete handle;
        }
    };


}  // namespace cyder

#endif //CYDER_WEAKHELPER_H
