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


#include "platform/Socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "platform/Log.h"

namespace cyder {

    static const int InvalidNativeHandle = -1;

    class Socket::PlatformData {
    public:
        PlatformData() {
            nativeHandle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        }

        explicit PlatformData(int nativeHandle) : nativeHandle(nativeHandle) {

        }

        int nativeHandle;
    };

    Socket::Socket() : data(new PlatformData) {
    }

    Socket::~Socket() {
        shutdown();
        delete data;
    }

    bool Socket::bind(int port) {
        ASSERT(port >= 0);
        ASSERT(port < 65536);
        if (!isValid()) {
            return false;
        }
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        sin.sin_port = htons(static_cast<uint16_t>(port));
        int result = ::bind(
                data->nativeHandle, reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin));
        return result == 0;
    }


    bool Socket::listen(int backlog) {
        if (!isValid()) {
            return false;
        }
        int result = ::listen(data->nativeHandle, backlog);
        return result == 0;
    }


    Socket* Socket::accept() {
        if (!isValid()) {
            return nullptr;
        }
        while (true) {
            int native_handle = ::accept(data->nativeHandle, nullptr, nullptr);
            if (native_handle == InvalidNativeHandle) {
                if (errno == EINTR) {
                    continue;
                }
                return nullptr;
            }
            return new Socket(new PlatformData(native_handle));
        }
    }


    bool Socket::connect(const char* host, const char* port) {
        ASSERT(nullptr != host);
        ASSERT(nullptr != port);
        if (!isValid()) {
            return false;
        }

        struct addrinfo* info = nullptr;
        struct addrinfo hint;
        memset(&hint, 0, sizeof(hint));
        hint.ai_family = AF_INET;
        hint.ai_socktype = SOCK_STREAM;
        hint.ai_protocol = IPPROTO_TCP;
        int result = ::getaddrinfo(host, port, &hint, &info);
        if (result != 0) {
            return false;
        }

        for (struct addrinfo* ai = info; ai != nullptr; ai = ai->ai_next) {
            while (true) {
                result = ::connect(
                        data->nativeHandle, ai->ai_addr, static_cast<int>(ai->ai_addrlen));
                if (result == 0) {
                    freeaddrinfo(info);
                    return true;
                }
                if (errno == EINTR) {
                    continue;
                }
                break;
            }
        }
        freeaddrinfo(info);
        return false;
    }


    bool Socket::shutdown() {
        if (!isValid()) {
            return false;
        }
        int result = ::shutdown(data->nativeHandle, SHUT_RDWR);
        ::close(data->nativeHandle);
        data->nativeHandle = InvalidNativeHandle;
        return result == 0;
    }


    int Socket::send(const char* buffer, int length) {
        static int value = 1;
        setsockopt(data->nativeHandle, SOL_SOCKET, SO_NOSIGPIPE, &value, sizeof(value));
        ASSERT(length <= 0 || buffer != nullptr);
        if (!isValid()) {
            return 0;
        }
        size_t offset = 0;
        while (offset < length) {
            auto result = ::send(data->nativeHandle, buffer + offset, length - offset, 0);
            if (result == 0) {
                break;
            } else if (result > 0) {
                ASSERT(result <= length - offset);
                offset += result;
            } else {
                if (errno == EINTR) {
                    continue;
                }
                return 0;
            }
        }
        return static_cast<int>(offset);
    }


    int Socket::receive(char* buffer, int length) {
        if (!isValid()) {
            return 0;
        }
        if (length <= 0) {
            return 0;
        }
        ASSERT(nullptr != buffer);
        while (true) {
            auto result = ::recv(data->nativeHandle, buffer, static_cast<size_t>(length), 0);
            if (result < 0) {
                if (errno == EINTR) {
                    continue;
                }
                return 0;
            }
            return static_cast<int>(result);
        }
    }


    bool Socket::setReuseAddress(bool reuse_address) {
        if (!isValid()) {
            return 0;
        }
        int v = reuse_address ? 1 : 0;
        int result = ::setsockopt(data->nativeHandle, SOL_SOCKET, SO_REUSEADDR,
                                  reinterpret_cast<char*>(&v), sizeof(v));
        return result == 0;
    }

    bool Socket::isValid() const {
        return data->nativeHandle != InvalidNativeHandle;
    }

    int Socket::getLastError() {
        return errno;
    }

}   // namespace cyder
