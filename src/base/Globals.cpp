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


#include "Globals.h"
#include <algorithm>

namespace cyder {

    std::string Globals::applicationDirectory = "";

    void Globals::initialize(const std::string& execPath) {
        if (execPath.empty()) {
            return;
        }
        std::string path = execPath;
        std::replace(path.begin(), path.end(), '\\', '/');
        auto index = path.rfind("/");
        if (index != std::string::npos) {
            applicationDirectory = path.substr(0, index + 1);
        }
        index = path.rfind(".app/Contents/");
        if(index != std::string::npos) {
            applicationDirectory = path.substr(0,index+14)+"Resources/";
        }
    }

    std::string Globals::resolvePath(const std::string& relativePath) {
        if (relativePath.empty()) {
            return relativePath;
        }
        std::string path = relativePath;
        std::replace(path.begin(), path.end(), '\\', '/');
        if (path[0] == '/' || path.find(":") != std::string::npos) {
            return path;
        }
        return applicationDirectory + path;
    }

}  // namespace cyder