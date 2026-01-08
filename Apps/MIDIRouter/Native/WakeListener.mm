#include "WakeListener.h"
#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

@interface WakeObserver : NSObject
{
@public
    Native::Callback callback;
}
- (void)onWake:(NSNotification*)n;
@end

@implementation WakeObserver
- (void)onWake:(NSNotification*)n
{
    if (callback)
        callback();
}
@end

namespace Native
{

struct WakeListener::Impl
{
    WakeObserver* observer;
};

WakeListener::WakeListener(Callback cb)
    : impl(std::make_unique<Impl>())
{
    impl->observer = [[WakeObserver alloc] init];
    impl->observer->callback = cb;

    [[[NSWorkspace sharedWorkspace] notificationCenter]
        addObserver:impl->observer
           selector:@selector(onWake:)
               name:NSWorkspaceDidWakeNotification
             object:nil];
}

WakeListener::~WakeListener()
{
    [[[NSWorkspace sharedWorkspace] notificationCenter]
        removeObserver:impl->observer];
    [impl->observer release];
}
} // namespace Native