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


#ifndef CYDER_DEBUGAGENT_H
#define CYDER_DEBUGAGENT_H

#include <string>
#include <v8-debug.h>
#include <thread>
#include "platform/Socket.h"

namespace cyder {

    /**
     * Debug agent which starts a socket listener on the debugger port and handles connection from a remote debugger.
     */
    class DebugAgent {
    public:

        /**
         * Enable the V8 debug agent. The debug agent will listen on the supplied TCP/IP port for remote debugger connection.
         * @param hostName the name of the embedding application.
         * @param port the TCP/IP port to listen on.
         * @param waitForConnection whether V8 should pause on a first statement allowing remote debugger to connect before
         * anything interesting happened.
         */
        static void Enable(const std::string& hostName, int port, bool waitForConnection = false);
        /**
         * Disable the V8 debug agent. The TCP/IP connection will be closed.
         */
        static void Disable();

    private:
        static DebugAgent* debugAgent;
        static bool initialized;

        DebugAgent(v8::Isolate* isolate, const std::string& hostName, int port, bool waitForConnection);
        ~DebugAgent();
        void runClientLoop();
        void onDebugMessage(const v8::Debug::Message& message);

        bool terminate;
        v8::Isolate* isolate;
        std::string hostName;
        int port;  // Port to use for the agent.
        Socket* server;  // Server socket for listen/accept.
        Socket* client;  // Client socket for send/receive.

        std::thread thread;

        friend class DebugAgentSession;

        friend void DebugAgentMessageHandler(const v8::Debug::Message& message);
    };

}  // namespace cyder

#endif //CYDER_DEBUGAGENT_H
