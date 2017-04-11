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

#ifndef CYDER_WINDOWINITOPTIONS_H
#define CYDER_WINDOWINITOPTIONS_H

#include "WindowSystemChrome.h"
#include "WindowType.h"

class Window;

namespace cyder {

    /**
     * The WindowInitOptions class defines the initialization options used to construct a new Window instance.
     * The properties defined in the initialization options cannot be changed after a window is created.
     */
    class WindowInitOptions final {
    public:
        WindowInitOptions() : maximizable(true),
                              minimizable(true),
                              resizable(true),
                              transparent(false),
                              owner(nullptr),
                              systemChrome(WindowSystemChrome::STANDARD),
                              type(WindowType::NORMAL) {}

        bool maximizable;
        bool minimizable;
        bool resizable;
        bool transparent;

        /**
         * Specifies the Window object that should own any windows created with this WindowInitOptions.
         * When a window has an owner, that window is always displayed in front of its owner, is minimized and hidden along
         * with its owner, and closes when its owner closes.
         */
        Window* owner;
        int systemChrome;
        int type;

    };

} // namespace cyder

#endif //CYDER_WINDOWINITOPTIONS_H
