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

#ifndef CYDER_WINDOWSYSTEMCHROME_H
#define CYDER_WINDOWSYSTEMCHROME_H

#include <string>

namespace cyder {
    /**
     * The WindowSystemChrome class defines constants for the systemChrome property of the WindowInitOptions object
     * used to create a native window. System chrome refers to the operating system-specific elements of a window such as a
     * title bar, minimize, maximize, and close buttons.
     * Note: The type of system chrome used is specified when a window is created and cannot be changed.
     */
    class WindowSystemChrome {
    public:
        /**
         * The standard chrome for the host operating system.
         */
        static const int STANDARD = 1;

        /**
         * No system chrome.
         */
        static const int NONE = 2;
    };

} // namespace cyder

#endif //CYDER_WINDOWSYSTEMCHROME_H
