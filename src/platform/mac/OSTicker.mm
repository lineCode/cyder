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



#import "OSTicker.h"


static OSTicker* ticker = nullptr;

void Ticker::Start(Updater updater) {
    ticker.updater = updater;
    if(updater) {
        [ticker startTicker];
    }
    else {
        [ticker stopTicker];
    }
}

void Ticker::Stop() {
    ticker.updater = nullptr;
    [ticker stopTicker];
}

static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink,
                                      const CVTimeStamp * inNow,
                                      const CVTimeStamp * inOutputTime,
                                      CVOptionFlags flagsIn,
                                      CVOptionFlags *  flagsOut,
                                      void * displayLinkContext )
{
    [static_cast<OSTicker*>(displayLinkContext) performSelectorOnMainThread: @selector(mainThreadUpdate) withObject:nil waitUntilDone:YES];
    return 0;
}

@implementation OSTicker
@synthesize updater;

+(OSTicker*) globalTicker {
    return ticker;
}

-(id) init {
    self = [super init];
    updater = nil;
    forceUpdated = false;
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);
    ticker = self;
    return self;
}

-(void) startTicker {
    if(!CVDisplayLinkIsRunning(displayLink)){
        CVDisplayLinkStart(displayLink);
    }
}

-(void) stopTicker {
    if(CVDisplayLinkIsRunning(displayLink)){
        CVDisplayLinkStop(displayLink);
    }
}

-(void) mainThreadUpdate {
    if(forceUpdated){
        // We have already triggered the updater() method, so skip this one.
        forceUpdated = false;
        return;
    }
    if(updater){
        updater();
    }
}

-(void) forceUpdate {
    if(updater){
        updater();
    }
    forceUpdated = true;
}

- (void)dealloc {
    CVDisplayLinkStop(displayLink);
    CVDisplayLinkRelease(displayLink);
    ticker = nullptr;
    [super dealloc];
}

@end
