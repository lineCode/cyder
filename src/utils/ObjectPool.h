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

#ifndef CYDER_OBJECTPOOL_H
#define CYDER_OBJECTPOOL_H

#include <vector>

namespace cyder {

    template<class T>
    class ObjectPool {
    public:
        explicit ObjectPool(int maxSize = 64) : maxSize(maxSize) {
        }

        ~ObjectPool() {
            for (const auto& item:objectList) {
                delete item;
            }
        }

        void push(T* object) {
            if (objectList.size() >= maxSize) {
                return;
            }
            objectList.push_back(object);
        }

        T* pop() {
            if (objectList.size() > 0) {
                auto result = objectList[objectList.size() - 1];
                objectList.pop_back();
                return result;
            }
            return nullptr;
        }

        int size() const {
            return static_cast<int>(objectList.size());
        }

    private:
        int maxSize;
        std::vector<T*> objectList;
    };

}

#endif //CYDER_OBJECTPOOL_H
