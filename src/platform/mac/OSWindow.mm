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



#import <Cocoa/Cocoa.h>
#include "OSWindow.h"
#include "OSApplication.h"
#include "OSAnimationFrame.h"

namespace cyder {

    Window* Window::New(const WindowInitOptions &initOptions) {
        return new OSWindow(initOptions);
    }

    OSWindow::OSWindow(const WindowInitOptions &initOptions) {
        nsWindow = createNSWindow(initOptions);
        [nsWindow setAcceptsMouseMovedEvents:YES];
        NSRect windowRect = [nsWindow frame];
        NSWindowStyleMask windowStyle = [nsWindow styleMask];
        NSRect contentRect = [NSWindow contentRectForFrameRect:windowRect styleMask:windowStyle];
        nsView = [[NSView alloc] initWithFrame:contentRect];
        [nsView setWantsBestResolutionOpenGLSurface:YES];
        [nsView setTranslatesAutoresizingMaskIntoConstraints:NO];
        NSView* contentView = nsWindow.contentView;
        [contentView addSubview:nsView];
        _screenBuffer = new ScreenBuffer(this);
        _screenBuffer->reset(nsView);
        osWindowDelegate = [[OSWindowDelegate alloc] initWithWindow:this];
        [nsWindow setDelegate:osWindowDelegate];
        NSDictionary* views = NSDictionaryOfVariableBindings(nsView);

        [contentView addConstraints:
                [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[nsView]|"
                                                        options:NSLayoutFormatDirectionLeadingToTrailing
                                                        metrics:nil
                                                          views:views]];

        [contentView addConstraints:
                [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[nsView]|"
                                                        options:NSLayoutFormatDirectionLeadingToTrailing
                                                        metrics:nil
                                                          views:views]];
    }

    OSWindow::~OSWindow() {
        delete _screenBuffer;
        [osWindowDelegate release];
        [nsView release];
        if (!opened) {
            [nsWindow release];
        }
    }

    void OSWindow::activate() {
        [nsWindow makeKeyAndOrderFront:nil];
        auto app = static_cast<OSApplication*>(Application::application);
        if (!opened) {
            opened = true;
            app->windowOpened(this);
        }
        OSAnimationFrame::ForceScreenUpdateNow();
    }


    void OSWindow::close() {
        [nsWindow close];
    }


    std::string OSWindow::title() {
        return std::string([nsWindow.title UTF8String]);
    }

    void OSWindow::setTitle(const std::string &title) {
        nsWindow.title = [NSString stringWithCString:title.c_str() encoding:[NSString defaultCStringEncoding]];
    }

    float OSWindow::x() const {
        return static_cast<float>(nsWindow.frame.origin.x);
    }

    void OSWindow::setX(float value) {
        NSPoint point = NSMakePoint(value, y());
        [nsWindow setFrameOrigin:point];
    }

    float OSWindow::y() const {
        NSRect screenFrame = nsWindow.screen.frame;
        NSRect frame = nsWindow.frame;
        return static_cast<float>(screenFrame.size.height - frame.origin.y - frame.size.height);
    }

    void OSWindow::setY(float value) {
        NSRect screenFrame = nsWindow.screen.frame;
        NSRect frame = nsWindow.frame;
        float y = static_cast<float>(screenFrame.size.height - value - frame.size.height);
        NSPoint point = NSMakePoint(frame.origin.x, y);
        [nsWindow setFrameOrigin:point];
    }

    float OSWindow::width() const {
        return static_cast<float>(nsWindow.frame.size.width);
    }

    float OSWindow::height() const {
        return static_cast<float>(nsWindow.frame.size.height);
    }


    float OSWindow::contentWidth() const {
        return static_cast<float>(nsWindow.contentView.frame.size.width);
    }

    float OSWindow::contentHeight() const {
        return static_cast<float>(nsWindow.contentView.frame.size.height);
    }

    void OSWindow::setContentSize(float width, float height) {
        NSSize size = NSMakeSize(width, height);
        [nsWindow setContentSize:size];
    }

    float OSWindow::scaleFactor() const {
        return static_cast<float>(nsWindow.backingScaleFactor);
    }

    void OSWindow::setDelegate(WindowDelegate* delegate) {
        this->delegate = delegate;
    }

    bool OSWindow::windowShouldClose() {
        if (delegate) {
            return delegate->onClosing();
        }
        return true;
    }

    void OSWindow::windowWillClose() {
        auto app = static_cast<OSApplication*>(Application::application);
        app->windowClosed(this);
        _screenBuffer->dispose();
        if (delegate) {
            delegate->onClosed();
        }
    }

    void OSWindow::windowDidBecomeKey() {
        if (delegate) {
            delegate->onFocusIn();
            OSAnimationFrame::ForceScreenUpdateNow();
        }
    }

    void OSWindow::windowDidResize() {
        NSSize size = nsWindow.contentView.frame.size;
        float scaleFactor = static_cast<float>(nsWindow.backingScaleFactor);
        _screenBuffer->updateSize(SkScalarRoundToInt(size.width * scaleFactor), SkScalarRoundToInt(size.height * scaleFactor));
        if (delegate) {
            delegate->onResized();
            OSAnimationFrame::ForceScreenUpdateNow();
        }
    }

    void OSWindow::windowDidChangeBackingProperties() {
        // The window occurs a flicker of blank screen after dragging it to another monitor.
        // Resetting the opengl backend could fixes this problem.
        _screenBuffer->reset(nsView);
        if (delegate) {
            delegate->onScaleFactorChanged();
            // Update the screen immediately after the backend resetting to prevent flickering.
            OSAnimationFrame::ForceScreenUpdateNow();
        }
    }

    NSWindow* OSWindow::createNSWindow(const WindowInitOptions &options) {
        NSRect contentSize = NSMakeRect(0, 0, 500, 400);
        NSWindowStyleMask windowStyleMask = NSClosableWindowMask;
        if (options.systemChrome == WindowSystemChrome::STANDARD) {
            windowStyleMask |= NSTitledWindowMask;
        }
        if (options.resizable) {
            windowStyleMask |= NSResizableWindowMask;
        }
        if (options.minimizable) {
            windowStyleMask |= NSMiniaturizableWindowMask;
        }
        NSWindow* window = [[NSWindow alloc] initWithContentRect:contentSize styleMask:windowStyleMask backing:NSBackingStoreBuffered defer:NO];
        if (options.systemChrome == WindowSystemChrome::NONE && options.transparent) {
            window.backgroundColor = [NSColor colorWithCalibratedWhite:1.0 alpha:0.0];
            [window setOpaque:NO];
        }
        return window;
    }

} // namespace cyder