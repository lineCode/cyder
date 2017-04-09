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

#ifndef CYDER_SOCKET_H
#define CYDER_SOCKET_H


namespace cyder {

    class Socket final {
    public:
        Socket();

        ~Socket();

        // Server initialization.
        bool bind(int port);
        bool listen(int backlog);
        Socket* accept();

        /**
         * Client initialization.
         */
        bool connect(const char* host, const char* port);

        /**
         *  Shutdown socket for both read and write. This causes blocking send and receive calls to exit.
         *  After |shutdown()| the Socket object cannot be used for any communication.
         */
        bool shutdown();

        // Data Transmission
        // Return 0 on failure.
        int send(const char* buffer, int length);
        int receive(char* buffer, int length);

        /**
         * Set the value of the SO_REUSEADDR socket option.
         */
        bool setReuseAddress(bool reuse_address);

        bool isValid() const;

        static int getLastError();


    private:
        class PlatformData;

        PlatformData* data;

        explicit Socket(PlatformData* data) : data(data) {}
    };

}  // namespace cyder

#endif  // CYDER_SOCKET_H
