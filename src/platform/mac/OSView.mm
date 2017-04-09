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



#import "OSView.h"
#import "OSTicker.h"
#import "platform/Log.h"
#import "OSWindow.h"
#import "OSScreen.h"


@implementation OSView
@synthesize osWindow, screen;

-(BOOL) isFlipped {
    return YES;
}

- (BOOL) acceptsFirstResponder {
    return YES;
}


- (id) initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    osWindow = nil;
    screen = nil;

    [self setWantsBestResolutionOpenGLSurface:YES];
    return self;
}

- (void) viewDidMoveToWindow {
    [super viewDidMoveToWindow];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(backingPropertiesChanged:)
                                                 name:@"NSWindowDidChangeBackingPropertiesNotification"
                                               object:[self window]];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowWillClose:)
                                                 name:@"NSWindowWillCloseNotification"
                                               object:[self window]];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(windowDidBecomeKey:)
                                                 name:@"NSWindowDidBecomeKeyNotification"
                                               object:[self window]];

    screen = new OSScreen(self);
    
    [self performSelector:@selector(resetBackend) withObject:nil afterDelay:1];

}

-(void) resetBackend {
    // The window occurs a flicker of blank screen after dragging it to another monitor.
    // Reseting the opengl backend could fixes this problem.
    NSSize size = self.bounds.size;
    float scaleFactor = self.window.backingScaleFactor;
    screen->reset(size.width, size.height, scaleFactor);
    if(osWindow->resizeCallback){
        osWindow->resizeCallback();
    }
    // Update the screen immediately after the backend resetting to prevent flickering.
    [[OSTicker globalTicker] forceUpdate];
}

- (void) backingPropertiesChanged:(NSNotification *)notification {
    [self resetBackend];
}

- (void) windowWillClose:(NSNotification *)notification {
    screen->dispose();
}

- (void) windowDidBecomeKey:(NSNotification *)notification {
    osWindow->windowDidActivated();
}


- (void) updateScreenProperties {
    NSSize size = self.bounds.size;
    float scaleFactor = self.window.backingScaleFactor;
    screen->updateSize(size.width, size.height, scaleFactor);

    if(osWindow->resizeCallback){
        osWindow->resizeCallback();
    }
    // the main thread is fully blocked while window resizing, we need to trigger animation callbacks manually.
    [[OSTicker globalTicker] forceUpdate];
    
}

- (void) setFrameSize:(NSSize)newSize {
    [super setFrameSize:newSize];
    [self updateScreenProperties];
}


- (void) dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                 name:@"NSWindowDidChangeBackingPropertiesNotification"
                                               object:[self window]];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                 name:@"NSWindowWillCloseNotification"
                                               object:[self window]];
    
    [[NSNotificationCenter defaultCenter] removeObserver:self
                                                 name:@"NSWindowDidBecomeKeyNotification"
                                               object:[self window]];

    delete screen;
    [super dealloc];

}


@end
