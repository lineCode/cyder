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



#import <Cocoa/Cocoa.h>
#include "OSWindow.h"
#include "platform/Log.h"
#include "OSApplication.h"


namespace cyder {

    Window* Window::New(const WindowInitOptions& initOptions) {
        return new OSWindow(initOptions);
    }

    OSWindow::OSWindow(const WindowInitOptions& initOptions) {
        nsWindow = createNSWindow(initOptions);
        [nsWindow setAcceptsMouseMovedEvents:YES];
        NSRect windowRect = [nsWindow frame];
        NSUInteger windowStyle = [nsWindow styleMask];
        NSRect contentRect = [NSWindow contentRectForFrameRect:windowRect styleMask:windowStyle];
        OSView* customView = [[OSView alloc] initWithFrame:contentRect];
        osView = customView;
        customView.osWindow = this;


        [customView setTranslatesAutoresizingMaskIntoConstraints:NO];
        NSView* contentView = nsWindow.contentView;
        [contentView addSubview:customView];
        NSDictionary *views = NSDictionaryOfVariableBindings(customView);

        [contentView addConstraints:
        [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[customView]|"
        options:0
        metrics:nil
        views:views]];

        [contentView addConstraints:
        [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[customView]|"
        options:0
        metrics:nil
        views:views]];
    }

    OSWindow::~OSWindow() {
        if([nsWindow screen]){
            close();
        }
        [osView release];
        [nsWindow release];
    }

    void OSWindow::activate() {
        [nsWindow makeKeyAndOrderFront:nil];
    }

    void OSWindow::windowDidActivated() {
        auto app = static_cast<OSApplication*>(Application::application);
        app->windowActivated(this);
    }

    void OSWindow::close() {
        auto app = static_cast<OSApplication*>(Application::application);
        app->windowClosed(this);
        [nsWindow close];
    }


    std::string OSWindow::title() {
        return std::string([nsWindow.title UTF8String]);
    }
    void OSWindow::setTitle(const std::string& title) {
        nsWindow.title = [NSString stringWithCString:title.c_str() encoding:[NSString defaultCStringEncoding]];
    }

    float OSWindow::x() const {
        return nsWindow.frame.origin.x;
    }

    void OSWindow::setX(float value) {
        NSPoint point = NSMakePoint(value, y());
        [nsWindow setFrameOrigin:point];
    }

    float OSWindow::y() const {
        NSRect screenFrame = nsWindow.screen.frame;
        NSRect frame = nsWindow.frame;
        return screenFrame.size.height - frame.origin.y - frame.size.height;
    }

    void OSWindow::setY(float value) {
        NSRect screenFrame = nsWindow.screen.frame;
        NSRect frame = nsWindow.frame;
        float y = screenFrame.size.height - value - frame.size.height;
        NSPoint point = NSMakePoint(frame.origin.x, y);
        [nsWindow setFrameOrigin:point];
    }

    float OSWindow::width() const {
        return nsWindow.frame.size.width;
    }

    float OSWindow::height() const {
        return nsWindow.frame.size.height;
    }


    float OSWindow::contentWidth() const {
        return nsWindow.contentView.frame.size.width;
    }

    float OSWindow::contentHeight() const {
        return nsWindow.contentView.frame.size.height;
    }

    void OSWindow::setContentSize(float width, float height) {
        NSSize size = NSMakeSize(width, height);
        [nsWindow setContentSize:size];
    }

    GPUScreen* OSWindow::screen() {
        return osView.screen;
    }

    void OSWindow::setResizeCallback(std::function<void()> callback) {
        resizeCallback = callback;
    }

    NSWindow* OSWindow::createNSWindow(const WindowInitOptions& options){
        NSRect contentSize = NSMakeRect(0, 0, 500, 400);
        NSUInteger windowStyleMask = NSClosableWindowMask;
        if(options.systemChrome==WindowSystemChrome::STANDARD){
            windowStyleMask |= NSTitledWindowMask;
        }
        if(options.resizable){
            windowStyleMask |= NSResizableWindowMask;
        }
        if(options.minimizable){
            windowStyleMask |= NSMiniaturizableWindowMask;
        }
        NSWindow* window = [[NSWindow alloc] initWithContentRect:contentSize styleMask:windowStyleMask backing:NSBackingStoreBuffered defer:NO];
        if(options.systemChrome==WindowSystemChrome::NONE&&options.transparent){
            window.backgroundColor = [NSColor colorWithCalibratedWhite:1.0 alpha:0.0];
            [window setOpaque:NO];
        }
        return window;
    }

} // namespace cyder