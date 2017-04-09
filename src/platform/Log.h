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

#ifndef CYDER_LOG_H
#define CYDER_LOG_H

#include <stdlib.h>

#if !defined(CYDER_DEBUG) && !defined(CYDER_RELEASE)
#ifdef NO_DEBUG
#define CYDER_RELEASE
#else
#define CYDER_DEBUG
#endif
#endif


namespace cyder {

    void printLog(const char format[], ...);
    void printError(const char format[], ...);

#define ABORT(msg) \
    do { \
       printError("%s:%d: fatal error: \"%s\"\n", __FILE__, __LINE__, #msg); \
       ::abort(); \
    } while (false)


#ifdef CYDER_DEBUG

#define LOG(...)            printLog(__VA_ARGS__)
#define ASSERT(assertion)   if(!(assertion)) { ABORT(#assertion); }

#else

#define LOG(...)
#define ASSERT(assertion)

#endif
} // namespace cyder

#endif //CYDER_LOG_H
