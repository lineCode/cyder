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


#include "Base64.h"

namespace cyder {

    static const char base64EncMap[64] = {
            0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
            0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
            0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
            0x59, 0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
            0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
            0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
            0x77, 0x78, 0x79, 0x7A, 0x30, 0x31, 0x32, 0x33,
            0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x2B, 0x2F
    };

    static const char base64DecMap[128] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x3F,
            0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
            0x3C, 0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
            0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
            0x17, 0x18, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
            0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
            0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
            0x31, 0x32, 0x33, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    size_t Base64::EncodeLength(size_t byteLength) {
        return ((byteLength + 2) / 3) * 4;
    }

    void Base64::Encode(const char* data, size_t length, char* out) {
        if (!length)
            return;
        auto outLength = EncodeLength(length);

        unsigned sidx = 0;
        unsigned didx = 0;

        if (length > 1) {
            while (sidx < length - 2) {
                out[didx++] = base64EncMap[(data[sidx] >> 2) & 077];
                out[didx++] = base64EncMap[((data[sidx + 1] >> 4) & 017) | ((data[sidx] << 4) & 077)];
                out[didx++] = base64EncMap[((data[sidx + 2] >> 6) & 003) | ((data[sidx + 1] << 2) & 077)];
                out[didx++] = base64EncMap[data[sidx + 2] & 077];
                sidx += 3;
            }
        }

        if (sidx < length) {
            out[didx++] = base64EncMap[(data[sidx] >> 2) & 077];
            if (sidx < length - 1) {
                out[didx++] = base64EncMap[((data[sidx + 1] >> 4) & 017) | ((data[sidx] << 4) & 077)];
                out[didx++] = base64EncMap[(data[sidx + 1] << 2) & 077];
            } else {
                out[didx++] = base64EncMap[(data[sidx] << 4) & 077];
            }
        }

        while (didx < outLength) {
            out[didx] = '=';
            ++didx;
        }
    }

    size_t Base64::DecodeLength(size_t textLength) {
        if (!textLength || textLength % 4) {
            return 0;
        }
        return textLength / 4 * 3;
    }

    void Base64::Decode(const char* text, size_t length, char* result) {
        if (!length || length % 4) {
            return;
        }
        auto src = text;
        char* dest = result;
        unsigned index = 0;
        while (index < length) {
            index += 4;
            char a = base64DecMap[(unsigned char) *(src++)];
            char b = base64DecMap[(unsigned char) *(src++)];
            char c = base64DecMap[(unsigned char) *(src++)];
            char d = base64DecMap[(unsigned char) *(src++)];
            *(dest++) = (char) ((a << 2) | ((b & 0x30) >> 4));
            if (c == (char) -1)
                break;
            *(dest++) = (char) (((b & 0x0f) << 4) | ((c & 0x3c) >> 2));
            if (d == (char) -1)
                break;
            *(dest++) = (char) (((c & 0x03) << 6) | d);
        }
    }

}  // namespace cyder