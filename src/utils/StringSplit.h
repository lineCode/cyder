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

#ifndef CYDER_STRINGSPLIT_H
#define CYDER_STRINGSPLIT_H

#include <string>
#include <vector>

namespace cyder {
    inline std::vector<std::string> stringSplit(const std::string& text, const std::string& separator) {
        std::vector<std::string> result;
        std::string::size_type pos1, pos2;
        pos2 = text.find(separator);
        pos1 = 0;
        while (std::string::npos != pos2) {
            result.push_back(text.substr(pos1, pos2 - pos1));

            pos1 = pos2 + separator.size();
            pos2 = text.find(separator, pos1);
        }
        if (pos1 != text.length()) {
            result.push_back(text.substr(pos1));
        }
        return result;
    }

}

#endif //CYDER_STRINGSPLIT_H
